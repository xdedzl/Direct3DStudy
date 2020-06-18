#include "DirectXMath.h"
#include <string>
#include "LightHelper.h"
#include "Geometry.h"
using namespace DirectX;

typedef Geometry::MeshData<VertexPosNormalTex, DWORD> MeshData;

class GameObject
{
public:
	XMFLOAT3 position;
	XMFLOAT3 angle;
	XMFLOAT3 sacle;

	Material material;

	MeshData meshData;

	void Update();
	void SetMesh(MeshData meshData);
};