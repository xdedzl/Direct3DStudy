#pragma once

#include "GameObject.h"
#include "Camera.h"

class EndlessLand
{
public:
	void Awake(ID3D11Device* d3dDevice);
	void OnUpdate(float dt);
	void OnDraw(ID3D11DeviceContext* d3dDeviceContext);
	void SetTarget(Transform* target);
	void RefreshPos(int offsetX, int offsetZ);
private:
	GameObject m_Lands[9];
	Camera* m_Camera;
	Transform* m_Target;
	int xIndex;
	int zIndex;
	static float LandWidth;
};