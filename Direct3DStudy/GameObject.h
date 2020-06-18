#include "DirectXMath.h"
#include <string>
#include "LightHelper.h"
#include "Geometry.h"
#include "Transform.h"
#include <wrl/client.h>
using namespace DirectX;

// 一个尽可能小的游戏对象类
class GameObject
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	GameObject();

	// 获取物体变换
	Transform& GetTransform();
	// 获取物体变换
	const Transform& GetTransform() const;

	// 设置缓冲区
	template<class VertexType, class IndexType>
	void SetBuffer(ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData);
	// 设置纹理
	void SetTexture(ID3D11ShaderResourceView* texture);

	// 绘制
	void Draw(ID3D11DeviceContext* deviceContext);

	// 设置调试对象名
	// 若缓冲区被重新设置，调试对象名也需要被重新设置
	void SetDebugObjectName(const std::string& name);
private:
	Transform m_Transform;								// 物体变换信息
	ComPtr<ID3D11ShaderResourceView> m_pTexture;		// 纹理
	ComPtr<ID3D11Buffer> m_pVertexBuffer;				// 顶点缓冲区
	ComPtr<ID3D11Buffer> m_pIndexBuffer;				// 索引缓冲区
	UINT m_VertexStride;								// 顶点字节大小
	UINT m_IndexCount;								    // 索引数目	
};