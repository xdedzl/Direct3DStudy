#pragma once
#include <DirectXMath.h>
#include "LightHelper.h"


struct CBChangesEveryDrawing
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldInvTranspose;
};

struct CBChangesEveryFrame
{
	DirectX::XMMATRIX view;
	DirectX::XMFLOAT4 eyePos;
};

struct CBChangesOnResize
{
	DirectX::XMMATRIX proj;
};

struct CBChangesRarely
{
	DirectionalLight dirLight[10];
	PointLight pointLight[10];
	SpotLight spotLight[10];
	Material material;
	int numDirLight;
	int numPointLight;
	int numSpotLight;
	float pad;			// 打包保证16字节对齐
};

struct VSConstantBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
	DirectX::XMMATRIX worldInvTranspose;
};

struct PSConstantBuffer
{
	DirectionalLight dirLight;
	PointLight pointLight;
	SpotLight spotLight;
	Material material;
	DirectX::XMFLOAT4 eyePos;
};