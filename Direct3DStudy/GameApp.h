#pragma once

#include "d3dApp.h"
#include "ConstantBufferStruct.h"
#include "Geometry.h"
#include "Camera.h"
#include "GameObject.h"
#include "EndlessLand.h"
#include "Vehicle.h"
#include "RenderStates.h"
#include "Skybox.h"

class GameApp : public D3DApp
{
public:
	

public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();


private:
	bool InitEffect();
	bool InitResource();

private:
	ComPtr<ID3D11Buffer> m_pConstantBuffers[4];		// 常量缓冲区

	Skybox m_Skybox;
	Vehicle m_Vehicle;
	EndlessLand m_EndlessLand;

	ComPtr<ID3D11InputLayout> m_pVertexLayout3D;	// 用于3D的顶点输入布局
	ComPtr<ID3D11VertexShader> m_pVertexShader3D;	// 顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader3D;		// 像素着色器

	ComPtr<ID3D11InputLayout> m_SkyboxInputLayout;	
	ComPtr<ID3D11VertexShader> m_SkyboxVS;			
	ComPtr<ID3D11PixelShader> m_SkyboxPS;			

	CBChangesEveryFrame m_CBFrame;							    // 该缓冲区存放仅在每一帧进行更新的变量
	CBChangesOnResize m_CBOnResize;							    // 该缓冲区存放仅在窗口大小变化时更新的变量
	CBChangesRarely m_CBRarely;								    // 该缓冲区存放不会再进行修改的变量

	std::shared_ptr<Camera> m_pCamera;

	ComPtr<ID3D11RasterizerState> m_pRSWireframe;	// 光栅化状态：线框模式
	bool m_IsWireframeMode;							// 当前是否为线框模式
};