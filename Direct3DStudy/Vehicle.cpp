#include "Vehicle.h"
#include "DXTrace.h"
#include "DDSTextureLoader.h"

using namespace DirectX;

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
void Vehicle::Awake(ID3D11Device* d3dDevice)
{
	ComPtr<ID3D11ShaderResourceView> texture;
	// 初始化木箱
	(CreateDDSTextureFromFile(d3dDevice, L"Texture\\WoodCrate.dds", nullptr, texture.GetAddressOf()));
	main.SetBuffer(d3dDevice, Geometry::CreateBox());
	main.SetTexture(texture.Get());
	main.GetTransform().SetPosition(0.0f, 0.0f, 0.0f);
}

void Vehicle::OnDraw(ID3D11DeviceContext* d3dDeviceContext)
{
	main.Draw(d3dDeviceContext);
}