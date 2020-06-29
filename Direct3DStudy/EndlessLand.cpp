#include "EndlessLand.h"
#include "DXTrace.h"
#include "DDSTextureLoader.h"
#include "GameObject.h"
#include "Input.h"
#include "XMath.h"

using namespace DirectX;

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

float EndlessLand::LandWidth = 1000;


void EndlessLand::Awake(ID3D11Device* device)
{
	for (size_t i = 0; i < 9; i++)
	{
		ComPtr<ID3D11ShaderResourceView> texture;
		HR(CreateDDSTextureFromFile(device, L"Texture\\floor.dds", nullptr, texture.ReleaseAndGetAddressOf()));
		auto land = &m_Lands[i];
		land->SetBuffer(device, Geometry::CreatePlane(LandWidth, LandWidth, 10.0f, 10.0f));
		land->SetTexture(texture.Get());
	}

	RefreshPos(0, 0);
	xIndex = 0;
	zIndex = 0;
}

void EndlessLand::OnDraw(ID3D11DeviceContext* d3dDeviceContext)
{
	for (size_t i = 0; i < 9; i++)
	{
		m_Lands[i].Draw(d3dDeviceContext);
	}
}

void EndlessLand::OnUpdate(float dt)
{
	if (m_Target != nullptr)
	{
		int newX = m_Target->localPosition.x / LandWidth;
		int newZ = m_Target->localPosition.z / LandWidth;
		if (newX != xIndex || newZ != zIndex)
		{
			RefreshPos(newX, newZ);
			xIndex = newX;
			zIndex = newZ;
		}
	}
}

void EndlessLand::SetTarget(Transform* target)
{
	m_Target = target;
}

void EndlessLand::RefreshPos(int offsetX, int offsetZ)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			auto land = &m_Lands[i * 3 + j];
			float x = (j + offsetX -1) * LandWidth;
			float z = (i + offsetZ - 1) * LandWidth;
			land->transform->SetLocalPosition(x, -1.0f, z);
		}
	}
}