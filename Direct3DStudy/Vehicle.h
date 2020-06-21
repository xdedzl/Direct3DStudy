#pragma once
#include "GameObject.h"
class Vehicle
{
public:
	GameObject main;
	GameObject wheels[4];
	void Awake(ID3D11Device* d3dDevice);
	void OnDraw(ID3D11DeviceContext* d3dDeviceContext);
	void OnUpdate(float dt);
};