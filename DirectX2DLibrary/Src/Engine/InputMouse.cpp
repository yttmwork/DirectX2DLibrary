#include <Windows.h>
#include <stdio.h>
#include "Window.h"
#include "Input.h"
#include "InputMouse.h"

static LPDIRECTINPUTDEVICE8 g_MouseDevice;	// Mouse用Deviceのポインタ
static DIMOUSESTATE g_CurrentMouseState;	// マウスの現在の入力情報
static DIMOUSESTATE g_PrevMouseState;		// マウスの現在の入力情報
static Vec2 g_MousePos;						// マウス座標

bool InitMouse(LPDIRECTINPUT8 input_interface)
{
	if (g_MouseDevice != nullptr)
	{
		g_MouseDevice->Release();
		g_MouseDevice = nullptr;
	}

	// デバイス生成
	HRESULT hr = input_interface->CreateDevice(
		GUID_SysMouse,
		&g_MouseDevice,
		nullptr);

	if (FAILED(hr))
	{
		return false;
	}

	// 入力フォーマットの指定
	hr = g_MouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}

	// 協調モードの設定
	hr = g_MouseDevice->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr),
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
	{
		return false;
	}

	g_MouseDevice->Acquire();

	return true;
}

void ReleaseMouse()
{
	if (g_MouseDevice != nullptr)
	{
		// 制御を停止
		g_MouseDevice->Unacquire();
		g_MouseDevice->Release();
		g_MouseDevice = nullptr;
	}
}

void UpdateMouse()
{
	if (g_MouseDevice == nullptr)
	{
		return;
	}

	// 更新前に最新マウス情報を保存する
	g_PrevMouseState = g_CurrentMouseState;

	// マウスの状態を取得します
	HRESULT	hr = g_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);
	if (FAILED(hr))
	{
		g_MouseDevice->Acquire();
		hr = g_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);
	}

	Vec2 prev = g_MousePos;
	POINT p;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&p);

	// スクリーン座標にクライアント座標に変換する
	ScreenToClient(FindWindowA(WINDOW_CLASS_NAME, nullptr), &p);

	// 変換した座標を保存
	g_MousePos.X = (float)p.x;
	g_MousePos.Y = (float)p.y;
}

bool GetMouseDown(MouseButton button_type)
{
	const int MouseTrgValue = 0x80;

	if (!(g_PrevMouseState.rgbButtons[button_type] & MouseTrgValue) &&
		g_CurrentMouseState.rgbButtons[button_type] & MouseTrgValue)
	{
		return true;
	}

	return false;
}

bool GetMouse(MouseButton button_type)
{
	const int MouseTrgValue = 0x80;
	if (g_PrevMouseState.rgbButtons[button_type] & MouseTrgValue &&
		g_CurrentMouseState.rgbButtons[button_type] & MouseTrgValue)
	{
		return true;
	}

	return false;
}

bool GetMouseUp(MouseButton button_type)
{
	const int MouseTrgValue = 0x80;

	if (g_PrevMouseState.rgbButtons[button_type] & MouseTrgValue &&
		!(g_CurrentMouseState.rgbButtons[button_type] & MouseTrgValue))
	{
		return true;
	}

	return false;
}
