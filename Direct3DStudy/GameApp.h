#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "LightHelper.h"
#include "Geometry.h"

class GameApp : public D3DApp
{
public:

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
	bool ResetMesh(const Geometry::MeshData<VertexPosNormalColor>& meshData);


private:
	ComPtr<ID3D11InputLayout> m_pVertexLayout;	    // �������벼��
	ComPtr<ID3D11Buffer> m_pVertexBuffer;			// ���㻺����
	ComPtr<ID3D11Buffer> m_pIndexBuffer;			// ����������
	ComPtr<ID3D11Buffer> m_pConstantBuffers[2];		// ����������
	UINT m_IndexCount;								// ������������������С

	ComPtr<ID3D11VertexShader> m_pVertexShader;	    // ������ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader;		// ������ɫ��
	VSConstantBuffer m_VSConstantBuffer;	            // �����޸�GPU�����������ı���
	PSConstantBuffer m_PSConstantBuffer;             // �����޸�GPU�����������ı���

	DirectionalLight m_DirLight;					// Ĭ�ϻ�����
	PointLight m_PointLight;						// Ĭ�ϵ��
	SpotLight m_SpotLight;							// Ĭ�Ͼ۹��

	ComPtr<ID3D11RasterizerState> m_pRSWireframe;	// ��դ��״̬���߿�ģʽ
	bool m_IsWireframeMode;							// ��ǰ�Ƿ�Ϊ�߿�ģʽ
};


#endif