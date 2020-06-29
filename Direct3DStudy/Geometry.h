#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>
#include "Vertex.h"

using namespace DirectX;
using namespace std;

namespace Geometry
{
	// ��������
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	struct MeshData
	{
		vector<VertexType> vertexVec;	// ��������
		vector<IndexType> indexVec;	// ��������

		MeshData()
		{
			// ������������ͺϷ���
			static_assert(sizeof(IndexType) == 2 || sizeof(IndexType) == 4, "The size of IndexType must be 2 bytes or 4 bytes!");
			static_assert(is_unsigned<IndexType>::value, "IndexType must be unsigned integer!");
		}
	};

	// ���������������ݣ�levels��slicesԽ�󣬾���Խ�ߡ�
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateSphere(float radius = 1.0f, UINT levels = 20, UINT slices = 20,
		const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// ������������������
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f,
		const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// ����Բ�����������ݣ�slicesԽ�󣬾���Խ�ߡ�
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateCylinder(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// ����ֻ��Բ���������������ݣ�slicesԽ�󣬾���Խ��
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateCylinderNoCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// ����һ��ƽ��
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreatePlane(float width = 10.0f, float depth = 10.0f, float texU = 1.0f, float texV = 1.0f,
		const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
}

namespace Geometry
{
	namespace Internal
	{
		//
		// ���½ṹ��ͺ��������ڲ�ʵ��ʹ��
		//

		struct VertexData
		{
			XMFLOAT3 pos;
			XMFLOAT3 normal;
			XMFLOAT4 tangent;
			XMFLOAT4 color;
			XMFLOAT2 tex;
		};

		// ����Ŀ�궥������ѡ���Խ����ݲ���
		template<class VertexType>
		inline void InsertVertexElement(VertexType& vertexDst, const VertexData& vertexSrc)
		{
			static string semanticName;
			static const map<string, pair<size_t, size_t>> semanticSizeMap = {
				{"POSITION", pair<size_t, size_t>(0, 12)},
				{"NORMAL", pair<size_t, size_t>(12, 24)},
				{"TANGENT", pair<size_t, size_t>(24, 40)},
				{"COLOR", pair<size_t, size_t>(40, 56)},
				{"TEXCOORD", pair<size_t, size_t>(56, 64)}
			};

			for (size_t i = 0; i < ARRAYSIZE(VertexType::inputLayout); i++)
			{
				semanticName = VertexType::inputLayout[i].SemanticName;
				const auto& range = semanticSizeMap.at(semanticName);
				memcpy_s(reinterpret_cast<char*>(&vertexDst) + VertexType::inputLayout[i].AlignedByteOffset,
					range.second - range.first,
					reinterpret_cast<const char*>(&vertexSrc) + range.first,
					range.second - range.first);
			}
		}
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreateSphere(float radius, UINT levels, UINT slices, const XMFLOAT4& color)
	{
		MeshData<VertexType, IndexType> meshData;
		UINT vertexCount = 2 + (levels - 1) * (slices + 1);
		UINT indexCount = 6 * (levels - 1) * slices;
		meshData.vertexVec.resize(vertexCount);
		meshData.indexVec.resize(indexCount);

		Internal::VertexData vertexData;
		IndexType vIndex = 0, iIndex = 0;

		float phi = 0.0f, theta = 0.0f;
		float per_phi = XM_PI / levels;
		float per_theta = XM_2PI / slices;
		float x, y, z;

		// ���붥�˵�
		vertexData = { XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 0.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		for (UINT i = 1; i < levels; ++i)
		{
			phi = per_phi * i;
			// ��Ҫslices + 1����������Ϊ �����յ���Ϊͬһ�㣬����������ֵ��һ��
			for (UINT j = 0; j <= slices; ++j)
			{
				theta = per_theta * j;
				x = radius * sinf(phi) * cosf(theta);
				y = radius * cosf(phi);
				z = radius * sinf(phi) * sinf(theta);
				// ������ֲ����ꡢ��������Tangent��������������
				XMFLOAT3 pos = XMFLOAT3(x, y, z), normal;
				XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));

				vertexData = { pos, normal, XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(theta / XM_2PI, phi / XM_PI) };
				Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
			}
		}

		// ����׶˵�
		vertexData = { XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),
			XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 1.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);


		// ��������
		if (levels > 1)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				meshData.indexVec[iIndex++] = 0;
				meshData.indexVec[iIndex++] = j % (slices + 1) + 1;
				meshData.indexVec[iIndex++] = j;
			}
		}


		for (UINT i = 1; i < levels - 1; ++i)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
				meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
				meshData.indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

				meshData.indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
				meshData.indexVec[iIndex++] = i * (slices + 1) + j;
				meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
			}
		}

		// �𽥷�������
		if (levels > 1)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				meshData.indexVec[iIndex++] = (levels - 2) * (slices + 1) + j;
				meshData.indexVec[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
				meshData.indexVec[iIndex++] = (levels - 1) * (slices + 1) + 1;
			}
		}


		return meshData;
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreateBox(float width, float height, float depth, const XMFLOAT4& color)
	{
		MeshData<VertexType, IndexType> meshData;
		meshData.vertexVec.resize(24);


		Internal::VertexData vertexDataArr[24];
		float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

		// ����(+X��)
		vertexDataArr[0].pos = XMFLOAT3(w2, -h2, -d2);
		vertexDataArr[1].pos = XMFLOAT3(w2, h2, -d2);
		vertexDataArr[2].pos = XMFLOAT3(w2, h2, d2);
		vertexDataArr[3].pos = XMFLOAT3(w2, -h2, d2);
		// ����(-X��)
		vertexDataArr[4].pos = XMFLOAT3(-w2, -h2, d2);
		vertexDataArr[5].pos = XMFLOAT3(-w2, h2, d2);
		vertexDataArr[6].pos = XMFLOAT3(-w2, h2, -d2);
		vertexDataArr[7].pos = XMFLOAT3(-w2, -h2, -d2);
		// ����(+Y��)
		vertexDataArr[8].pos = XMFLOAT3(-w2, h2, -d2);
		vertexDataArr[9].pos = XMFLOAT3(-w2, h2, d2);
		vertexDataArr[10].pos = XMFLOAT3(w2, h2, d2);
		vertexDataArr[11].pos = XMFLOAT3(w2, h2, -d2);
		// ����(-Y��)
		vertexDataArr[12].pos = XMFLOAT3(w2, -h2, -d2);
		vertexDataArr[13].pos = XMFLOAT3(w2, -h2, d2);
		vertexDataArr[14].pos = XMFLOAT3(-w2, -h2, d2);
		vertexDataArr[15].pos = XMFLOAT3(-w2, -h2, -d2);
		// ����(+Z��)
		vertexDataArr[16].pos = XMFLOAT3(w2, -h2, d2);
		vertexDataArr[17].pos = XMFLOAT3(w2, h2, d2);
		vertexDataArr[18].pos = XMFLOAT3(-w2, h2, d2);
		vertexDataArr[19].pos = XMFLOAT3(-w2, -h2, d2);
		// ����(-Z��)
		vertexDataArr[20].pos = XMFLOAT3(-w2, -h2, -d2);
		vertexDataArr[21].pos = XMFLOAT3(-w2, h2, -d2);
		vertexDataArr[22].pos = XMFLOAT3(w2, h2, -d2);
		vertexDataArr[23].pos = XMFLOAT3(w2, -h2, -d2);

		for (UINT i = 0; i < 4; ++i)
		{
			// ����(+X��)
			vertexDataArr[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertexDataArr[i].tangent = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			vertexDataArr[i].color = color;
			// ����(-X��)
			vertexDataArr[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
			vertexDataArr[i + 4].tangent = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
			vertexDataArr[i + 4].color = color;
			// ����(+Y��)
			vertexDataArr[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertexDataArr[i + 8].tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertexDataArr[i + 8].color = color;
			// ����(-Y��)
			vertexDataArr[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertexDataArr[i + 12].tangent = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
			vertexDataArr[i + 12].color = color;
			// ����(+Z��)
			vertexDataArr[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
			vertexDataArr[i + 16].tangent = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
			vertexDataArr[i + 16].color = color;
			// ����(-Z��)
			vertexDataArr[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertexDataArr[i + 20].tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertexDataArr[i + 20].color = color;
		}

		for (UINT i = 0; i < 6; ++i)
		{
			vertexDataArr[i * 4].tex = XMFLOAT2(0.0f, 1.0f);
			vertexDataArr[i * 4 + 1].tex = XMFLOAT2(0.0f, 0.0f);
			vertexDataArr[i * 4 + 2].tex = XMFLOAT2(1.0f, 0.0f);
			vertexDataArr[i * 4 + 3].tex = XMFLOAT2(1.0f, 1.0f);
		}

		for (UINT i = 0; i < 24; ++i)
		{
			Internal::InsertVertexElement(meshData.vertexVec[i], vertexDataArr[i]);
		}

		meshData.indexVec = {
			0, 1, 2, 2, 3, 0,		// ����(+X��)
			4, 5, 6, 6, 7, 4,		// ����(-X��)
			8, 9, 10, 10, 11, 8,	// ����(+Y��)
			12, 13, 14, 14, 15, 12,	// ����(-Y��)
			16, 17, 18, 18, 19, 16, // ����(+Z��)
			20, 21, 22, 22, 23, 20	// ����(-Z��)
		};

		return meshData;
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreateCylinder(float radius, float height, UINT slices, const XMFLOAT4& color)
	{
		auto meshData = CreateCylinderNoCap<VertexType, IndexType>(radius, height, slices, color);
		UINT vertexCount = 4 * (slices + 1) + 2;
		UINT indexCount = 12 * slices;
		meshData.vertexVec.resize(vertexCount);
		meshData.indexVec.resize(indexCount);

		float h2 = height / 2;
		float theta = 0.0f;
		float per_theta = XM_2PI / slices;

		IndexType vIndex = 2 * (slices + 1), iIndex = 6 * slices;
		IndexType offset = 2 * (slices + 1);
		Internal::VertexData vertexData;

		// ���붥��Բ��
		vertexData = { XMFLOAT3(0.0f, h2, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.5f, 0.5f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		// ���붥��Բ�ϸ���
		for (UINT i = 0; i <= slices; ++i)
		{
			theta = i * per_theta;
			vertexData = { XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta)), XMFLOAT3(0.0f, 1.0f, 0.0f),
				XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
			Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
		}

		// ����׶�Բ��
		vertexData = { XMFLOAT3(0.0f, -h2, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),
			XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.5f, 0.5f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		// ����ײ�Բ�ϸ���
		for (UINT i = 0; i <= slices; ++i)
		{
			theta = i * per_theta;
			vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT3(0.0f, -1.0f, 0.0f),
				XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
			Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
		}



		// ���붥������������
		for (UINT i = 1; i <= slices; ++i)
		{
			meshData.indexVec[iIndex++] = offset;
			meshData.indexVec[iIndex++] = offset + i % (slices + 1) + 1;
			meshData.indexVec[iIndex++] = offset + i;
		}

		// ����ײ�����������
		offset += slices + 2;
		for (UINT i = 1; i <= slices; ++i)
		{
			meshData.indexVec[iIndex++] = offset;
			meshData.indexVec[iIndex++] = offset + i;
			meshData.indexVec[iIndex++] = offset + i % (slices + 1) + 1;
		}

		return meshData;
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreateCylinderNoCap(float radius, float height, UINT slices, const XMFLOAT4& color)
	{
		MeshData<VertexType, IndexType> meshData;
		UINT vertexCount = 2 * (slices + 1);
		UINT indexCount = 6 * slices;
		meshData.vertexVec.resize(vertexCount);
		meshData.indexVec.resize(indexCount);

		float h2 = height / 2;
		float theta = 0.0f;
		float per_theta = XM_2PI / slices;

		Internal::VertexData vertexData;

		// ������涥�˵�
		for (UINT i = 0; i <= slices; ++i)
		{
			theta = i * per_theta;
			vertexData = { XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta)), XMFLOAT3(cosf(theta), 0.0f, sinf(theta)),
				XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(theta / XM_2PI, 0.0f) };
			Internal::InsertVertexElement(meshData.vertexVec[i], vertexData);
		}

		// �������׶˵�
		for (UINT i = 0; i <= slices; ++i)
		{
			theta = i * per_theta;
			vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT3(cosf(theta), 0.0f, sinf(theta)),
				XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(theta / XM_2PI, 1.0f) };
			UINT vIndex = (slices + 1) + i;
			Internal::InsertVertexElement(meshData.vertexVec[vIndex], vertexData);
		}

		// ��������
		UINT iIndex = 0;

		for (UINT i = 0; i < slices; ++i)
		{
			meshData.indexVec[iIndex++] = i;
			meshData.indexVec[iIndex++] = i + 1;
			meshData.indexVec[iIndex++] = (slices + 1) + i + 1;

			meshData.indexVec[iIndex++] = (slices + 1) + i + 1;
			meshData.indexVec[iIndex++] = (slices + 1) + i;
			meshData.indexVec[iIndex++] = i;
		}


		return meshData;
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreatePlane(float width, float depth, float texU, float texV, const XMFLOAT4& color)
	{
		auto normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		auto tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		MeshData<VertexType, IndexType> meshData;
		meshData.vertexVec.resize(4);

		Internal::VertexData vertexData;
		UINT vIndex = 0;

		vertexData = { XMFLOAT3(-width / 2, 0.0f, -depth / 2), normal, tangent, color, XMFLOAT2(0.0f, texV) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		vertexData = { XMFLOAT3(-width / 2, 0.0f, depth / 2), normal, tangent, color, XMFLOAT2(0.0f, 0.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		vertexData = { XMFLOAT3(width / 2, 0.0f, depth / 2), normal, tangent, color, XMFLOAT2(texU, 0.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		vertexData = { XMFLOAT3(width / 2, 0.0f, -depth / 2), normal, tangent, color, XMFLOAT2(texU, texV) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		meshData.indexVec = { 0, 1, 2, 2, 3, 0 };
		return meshData;
	}
}