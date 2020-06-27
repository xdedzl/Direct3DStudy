#include "Skybox.h"
#include "Geometry.h"
#include "d3dUtil.h"
#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;

HRESULT Skybox::InitResource(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wstring& cubemapFilename, float skySphereRadius, bool gengerateMips)
{
	m_pIndexBuffer.Reset();
	m_pVertexBuffer.Reset();
	m_pTextureCubeSRV.Reset();

	HRESULT hr;

	hr = CreateDDSTextureFromFile(device, gengerateMips ? deviceContext : nullptr, cubemapFilename.c_str(), nullptr, m_pTextureCubeSRV.GetAddressOf());

	if (FAILED(hr))
		return hr;

	return InitResource(device, skySphereRadius);
}

void Skybox::Draw(ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	UINT strides[1] = { sizeof(XMFLOAT3) };
	UINT offsets[1] = { 0 };
	deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), strides, offsets);
	deviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	XMFLOAT3 pos = camera.GetPosition();


}

HRESULT Skybox::InitResource(ID3D11Device* device, float skySphereRadius)
{
	HRESULT hr;
	auto sphere = Geometry::CreateSphere<VertexPos>(skySphereRadius);

	// vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * (UINT)sphere.vertexVec.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = sphere.vertexVec.data();

	hr = device->CreateBuffer(&vbd, &InitData, &m_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// index buffer
	m_IndexCount = (UINT)sphere.indexVec.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * m_IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	InitData.pSysMem = sphere.indexVec.data();

	return device->CreateBuffer(&ibd, &InitData, &m_pIndexBuffer);
}