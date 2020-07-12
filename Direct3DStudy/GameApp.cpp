#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
#include "DDSTextureLoader.h"
#include "Input.h"
using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance),
	m_CBFrame(),
	m_CBOnResize(),
	m_CBRarely(),
	m_IsWireframeMode(false)
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	RenderStates::InitAll(m_pd3dDevice.Get());

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	// 初始化鼠标，键盘不需要
	Input::Instance()->Init(m_hMainWnd);

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();

	if (m_pCamera != nullptr)
	{
		m_pCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
		m_pCamera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
		m_CBOnResize.proj = XMMatrixTranspose(m_pCamera->GetProjXM());

		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[2].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		memcpy_s(mappedData.pData, sizeof(CBChangesOnResize), &m_CBOnResize, sizeof(CBChangesOnResize));
		m_pd3dImmediateContext->Unmap(m_pConstantBuffers[2].Get(), 0);
	}
}

void GameApp::UpdateScene(float dt)
{
	static float phi = 0.0f, theta = 0.0f, x = 0.0f, z = 0.0f;

	Input::Instance()->OnUpdate();

	// 键盘切换光栅化状态
	if (Input::Instance()->IsKeyPressed(Keyboard::L))
	{
		m_IsWireframeMode = !m_IsWireframeMode;
		if (m_IsWireframeMode)
			m_pd3dImmediateContext->RSSetState(RenderStates::RSWireframe.Get());
		else
			m_pd3dImmediateContext->RSSetState(nullptr);
	}
	if (Input::Instance()->IsKeyPressed(Keyboard::J))
	{
		m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	}

	if (Input::Instance()->IsKeyDown(Keyboard::W))
		z += dt * 2;
	if (Input::Instance()->IsKeyDown(Keyboard::S))
		z -= dt * 2;
	if (Input::Instance()->IsKeyDown(Keyboard::A))
		x -= dt * 2;
	if (Input::Instance()->IsKeyDown(Keyboard::D))
		x += dt * 2;

	auto camera = std::dynamic_pointer_cast<FollowCamera>(m_pCamera);
	camera->OnUpdate(dt);
	m_Skybox.GetTransform()->SetLocalPosition(m_pCamera->GetPosition());

	m_EndlessLand.OnUpdate(dt);
	m_Vehicle.OnUpdate(dt);

	// 更新观察矩阵
	XMStoreFloat4(&m_CBFrame.eyePos, m_pCamera->GetPositionXM());
	m_CBFrame.view = XMMatrixTranspose(m_pCamera->GetViewXM());

	// 退出程序，这里应向窗口发送销毁信息
	if (Input::Instance()->IsKeyDown(Keyboard::Escape))
		SendMessage(MainWnd(), WM_DESTROY, 0, 0);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBChangesEveryFrame), &m_CBFrame, sizeof(CBChangesEveryFrame));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);
}

void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);

	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&Colors::Black));
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// draw skybox
	m_pd3dImmediateContext->IASetInputLayout(m_SkyboxInputLayout.Get());
	m_pd3dImmediateContext->VSSetShader(m_SkyboxVS.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_SkyboxPS.Get(), nullptr, 0);

	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->PSSetSamplers(0, 1, RenderStates::SSLinearWrap.GetAddressOf());
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSLessEqual.Get(), 0);

	m_Skybox.Draw(m_pd3dImmediateContext.Get());


	// draw gameobject
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);

	m_pd3dImmediateContext->RSSetState(nullptr);
	m_pd3dImmediateContext->PSSetSamplers(0, 1, RenderStates::SSAnistropicWrap.GetAddressOf());
	m_pd3dImmediateContext->OMSetDepthStencilState(nullptr, 0);

	m_EndlessLand.OnDraw(m_pd3dImmediateContext.Get());
	m_Vehicle.OnDraw(m_pd3dImmediateContext.Get());

	m_Skybox.Draw(m_pd3dImmediateContext.Get());

	// 呈现最终画面
	HR(m_pSwapChain->Present(0, 0));
}

bool GameApp::InitEffect()
{
	ComPtr<ID3DBlob> blob;

	// 创建顶点着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_VS_3D.cso", L"HLSL\\Basic_VS_3D.hlsl", "VS_3D", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader3D.GetAddressOf()));

	// 创建顶点布局(3D)
	HR(m_pd3dDevice->CreateInputLayout(VertexPosNormalTex::inputLayout, ARRAYSIZE(VertexPosNormalTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout3D.GetAddressOf()));

	// 创建像素着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_PS_3D.cso", L"HLSL\\Basic_PS_3D.hlsl", "PS_3D", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader3D.GetAddressOf()));


	// skybox
	CreateShaderFromFile(L"HLSL\\Sky_VS.cso", L"HLSL\\Sky_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf());
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_SkyboxVS.GetAddressOf()));

	HR(m_pd3dDevice->CreateInputLayout(VertexPos::inputLayout, ARRAYSIZE(VertexPos::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_SkyboxInputLayout.GetAddressOf()));

	HR(CreateShaderFromFile(L"HLSL\\Sky_PS.cso", L"HLSL\\Sky_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_SkyboxPS.GetAddressOf()));

	return true;
}

bool GameApp::InitResource()
{
	// ******************
	// 初始化网格模型
	//
	//auto meshData = Geometry::CreateBox<VertexPosNormalColor>();
	//ResetMesh(meshData);

	// ******************
	// 设置常量缓冲区描述
	//
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 新建用于VS和PS的常量缓冲区
	cbd.ByteWidth = sizeof(CBChangesEveryDrawing);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[0].GetAddressOf()));
	cbd.ByteWidth = sizeof(CBChangesEveryFrame);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[1].GetAddressOf()));
	cbd.ByteWidth = sizeof(CBChangesOnResize);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[2].GetAddressOf()));
	cbd.ByteWidth = sizeof(CBChangesRarely);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[3].GetAddressOf()));

	auto camera = std::shared_ptr<FollowCamera>(new FollowCamera);
	m_pCamera = camera;
	camera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
	camera->SetTarget(m_Vehicle.main.GetTransform());
	m_pCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);

	// init skybox
	m_Skybox.SetBuffer(m_pd3dDevice.Get(), Geometry::CreateSphere<VertexPos, DWORD>(10));
	ComPtr<ID3D11ShaderResourceView> skyboxTexture;
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"Texture\\desert.dds", nullptr, skyboxTexture.ReleaseAndGetAddressOf()));
	m_Skybox.SetTexture(skyboxTexture.Get());
	m_Skybox.GetTransform()->SetLocalPosition(0.0f, 0.0f, 0.0f);

	// init land
	m_EndlessLand = EndlessLand();
	m_EndlessLand.Awake(m_pd3dDevice.Get());
	m_EndlessLand.SetTarget(m_pCamera->transform);

	// init vehicle
	m_Vehicle = Vehicle();
	m_Vehicle.Awake(m_pd3dDevice.Get());

	// ******************
	// 初始化常量缓冲区的值
	// 初始化每帧可能会变化的值

	// 初始化仅在窗口大小变动时修改的值
	m_CBOnResize.proj = XMMatrixTranspose(m_pCamera->GetProjXM());

	// 初始化不会变化的值
	// 环境光
	m_CBRarely.dirLight[0].ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_CBRarely.dirLight[0].diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_CBRarely.dirLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_CBRarely.dirLight[0].direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	// 灯光
	//m_CBRarely.pointLight[0].position = XMFLOAT3(0.0f, 10.0f, 0.0f);
	//m_CBRarely.pointLight[0].ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//m_CBRarely.pointLight[0].diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	//m_CBRarely.pointLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//m_CBRarely.pointLight[0].att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	//m_CBRarely.pointLight[0].range = 25.0f;
	m_CBRarely.numDirLight = 1;
	m_CBRarely.numPointLight = 0;
	m_CBRarely.numSpotLight = 0;
	// 初始化材质
	m_CBRarely.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_CBRarely.material.diffuse = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	m_CBRarely.material.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 50.0f);

	// 更新不容易被修改的常量缓冲区资源
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[2].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBChangesOnResize), &m_CBOnResize, sizeof(CBChangesOnResize));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[2].Get(), 0);

	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[3].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBChangesRarely), &m_CBRarely, sizeof(CBChangesRarely));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[3].Get(), 0);


	// ******************
	// 初始化光栅化状态
	//
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	HR(m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, m_pRSWireframe.GetAddressOf()));

	// ******************
	// 给渲染管线各个阶段绑定好所需资源
	// 设置图元类型，设定输入布局
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
	// 将着色器绑定到渲染管线
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
	// 预先绑定各自所需的缓冲区，其中每帧更新的缓冲区需要绑定到两个缓冲区上
	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffers[0].GetAddressOf());
	m_pd3dImmediateContext->VSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
	m_pd3dImmediateContext->VSSetConstantBuffers(2, 1, m_pConstantBuffers[2].GetAddressOf());

	m_pd3dImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(3, 1, m_pConstantBuffers[3].GetAddressOf());
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);

	return true;
}