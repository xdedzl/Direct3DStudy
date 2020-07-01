#pragma once
#include <DirectXMath.h>
#include <list>
using namespace std;
using namespace DirectX;

class Transform
{
public:
	Transform() = default;
	Transform(const XMFLOAT3& scale, const XMFLOAT3& rotation, const XMFLOAT3& position);
	~Transform() = default;

	Transform(const Transform&) = default;
	Transform& operator=(const Transform&) = default;

	Transform(Transform&&) = default;
	Transform& operator=(Transform&&) = default;

	// 获取对象缩放比例
	XMFLOAT3 GetScale() const;
	// 获取对象欧拉角(弧度制)
	// 对象以Z-X-Y轴顺序旋转
	XMFLOAT3 GetRotation() const;
	XMFLOAT3 GetLocalRotation() const;
	// 获取对象位置
	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetLocalPosition() const;

	// 获取右方向轴
	XMFLOAT3 GetRightAxis() const;
	// 获取上方向轴
	XMFLOAT3 GetUpAxis() const;
	// 获取前方向轴
	XMFLOAT3 GetForwardAxis() const;

	// 获取世界变换矩阵
	XMFLOAT4X4 GetLocalToWorldMatrix() const;
	// 获取世界变换矩阵
	XMMATRIX GetLocalToWorldMatrixXM() const;

	// 获取逆世界变换矩阵
	XMFLOAT4X4 GetWorldToLocalMatrix() const;
	// 获取逆世界变换矩阵
	XMMATRIX GetWorldToLocalMatrixXM() const;

	// 设置对象缩放比例
	void SetScale(const XMFLOAT3& scale);
	// 设置对象缩放比例
	void SetScale(float x, float y, float z);

	// 设置对象欧拉角(弧度制)
	// 对象将以Z-X-Y轴顺序旋转
	void SetLocalRotation(const XMFLOAT3& eulerAnglesInRadian);
	// 设置对象欧拉角(弧度制)
	// 对象将以Z-X-Y轴顺序旋转
	void SetLocalRotation(float x, float y, float z);

	// 设置对象位置
	void SetLocalPosition(const XMFLOAT3& position);
	// 设置对象位置
	void SetLocalPosition(float x, float y, float z);

	// 指定欧拉角旋转对象
	void Rotate(const XMFLOAT3& eulerAnglesInRadian);
	// 指定以原点为中心绕轴旋转
	void RotateAxis(const XMFLOAT3& axis, float radian);
	// 指定以point为旋转中心绕轴旋转
	void RotateAround(const XMFLOAT3& point, const XMFLOAT3& axis, float radian);

	// 沿着某一方向平移
	void Translate(const XMFLOAT3& direction, float magnitude);

	// 观察某一点
	void LookAt(const XMFLOAT3& target, const XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });
	// 沿着某一方向观察
	void LookTo(const XMFLOAT3& direction, const XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });

	Transform* GetParent();
	void SetParent(Transform* parent);
	void AddChild(Transform* child);

	// 父物体
	__declspec(property(get = GetParent, put = SetParent)) Transform* parent;
	// 自身位置
	__declspec(property(get = GetLocalPosition, put = SetLocalPosition)) XMFLOAT3 localPosition;
	// 位置
	__declspec(property(get = GetPosition)) XMFLOAT3 position;
	// 自身角度
	__declspec(property(get = GetLocalRotation, put = SetLocalRotation)) XMFLOAT3 localEulerAngles;

	inline XMFLOAT3 GetLocalRotation2() const { return localEulerAngles; }
	inline void SetLocalRotation2(const XMFLOAT3& newValue) { localEulerAngles = newValue; }
	inline void SetLocalRotation2(const XMFLOAT3&& newValue) { localEulerAngles = newValue; }
	__declspec(property(get = GetRotation)) XMFLOAT3 eulerAngles;

private:
	// 从旋转矩阵获取旋转欧拉角
	XMFLOAT3 GetEulerAnglesFromRotationMatrix(const XMFLOAT4X4& rotationMatrix);

private:
	XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };				// 缩放
	XMFLOAT3 m_Rotation = {};								// 旋转欧拉角(弧度制)
	XMFLOAT3 m_Position = {};								// 位置
	XMFLOAT3 m_LoaclPosition = {};
	Transform* m_Parnet;
	list<Transform*> m_Childs;
};