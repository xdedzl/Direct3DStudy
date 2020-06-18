#include "DirectXMath.h"
#include <string>
#include "LightHelper.h"
#include "Geometry.h"
#include "Transform.h"
#include <wrl/client.h>
using namespace DirectX;

// һ��������С����Ϸ������
class GameObject
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	GameObject();

	// ��ȡ����任
	Transform& GetTransform();
	// ��ȡ����任
	const Transform& GetTransform() const;

	// ���û�����
	template<class VertexType, class IndexType>
	void SetBuffer(ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData);
	// ��������
	void SetTexture(ID3D11ShaderResourceView* texture);

	// ����
	void Draw(ID3D11DeviceContext* deviceContext);

	// ���õ��Զ�����
	// �����������������ã����Զ�����Ҳ��Ҫ����������
	void SetDebugObjectName(const std::string& name);
private:
	Transform m_Transform;								// ����任��Ϣ
	ComPtr<ID3D11ShaderResourceView> m_pTexture;		// ����
	ComPtr<ID3D11Buffer> m_pVertexBuffer;				// ���㻺����
	ComPtr<ID3D11Buffer> m_pIndexBuffer;				// ����������
	UINT m_VertexStride;								// �����ֽڴ�С
	UINT m_IndexCount;								    // ������Ŀ	
};