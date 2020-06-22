#pragma once
#include <DirectXMath.h>
using namespace DirectX;

constexpr float Deg2Rad = XM_PI / 180.0f;
constexpr float Rad2Deg = 57.29578f;

XMFLOAT3 Lerp(XMFLOAT3 x, XMFLOAT3 y, float s);
XMFLOAT3 Sub(XMFLOAT3 v1, XMFLOAT3 v2);
XMFLOAT3 Add(XMFLOAT3 v1, XMFLOAT3 v2);
XMFLOAT3 Mul(XMFLOAT3 v1, float s);

float Lerp(float x, float y, float s);

inline XMFLOAT3 Lerp(XMFLOAT3 x, XMFLOAT3 y, float s)
{
	auto plus = Mul(Sub(y, x), s);
	return Add(x, plus);
}

inline XMFLOAT3 Sub(XMFLOAT3 v1, XMFLOAT3 v2)
{
	return XMFLOAT3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline XMFLOAT3 Add(XMFLOAT3 v1, XMFLOAT3 v2)
{
	return XMFLOAT3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline XMFLOAT3 Mul(XMFLOAT3 v1, float s)
{
	return XMFLOAT3(v1.x * s, v1.y * s, v1.z * s);
}

inline float Lerp(float x, float y, float s)
{
	return x + (y - x) * s;
}