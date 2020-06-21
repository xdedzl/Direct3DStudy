#include "Vehicle.h"
#include "DXTrace.h"
#include "DDSTextureLoader.h"
#include "GameObject.h"
#include "Input.h"

using namespace DirectX;

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
void Vehicle::Awake(ID3D11Device* d3dDevice)
{
	ComPtr<ID3D11ShaderResourceView> texture;
	(CreateDDSTextureFromFile(d3dDevice, L"Texture\\WoodCrate.dds", nullptr, texture.GetAddressOf()));
	main.SetBuffer(d3dDevice, Geometry::CreateBox());
	main.SetTexture(texture.Get());
	main.GetTransform()->SetLocalPosition(-1.0f, 0.5f, 0.0f);

	for (size_t i = 0; i < 4; i++)
	{
		ComPtr<ID3D11ShaderResourceView> texture;
		(CreateDDSTextureFromFile(d3dDevice, L"Texture\\WoodCrate.dds", nullptr, texture.GetAddressOf()));
		wheels[i].SetBuffer(d3dDevice, Geometry::CreateSphere(0.5f));
		wheels[i].SetTexture(texture.Get());
		wheels[i].GetTransform()->SetParent(main.GetTransform());
	}

	wheels[0].GetTransform()->SetLocalPosition(-1.0f, -1.0f, 1.0f);
	wheels[1].GetTransform()->SetLocalPosition(1.0f, -1.0f, 1.0f);
	wheels[2].GetTransform()->SetLocalPosition(1.0f, -1.0f, -1.0f);
	wheels[3].GetTransform()->SetLocalPosition(-1.0f, -1.0f, -1.0f);
}

void Vehicle::OnDraw(ID3D11DeviceContext* d3dDeviceContext)
{
	main.Draw(d3dDeviceContext);

	for (size_t i = 0; i < 4; i++)
	{
		wheels[i].Draw(d3dDeviceContext);
	}
}

void Vehicle::OnUpdate(float dt)
{
	/*if (Input::Instance()->IsKeyDown(Keyboard::Up))
	{
		auto pos = main.GetTransform().GetPosition();
		pos.z += dt * 10;
		main.GetTransform().SetPosition(pos);
	}

	if (Input::Instance()->IsKeyDown(Keyboard::Down))
	{
		auto pos = main.GetTransform().GetPosition();
		pos.z -= dt * 10;
		main.GetTransform().SetPosition(pos);
	}*/

	float speed = 0;
	float rotateSpeed = 0;

	if (Input::Instance()->IsKeyDown(Keyboard::Up))
	{
		speed += dt * 10;
	}

	if (Input::Instance()->IsKeyDown(Keyboard::Down))
	{
		speed -= dt * 10;
	}

	if (Input::Instance()->IsKeyDown(Keyboard::Left))
	{
		rotateSpeed -= dt * 5;
	}

	if (Input::Instance()->IsKeyDown(Keyboard::Right))
	{
		rotateSpeed += dt * 5;
	}

	auto transform = main.GetTransform();

	transform->Translate(transform->GetForwardAxis(), speed);

	XMFLOAT3 rotate = XMFLOAT3(0, rotateSpeed, 0);

	transform->Rotate(rotate);
}