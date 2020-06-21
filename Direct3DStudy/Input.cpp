#include "Input.h"

Input* Input::m_instance = nullptr;

Input::Input()
{
	m_pMouse = std::make_unique<DirectX::Mouse>();
	m_pKeyboard = std::make_unique<DirectX::Keyboard>();
}

void Input::Init(HWND hwnd)
{
	// 初始化鼠标，键盘不需要
	m_pMouse->SetWindow(hwnd);
	m_pMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
}

void Input::MouseProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pMouse->ProcessMessage(message, wParam, lParam);
}

void Input::KeyboardProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pKeyboard->ProcessMessage(message, wParam, lParam);
}

void Input::OnUpdate()
{
	// 获取鼠标状态
	mouseState = m_pMouse->GetState();
	lastMouseState = m_MouseTracker.GetLastState();
	// 获取键盘状态
	keyState = m_pKeyboard->GetState();
	lastKeyState = m_KeyboardTracker.GetLastState();

	m_MouseTracker.Update(mouseState);
	m_KeyboardTracker.Update(keyState);

	if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.HELD)
	{
		float phi = 0.0f, theta = 0.0f, x = 0.0f, z = 0.0f;
		theta -= (mouseState.x - lastMouseState.x) * 0.01f;
		phi -= (mouseState.y - lastMouseState.y) * 0.01f;
	}

	// 重置滚轮值
	//m_pMouse->ResetScrollWheelValue();
}

bool Input::IsKeyDown(Keyboard::Keys key)
{
	return keyState.IsKeyDown(key);
}

bool Input::IsKeyUp(Keyboard::Keys key)
{
	return keyState.IsKeyUp(key);
}

bool Input::IsKeyPressed(Keyboard::Keys key)
{
	return m_KeyboardTracker.IsKeyPressed(key);
}