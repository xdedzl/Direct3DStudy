#pragma once
#include <memory>
#include "LightHelper.h"
#include "RenderStates.h"
#include "ConstantBufferStruct.h"

using namespace DirectX;

class IEffect
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	IEffect() = default;
	~IEffect() = default;

	// ���������������ƶ�
	//IEffect(const IEffect&) = delete;
	//IEffect& operator=(const IEffect&) = delete;
	//IEffect(IEffect&&) = default;
	//IEffect& operator=(IEffect&&) = default;

	// ���²��󶨳���������
	virtual void Apply(ID3D11DeviceContext* deviceContext) = 0;
};

class XEffect : public IEffect
{
public:
	bool InitAll(ID3D11Device* device);

	// Ӧ�ó�����������������Դ�ı��
	void Apply(ID3D11DeviceContext* deviceContext);

	void SetProjMatrix(ID3D11DeviceContext* deviceContext, CBChangesOnResize* cbChangeOnResize);

	void SetViewMatrix(ID3D11DeviceContext* deviceContext, CBChangesEveryFrame* cBChangesEveryFrame);

	void SetLight(ID3D11DeviceContext* deviceContext, CBChangesRarely* cBChangesRarely);

private:
	ComPtr<ID3D11InputLayout> m_pVertexLayout3D;	// ����3D�Ķ������벼��
	ComPtr<ID3D11Buffer> m_pConstantBuffers[4];		// ����������

	ComPtr<ID3D11VertexShader> m_pVertexShader3D;	// ������ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader3D;		// ������ɫ��
};

//class BasicEffect : public IEffect
//{
//public:
//
//	BasicEffect();
//	virtual ~BasicEffect();
//
//	BasicEffect(BasicEffect&& moveFrom) noexcept;
//	BasicEffect& operator=(BasicEffect&& moveFrom) noexcept;
//
//	// ��ȡ����
//	static BasicEffect& Get();
//
//	// ��ʼ��������Դ
//	bool InitAll(ID3D11Device* device);
//
//	// 
//	// ��Ⱦģʽ�ı��
//	//
//
//	// Ĭ��״̬������
//	void SetRenderDefault(ID3D11DeviceContext* deviceContext);
//
//	//
//	// ��������
//	//
//
//	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W);
//	void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V);
//	void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P);
//
//	//
//	// ���ա����ʺ������������
//	//
//
//	// �������͵ƹ�����������Ŀ
//	static const int maxLights = 5;
//
//	void SetDirLight(size_t pos, const DirectionalLight& dirLight);
//	void SetPointLight(size_t pos, const PointLight& pointLight);
//	void SetSpotLight(size_t pos, const SpotLight& spotLight);
//
//	void SetMaterial(const Material& material);
//
//	void SetTextureUsed(bool isUsed);
//
//	void SetTextureDiffuse(ID3D11ShaderResourceView* textureDiffuse);
//	void SetTextureCube(ID3D11ShaderResourceView* textureCube);
//
//	void XM_CALLCONV SetEyePos(DirectX::FXMVECTOR eyePos);
//
//	//
//	// ״̬��������
//	//
//
//	void SetReflectionEnabled(bool isEnable);
//
//
//	// Ӧ�ó�����������������Դ�ı��
//	void Apply(ID3D11DeviceContext* deviceContext);
//
//private:
//	class Impl;
//	std::unique_ptr<Impl> pImpl;
//};

//class SkyEffect : public IEffect
//{
//public:
//	SkyEffect();
//	virtual ~SkyEffect() override;
//
//	SkyEffect(SkyEffect&& moveFrom) noexcept;
//	SkyEffect& operator=(SkyEffect&& moveFrom) noexcept;
//
//	// ��ȡ����
//	static SkyEffect& Get();
//
//	// ��ʼ��������Դ
//	bool InitAll(ID3D11Device* device);
//
//	// 
//	// ��Ⱦģʽ�ı��
//	//
//
//	// Ĭ��״̬������
//	void SetRenderDefault(ID3D11DeviceContext* deviceContext);
//
//	//
//	// ��������
//	//
//
//	void XM_CALLCONV SetWorldViewProjMatrix(DirectX::FXMMATRIX W, DirectX::CXMMATRIX V, DirectX::CXMMATRIX P);
//	void XM_CALLCONV SetWorldViewProjMatrix(DirectX::FXMMATRIX WVP);
//
//	//
//	// ����������ӳ������
//	//
//
//	void SetTextureCube(ID3D11ShaderResourceView* textureCube);
//
//
//	// Ӧ�ó�����������������Դ�ı��
//	void Apply(ID3D11DeviceContext* deviceContext);
//
//private:
//	class Impl;
//	std::unique_ptr<Impl> pImpl;
//
//};