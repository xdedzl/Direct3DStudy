#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <cstring>
#include <DirectXMath.h>

using namespace DirectX;

// 方向光
struct DirectionalLight
{
	DirectionalLight() = default;

	DirectionalLight(const DirectionalLight&) = default;
	DirectionalLight& operator=(const DirectionalLight&) = default;

	DirectionalLight(DirectionalLight&&) = default;
	DirectionalLight& operator=(DirectionalLight&&) = default;

	DirectionalLight(const XMFLOAT4& _ambient, const XMFLOAT4& _diffuse, const XMFLOAT4& _specular,
		const XMFLOAT3& _direction) :
		ambient(_ambient), diffuse(_diffuse), specular(_specular), direction(_direction), pad()
	{
	}

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 direction;
	float pad; // 最后用一个浮点数填充使得该结构体大小满足16的倍数，便于我们以后在HLSL设置数组
};

// 点光
struct PointLight
{
	PointLight() = default;

	PointLight(const PointLight&) = default;
	PointLight& operator=(const PointLight&) = default;

	PointLight(PointLight&&) = default;
	PointLight& operator=(PointLight&&) = default;

	PointLight(const XMFLOAT4& _ambient, const XMFLOAT4& _diffuse, const XMFLOAT4& _specular,
		const XMFLOAT3& _position, float _range, const XMFLOAT3& _att) :
		ambient(_ambient), diffuse(_diffuse), specular(_specular), position(_position), range(_range), att(_att), pad()
	{
	}

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;

	// 打包成4D向量: (position, range)
	XMFLOAT3 position;
	float range;

	// 打包成4D向量: (A0, A1, A2, pad)
	XMFLOAT3 att;
	float pad; // 最后用一个浮点数填充使得该结构体大小满足16的倍数，便于我们以后在HLSL设置数组
};

// 聚光灯
struct SpotLight
{
	SpotLight() = default;

	SpotLight(const SpotLight&) = default;
	SpotLight& operator=(const SpotLight&) = default;

	SpotLight(SpotLight&&) = default;
	SpotLight& operator=(SpotLight&&) = default;

	SpotLight(const XMFLOAT4& _ambient, const XMFLOAT4& _diffuse, const XMFLOAT4& _specular,
		const XMFLOAT3& _position, float _range, const XMFLOAT3& _direction,
		float _spot, const XMFLOAT3& _att) :
		ambient(_ambient), diffuse(_diffuse), specular(_specular),
		position(_position), range(_range), direction(_direction), spot(_spot), att(_att), pad()
	{
	}

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;

	// 打包成4D向量: (position, range)
	XMFLOAT3 position;
	float range;

	// 打包成4D向量: (direction, spot)
	XMFLOAT3 direction;
	float spot;

	// 打包成4D向量: (att, pad)
	XMFLOAT3 att;
	float pad; // 最后用一个浮点数填充使得该结构体大小满足16的倍数，便于我们以后在HLSL设置数组
};

// 物体表面材质
struct Material
{
	Material() = default;

	Material(const Material&) = default;
	Material& operator=(const Material&) = default;

	Material(Material&&) = default;
	Material& operator=(Material&&) = default;

	Material(const XMFLOAT4& _ambient, const XMFLOAT4& _diffuse, const XMFLOAT4& _specular, const XMFLOAT4& _reflect) :
		ambient(_ambient), diffuse(_diffuse), specular(_specular), reflect(_reflect)
	{
	}

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular; // w = 镜面反射强度
	XMFLOAT4 reflect;
};

#endif
