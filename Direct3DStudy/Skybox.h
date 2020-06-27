#pragma once

#include <vector>
#include <string>
#include "Camera.h"
#include <wrl/client.h>

using namespace std;

class Skybox
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	Skybox() = default;
	~Skybox() = default;

	HRESULT InitResource(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wstring& cubemapFilename, float skySphereRadius, bool gengerateMips = false);

	void Draw(ID3D11DeviceContext* deviceContext, const Camera& camera);

private:
	HRESULT InitResource(ID3D11Device* device, float skySphereRadius);

private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	UINT m_IndexCount = 0;

	ComPtr<ID3D11ShaderResourceView> m_pTextureCubeSRV;

	ComPtr<ID3D11VertexShader> m_pVertexShader3D;	// 顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader3D;		// 像素着色器
};