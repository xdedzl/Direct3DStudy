#pragma once
#include "Mouse.h"
#include "Keyboard.h"

using namespace DirectX;

class Input
{
public:
	Input();
	void Init(HWND hwnd);
	void MouseProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void KeyboardProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void OnUpdate();
	bool IsKeyDown(Keyboard::Keys key);
	bool IsKeyUp(Keyboard::Keys key);
	bool IsKeyPressed(Keyboard::Keys key);
	static Input* Instance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new Input;
		}
		return m_instance;
	}

private:
	static Input* m_instance;
	std::unique_ptr<DirectX::Mouse> m_pMouse;						// 鼠标
	DirectX::Mouse::ButtonStateTracker m_MouseTracker;				// 鼠标状态追踪器
	std::unique_ptr<DirectX::Keyboard> m_pKeyboard;					// 键盘
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;		// 键盘状态追踪器

	// 获取鼠标状态
	Mouse::State mouseState;
	Mouse::State lastMouseState;
	// 获取键盘状态
	Keyboard::State keyState;
	Keyboard::State lastKeyState;
};