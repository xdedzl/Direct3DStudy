#include "Skybox.h"
#include "ConstantBufferStruct.h"
#include "DXTrace.h"
using namespace DirectX;

Skybox::Skybox()
	: m_IndexCount(), m_VertexStride()
{
}

Transform* Skybox::GetTransform()
{
	return &m_Transform;
}

void Skybox::SetTexture(ID3D11ShaderResourceView* texture)
{
	m_pTexture = texture;
}

void Skybox::Draw(ID3D11DeviceContext* deviceContext)
{
	// ���ö���/����������
	UINT strides = m_VertexStride;
	UINT offsets = 0;
	deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &strides, &offsets);
	deviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// ��ȡ֮ǰ�Ѿ��󶨵���Ⱦ�����ϵĳ����������������޸�
	ComPtr<ID3D11Buffer> cBuffer = nullptr;
	deviceContext->VSGetConstantBuffers(0, 1, cBuffer.GetAddressOf());
	CBChangesEveryDrawing cbDrawing;

	// �ڲ�����ת��
	XMMATRIX W = m_Transform.GetLocalToWorldMatrixXM();
	cbDrawing.world = XMMatrixTranspose(W);
	cbDrawing.worldInvTranspose = XMMatrixInverse(nullptr, W);	// ����ת�õ���

	// ���³���������
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(deviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBChangesEveryDrawing), &cbDrawing, sizeof(CBChangesEveryDrawing));
	deviceContext->Unmap(cBuffer.Get(), 0);

	// ��������
	//deviceContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
	deviceContext->PSSetShaderResources(1, 1, m_pTexture.GetAddressOf());
	// ���Կ�ʼ����
	deviceContext->DrawIndexed(m_IndexCount, 0, 0);
}

void Skybox::SetDebugObjectName(const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	D3D11SetDebugObjectName(m_pVertexBuffer.Get(), name + ".VertexBuffer");
	D3D11SetDebugObjectName(m_pIndexBuffer.Get(), name + ".IndexBuffer");
#else
	UNREFERENCED_PARAMETER(name);
#endif
}