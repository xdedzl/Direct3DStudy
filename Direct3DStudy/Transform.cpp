#include "Transform.h"

using namespace DirectX;

Transform::Transform(const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& position)
	: m_Scale(scale), m_Rotation(rotation), m_Position(position)
{
	m_Parnet = nullptr;
	m_Childs = list<Transform*>();
}

XMFLOAT3 Transform::GetScale() const
{
	return m_Scale;
}

XMFLOAT3 Transform::GetRotation() const
{
	if (m_Parnet != nullptr)
	{
		auto parentRot = m_Parnet->GetRotation();
		return XMFLOAT3(m_Rotation.x + parentRot.x, m_Rotation.y + parentRot.y, m_Rotation.z + parentRot.z);
	}
	return m_Rotation;
}

XMFLOAT3 Transform::GetLocalRotation() const
{
	return m_Rotation;
}

XMFLOAT3 Transform::GetPosition() const
{
	if (m_Parnet != nullptr)
	{
		auto parentPos = m_Parnet->GetPosition();
		auto parentForward = m_Parnet->GetForwardAxis();

		return XMFLOAT3(m_Position.x + parentPos.x, m_Position.y + parentPos.y, m_Position.z + parentPos.z);
	}
	return m_Position;
}

XMFLOAT3 Transform::GetLocalPosition() const
{
	return m_Position;
}

XMFLOAT3 Transform::GetRightAxis() const
{
	XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_Rotation));
	XMFLOAT3 right;
	XMStoreFloat3(&right, R.r[0]);
	return right;
}

XMFLOAT3 Transform::GetUpAxis() const
{
	XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_Rotation));
	XMFLOAT3 up;
	XMStoreFloat3(&up, R.r[1]);
	return up;
}

XMFLOAT3 Transform::GetForwardAxis() const
{
	XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_Rotation));
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, R.r[2]);
	return forward;
}

XMFLOAT4X4 Transform::GetLocalToWorldMatrix() const
{
	XMFLOAT4X4 res;
	XMStoreFloat4x4(&res, GetLocalToWorldMatrixXM());
	return res;
}

XMMATRIX Transform::GetLocalToWorldMatrixXM() const
{
	XMVECTOR scaleVec = XMLoadFloat3(&m_Scale);
	XMVECTOR rotationVec = XMLoadFloat3(&m_Rotation);
	XMVECTOR positionVec = XMLoadFloat3(&m_Position);
	XMMATRIX World = XMMatrixScalingFromVector(scaleVec) * XMMatrixRotationRollPitchYawFromVector(rotationVec) * XMMatrixTranslationFromVector(positionVec);

	if (m_Parnet != nullptr)
	{
		auto parentMat = m_Parnet->GetLocalToWorldMatrixXM();
		return World * parentMat;
	}

	return World;
}

XMFLOAT4X4 Transform::GetWorldToLocalMatrix() const
{
	XMFLOAT4X4 res;
	XMStoreFloat4x4(&res, GetWorldToLocalMatrixXM());
	return res;
}

XMMATRIX Transform::GetWorldToLocalMatrixXM() const
{
	XMMATRIX InvWorld = XMMatrixInverse(nullptr, GetLocalToWorldMatrixXM());
	return InvWorld;
}

void Transform::SetScale(const XMFLOAT3& scale)
{
	m_Scale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	m_Scale = XMFLOAT3(x, y, z);
}

void Transform::SetRotation(const XMFLOAT3& eulerAnglesInRadian)
{
	m_Rotation = eulerAnglesInRadian;
}

void Transform::SetRotation(float x, float y, float z)
{
	m_Rotation = XMFLOAT3(x, y, z);
}

void Transform::SetLocalPosition(const XMFLOAT3& position)
{
	m_Position = position;
}

void Transform::SetLocalPosition(float x, float y, float z)
{
	m_Position = XMFLOAT3(x, y, z);
}

void Transform::Rotate(const XMFLOAT3& eulerAnglesInRadian)
{
	XMVECTOR newRotationVec = XMVectorAdd(XMLoadFloat3(&m_Rotation), XMLoadFloat3(&eulerAnglesInRadian));
	XMStoreFloat3(&m_Rotation, newRotationVec);
}

void Transform::RotateAxis(const XMFLOAT3& axis, float radian)
{
	XMVECTOR rotationVec = XMLoadFloat3(&m_Rotation);
	XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(rotationVec) *
		XMMatrixRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(radian));
	XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, R);
	m_Rotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}

void Transform::RotateAround(const XMFLOAT3& point, const XMFLOAT3& axis, float radian)
{
	XMVECTOR rotationVec = XMLoadFloat3(&m_Rotation);
	XMVECTOR positionVec = XMLoadFloat3(&m_Position);
	XMVECTOR centerVec = XMLoadFloat3(&point);

	// 以point作为原点进行旋转
	XMMATRIX RT = XMMatrixRotationRollPitchYawFromVector(rotationVec) * XMMatrixTranslationFromVector(positionVec - centerVec);
	RT *= XMMatrixRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(radian));
	RT *= XMMatrixTranslationFromVector(centerVec);
	XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, RT);
}

void Transform::Translate(const XMFLOAT3& direction, float magnitude)
{
	XMVECTOR directionVec = XMVector3Normalize(XMLoadFloat3(&direction));
	XMVECTOR newPosition = XMVectorMultiplyAdd(XMVectorReplicate(magnitude), directionVec, XMLoadFloat3(&m_Position));
	XMStoreFloat3(&m_Position, newPosition);
}

void Transform::LookAt(const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMMATRIX View = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMMATRIX InvView = XMMatrixInverse(nullptr, View);
	XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, InvView);
	m_Rotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}

void Transform::LookTo(const XMFLOAT3& direction, const XMFLOAT3& up)
{
	XMMATRIX View = XMMatrixLookToLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&direction), XMLoadFloat3(&up));
	XMMATRIX InvView = XMMatrixInverse(nullptr, View);
	XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, InvView);
	m_Rotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}

XMFLOAT3 Transform::GetEulerAnglesFromRotationMatrix(const XMFLOAT4X4& rotationMatrix)
{
	// 通过旋转矩阵反求欧拉角
	float c = sqrtf(1.0f - rotationMatrix(2, 1) * rotationMatrix(2, 1));
	// 防止r[2][1]出现大于1的情况
	if (isnan(c))
		c = 0.0f;
	XMFLOAT3 rotation;
	rotation.z = atan2f(rotationMatrix(0, 1), rotationMatrix(1, 1));
	rotation.x = atan2f(-rotationMatrix(2, 1), c);
	rotation.y = atan2f(rotationMatrix(2, 0), rotationMatrix(2, 2));
	return rotation;
}

Transform* Transform::GetParent()
{
	return m_Parnet;
}

void Transform::SetParent(Transform* parent)
{
	parent->AddChild(this);
}

void Transform::AddChild(Transform* child)
{
	child->m_Parnet = this;
	m_Childs.push_back(child);
}