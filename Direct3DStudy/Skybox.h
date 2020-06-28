#pragma once
#include "Transform.h"
#include <d3d11_1.h>
#include "Geometry.h"
#include <wrl\client.h>
#include "DXTrace.h"

// һ��������С����Ϸ������
class Skybox
{
public:
	Skybox();

	// ��ȡ����任
	Transform* GetTransform();

	// ���û�����
	template<class VertexType, class IndexType>
	void SetBuffer(ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData);
	// ��������
	void SetTexture(ID3D11ShaderResourceView* texture);

	// ����
	void Draw(ID3D11DeviceContext* deviceContext);

	// ���õ��Զ�����
	// �����������������ã����Զ�����Ҳ��Ҫ����������
	void SetDebugObjectName(const std::string& name);

	__declspec(property(get = GetTransform)) Transform* transform;
private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	Transform m_Transform;								// ����任��Ϣ
	ComPtr<ID3D11ShaderResourceView> m_pTexture;		// ����
	ComPtr<ID3D11Buffer> m_pVertexBuffer;				// ���㻺����
	ComPtr<ID3D11Buffer> m_pIndexBuffer;				// ����������
	UINT m_VertexStride;								// �����ֽڴ�С
	UINT m_IndexCount;								    // ������Ŀ	
};


template<class VertexType, class IndexType>
void Skybox::SetBuffer(ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData)
{
	// �ͷž���Դ
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();

	// ���ö��㻺��������
	m_VertexStride = sizeof(VertexType);
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * m_VertexStride;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// �½����㻺����
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	HR(device->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));


	// ������������������
	m_IndexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = m_IndexCount * sizeof(IndexType);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// �½�����������
	InitData.pSysMem = meshData.indexVec.data();
	HR(device->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));
}