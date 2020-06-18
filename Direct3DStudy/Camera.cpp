#include "Camera.h"
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
// ��һ�˳�/�����ӽ������
//

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::SetPosition(float x, float y, float z)
{
	SetPosition(XMFLOAT3(x, y, z));
}

void FirstPersonCamera::SetPosition(const XMFLOAT3& pos)
{
	m_Transform.SetPosition(pos);
}

void FirstPersonCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	m_Transform.SetPosition(pos);
	m_Transform.LookAt(target, up);
}

void FirstPersonCamera::LookTo(const XMFLOAT3& pos, const XMFLOAT3& to, const XMFLOAT3& up)
{
	m_Transform.SetPosition(pos);
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
	// ����x����ת����������[-7pi/18, 7pi/18]֮��
	rotation.x += rad;
	if (rotation.x > XM_PI * 7 / 18)
		rotation.x = XM_PI * 7 / 18;
	else if (rotation.x < -XM_PI * 7 / 18)
		rotation.x = -XM_PI * 7 / 18;

	m_Transform.SetRotation(rotation);
}

void FirstPersonCamera::RotateY(float rad)
{
	XMFLOAT3 rotation = m_Transform.GetRotation();
	rotation.y = XMScalarModAngle(rotation.y + rad);
	m_Transform.SetRotation(rotation);
}