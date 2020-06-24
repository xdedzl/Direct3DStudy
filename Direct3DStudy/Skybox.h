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

	HRESULT InitResource(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wstring& cubemapFilename, float skySphereRadius, bool hgengerateMips = false);

	void Draw(ID3D11DeviceContext* deviceContext);
};