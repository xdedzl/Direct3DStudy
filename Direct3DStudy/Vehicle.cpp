#include "Vehicle.h"
#include "DXTrace.h"
#include "DDSTextureLoader.h"
#include "GameObject.h"
#include "Input.h"
#include "XMath.h"

using namespace DirectX;

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
void Vehicle::Awake(ID3D11Device* d3dDevice)
{
	ComPtr<ID3D11ShaderResourceView> texture;
	(CreateDDSTextureFromFile(d3dDevice, L"Texture\\WoodCrate.dds", nullptr, texture.GetAddressOf()));
	main.SetBuffer(d3dDevice, Geometry::CreateBox(2,2,4));
	main.SetTexture(texture.Get());
	main.transform->SetLocalPosition(-1.0f, 0.5f, 0.0f);

	for (size_t i = 0; i < 4; i++)
	{
		ComPtr<ID3D11ShaderResourceView> texture;
		(CreateDDSTextureFromFile(d3dDevice, L"Texture\\WoodCrate.dds", nullptr, texture.GetAddressOf()));
		wheels[i].SetBuffer(d3dDevice, Geometry::CreateCylinder(0.5f,0.2f));
		wheels[i].SetTexture(texture.Get());
		wheels[i].transform->SetParent(main.GetTransform());

		XMFLOAT3 rotate = XMFLOAT3(0, 0, 90.0f * Deg2Rad);
		wheels[i].transform->SetRotation(rotate);
	}

	wheels[0].transform->SetLocalPosition(-1.0f, -1.0f, 2.0f);
	wheels[1].transform->SetLocalPosition(1.0f, -1.0f, 2.0f);
	wheels[2].transform->SetLocalPosition(1.0f, -1.0f, -2.0f);
	wheels[3].transform->SetLocalPosition(-1.0f, -1.0f, -2.0f);
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
	float speed = 0;
	float rotateSpeed = 0;

	if (Input::Instance()->IsKeyDown(Keyboard::Up)|| Input::Instance()->IsKeyDown(Keyboard::W))
	{
		speed += dt * 10;
	}

	if (Input::Instance()->IsKeyDown(Keyboard::Down) || Input::Instance()->IsKeyDown(Keyboard::S))
	{
		speed -= dt * 10;
	}

	if (Input::Instance()->IsKeyDown(Keyboard::Left) || Input::Instance()->IsKeyDown(Keyboard::A))
	{
		rotateSpeed -= dt * 5;
	}

	if (Input::Instance()->IsKeyDown(Keyboard::Right) || Input::Instance()->IsKeyDown(Keyboard::D))
	{
		rotateSpeed += dt * 5;
	}

	auto transform = main.GetTransform();

	transform->Translate(transform->GetForwardAxis(), speed);

	if (speed != 0)
	{
		int sign = speed / abs(speed);
		XMFLOAT3 mainRotate = XMFLOAT3(0, sign * rotateSpeed, 0);
		transform->Rotate(mainRotate);

		XMFLOAT3 wheelRoatate = XMFLOAT3(speed, 0, 0);
		for (size_t i = 0; i < 4; i++)
		{
			wheels[i].transform->Rotate(wheelRoatate);
		}
	}
	float target = 0;
	if (rotateSpeed != 0)
	{
		int sign = rotateSpeed / abs(rotateSpeed);
		target = sign * 45 * Deg2Rad;
	}
	for (size_t i = 0; i < 2; i++)
	{
		XMFLOAT3 rotate = wheels[i].transform->GetLocalRotation();
		rotate = XMFLOAT3(rotate.x, Lerp(rotate.y, target, dt * 10), rotate.z);
		wheels[i].transform->SetRotation(rotate);
	}
}