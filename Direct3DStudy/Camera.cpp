#include "Camera.h"
#include "Input.h"
#include "XMath.h"
using namespace DirectX;

Camera::~Camera()
{
}

XMVECTOR Camera::GetPositionXM() const
{
	return XMLoadFloat3(&m_Transform.GetPosition());
}

XMFLOAT3 Camera::GetPosition() const
{
	return m_Transform.GetPosition();
}

Transform* Camera::GetTransform()
{
	return &m_Transform;
}

float Camera::GetRotationX() const
{
	return m_Transform.GetRotation().x;
}

float Camera::GetRotationY() const
{
	return m_Transform.GetRotation().y;
}


XMVECTOR Camera::GetRightAxisXM() const
{
	return XMLoadFloat3(&m_Transform.GetRightAxis());
}

XMFLOAT3 Camera::GetRightAxis() const
{
	return m_Transform.GetRightAxis();
}

XMVECTOR Camera::GetUpAxisXM() const
{
	return XMLoadFloat3(&m_Transform.GetUpAxis());
}

XMFLOAT3 Camera::GetUpAxis() const
{
	return m_Transform.GetUpAxis();
}

XMVECTOR Camera::GetLookAxisXM() const
{
	return XMLoadFloat3(&m_Transform.GetForwardAxis());
}

XMFLOAT3 Camera::GetLookAxis() const
{
	return m_Transform.GetForwardAxis();
}

XMMATRIX Camera::GetViewXM() const
{
	return m_Transform.GetWorldToLocalMatrixXM();
}

XMMATRIX Camera::GetProjXM() const
{
	return XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
}

XMMATRIX Camera::GetViewProjXM() const
{
	return GetViewXM() * GetProjXM();
}

D3D11_VIEWPORT Camera::GetViewPort() const
{
	return m_ViewPort;
}

void Camera::SetFrustum(float fovY, float aspect, float nearZ, float farZ)
{
	m_FovY = fovY;
	m_Aspect = aspect;
	m_NearZ = nearZ;
	m_FarZ = farZ;
}

void Camera::SetViewPort(const D3D11_VIEWPORT& viewPort)
{
	m_ViewPort = viewPort;
}

void Camera::SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	m_ViewPort.TopLeftX = topLeftX;
	m_ViewPort.TopLeftY = topLeftY;
	m_ViewPort.Width = width;
	m_ViewPort.Height = height;
	m_ViewPort.MinDepth = minDepth;
	m_ViewPort.MaxDepth = maxDepth;
}

// ******************
// 第一人称/自由视角摄像机
//

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::OnUpdate(float dt)
{
	// 第一人称/自由摄像机的操作

		// 方向移动
	if (Input::Instance()->IsKeyDown(Keyboard::W))
	{
		Walk(dt * 6.0f);
		//camera->MoveForward(dt * 6.0f);
	}
	if (Input::Instance()->IsKeyDown(Keyboard::S))
	{
		Walk(dt * -6.0f);
	}
	if (Input::Instance()->IsKeyDown(Keyboard::A))
		Strafe(dt * -6.0f);
	if (Input::Instance()->IsKeyDown(Keyboard::D))
		Strafe(dt * 6.0f);

	// 将摄像机位置限制在[-8.9, 8.9]x[-8.9, 8.9]x[0.0, 8.9]的区域内
	// 不允许穿地
	XMFLOAT3 adjustedPos;
	XMStoreFloat3(&adjustedPos, XMVectorClamp(GetPositionXM(), XMVectorSet(-8.9f, 0.0f, -8.9f, 0.0f), XMVectorReplicate(8.9f)));
	SetPosition(adjustedPos);

	// 在鼠标没进入窗口前仍为ABSOLUTE模式
	/*if (mouseState.positionMode == Mouse::MODE_RELATIVE)
	{
		camera->Pitch(mouseState.y * dt * 2.5f);
		camera->RotateY(mouseState.x * dt * 2.5f);
	}*/

	//camera->Pitch(mouseState.y * dt * 2.5f);
	//camera->RotateY(mouseState.x * dt * 2.5f * (-0.0001f));
}

void FirstPersonCamera::SetPosition(float x, float y, float z)
{
	SetPosition(XMFLOAT3(x, y, z));
}

void FirstPersonCamera::SetPosition(const XMFLOAT3& pos)
{
	m_Transform.SetLocalPosition(pos);
}

void FirstPersonCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	m_Transform.SetLocalPosition(pos);
	m_Transform.LookAt(target, up);
}

void FirstPersonCamera::LookTo(const XMFLOAT3& pos, const XMFLOAT3& to, const XMFLOAT3& up)
{
	m_Transform.SetLocalPosition(pos);
	m_Transform.LookTo(to, up);
}

void FirstPersonCamera::Strafe(float d)
{
	m_Transform.Translate(m_Transform.GetRightAxis(), d);
}

void FirstPersonCamera::Walk(float d)
{
	XMVECTOR rightVec = XMLoadFloat3(&m_Transform.GetRightAxis());
	XMVECTOR frontVec = XMVector3Normalize(XMVector3Cross(rightVec, g_XMIdentityR1));
	XMFLOAT3 front;
	XMStoreFloat3(&front, frontVec);
	m_Transform.Translate(front, d);
}

void FirstPersonCamera::MoveForward(float d)
{
	m_Transform.Translate(m_Transform.GetForwardAxis(), d);
}

void FirstPersonCamera::Pitch(float rad)
{
	XMFLOAT3 rotation = m_Transform.GetRotation();
	// 将绕x轴旋转弧度限制在[-7pi/18, 7pi/18]之间
	rotation.x += rad;
	if (rotation.x > XM_PI * 7 / 18)
		rotation.x = XM_PI * 7 / 18;
	else if (rotation.x < -XM_PI * 7 / 18)
		rotation.x = -XM_PI * 7 / 18;

	m_Transform.SetLocalRotation(rotation);
}

void FirstPersonCamera::RotateY(float rad)
{
	XMFLOAT3 rotation = m_Transform.GetRotation();
	rotation.y = XMScalarModAngle(rotation.y + rad);
	m_Transform.SetLocalRotation(rotation);
}

void FollowCamera::SetTarget(Transform* target)
{
	m_Target = target;
}

void FollowCamera::OnUpdate(float dt)
{
	auto targetPos = m_Target->GetPosition();
	auto forward = m_Target->GetForwardAxis();
	
	XMVECTOR directionVec = XMVector3Normalize(XMLoadFloat3(&m_Target->GetForwardAxis()));
	XMVECTOR newPosition = XMVectorMultiplyAdd(XMVectorReplicate(6), -directionVec, XMLoadFloat3(&targetPos));

	XMFLOAT3 pos;
	XMStoreFloat3(&pos, newPosition);
	pos.y = targetPos.y + 2;

	pos = Lerp(m_Transform.GetPosition(), pos, dt * 5); // 插值

	m_Transform.SetLocalPosition(pos);
	m_Transform.LookAt(targetPos);
}