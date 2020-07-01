#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <cstring>
#include <DirectXMath.h>

using namespace DirectX;

// �����
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
	float pad; // �����һ�����������ʹ�øýṹ���С����16�ı��������������Ժ���HLSL��������
};

// ���
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

	// �����4D����: (position, range)
	XMFLOAT3 position;
	float range;

	// �����4D����: (A0, A1, A2, pad)
	XMFLOAT3 att;
	float pad; // �����һ�����������ʹ�øýṹ���С����16�ı��������������Ժ���HLSL��������
};

// �۹��
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

	// �����4D����: (position, range)
	XMFLOAT3 position;
	float range;

	// �����4D����: (direction, spot)
	XMFLOAT3 direction;
	float spot;

	// �����4D����: (att, pad)
	XMFLOAT3 att;
	float pad; // �����һ�����������ʹ�øýṹ���С����16�ı��������������Ժ���HLSL��������
};

// ����������
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
	XMFLOAT4 specular; // w = ���淴��ǿ��
	XMFLOAT4 reflect;
};

#endif
