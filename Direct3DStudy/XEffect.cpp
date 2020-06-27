#include "Effects.h"
#include "ConstantBufferStruct.h"
#include "DXTrace.h"
#include "d3dUtil.h"
#include "Vertex.h"

bool XEffect::InitAll(ID3D11Device* device)
{
	ComPtr<ID3DBlob> blob;

	// ����������ɫ��(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_VS_3D.cso", L"HLSL\\Basic_VS_3D.hlsl", "VS_3D", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader3D.GetAddressOf()));

	// �������㲼��(3D)
	HR(device->CreateInputLayout(VertexPosNormalTex::inputLayout, ARRAYSIZE(VertexPosNormalTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout3D.GetAddressOf()));

	// ����������ɫ��(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_PS_3D.cso", L"HLSL\\Basic_PS_3D.hlsl", "PS_3D", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader3D.GetAddressOf()));


	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// �½�����VS��PS�ĳ���������
	cbd.ByteWidth = sizeof(CBChangesEveryDrawing);
	HR(device->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[0].GetAddressOf()));
	cbd.ByteWidth = sizeof(CBChangesEveryFrame);
	HR(device->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[1].GetAddressOf()));
	cbd.ByteWidth = sizeof(CBChangesOnResize);
	HR(device->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[2].GetAddressOf()));
	cbd.ByteWidth = sizeof(CBChangesRarely);
	HR(device->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[3].GetAddressOf()));

	return TRUE;
}

void XEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	// ******************
	// ����Ⱦ���߸����׶ΰ󶨺�������Դ
	// ����ͼԪ���ͣ��趨���벼��
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(m_pVertexLayout3D.Get());
	// ����ɫ���󶨵���Ⱦ����
	deviceContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
	// Ԥ�Ȱ󶨸�������Ļ�����������ÿ֡���µĻ�������Ҫ�󶨵�������������
	deviceContext->VSSetConstantBuffers(0, 1, m_pConstantBuffers[0].GetAddressOf());
	deviceContext->VSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
	deviceContext->VSSetConstantBuffers(2, 1, m_pConstantBuffers[2].GetAddressOf());

	deviceContext->PSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
	deviceContext->PSSetConstantBuffers(3, 1, m_pConstantBuffers[3].GetAddressOf());
	deviceContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
}

void XEffect::SetProjMatrix(ID3D11DeviceContext* deviceContext, CBChangesOnResize* cbChangeOnResize)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(deviceContext->Map(m_pConstantBuffers[2].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBChangesOnResize), cbChangeOnResize, sizeof(CBChangesOnResize));
	deviceContext->Unmap(m_pConstantBuffers[2].Get(), 0);
};

void XEffect::SetViewMatrix(ID3D11DeviceContext* deviceContext, CBChangesEveryFrame* cBChangesEveryFrame)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(deviceContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBChangesEveryFrame), cBChangesEveryFrame, sizeof(CBChangesEveryFrame));
	deviceContext->Unmap(m_pConstantBuffers[1].Get(), 0);
};

void XEffect::SetLight(ID3D11DeviceContext* deviceContext, CBChangesRarely* cBChangesRarely)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(deviceContext->Map(m_pConstantBuffers[3].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBChangesRarely), &cBChangesRarely, sizeof(CBChangesRarely));
	deviceContext->Unmap(m_pConstantBuffers[3].Get(), 0);
};