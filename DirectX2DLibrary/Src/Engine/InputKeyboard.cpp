#include <Windows.h>
#include <math.h>
#include <D3dx9math.h>
#include "../Utility/Vec.h"
#include "Window.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

const int MaxKeyNum = 256;					// キー最大数

LPDIRECTINPUTDEVICE8 g_KeyboardDevice;		// Keyboard用Deviceのポインタ
ButtonState g_KeyboardState[MaxKeyNum];		// キーボード入力状態

bool InitKeyboard(LPDIRECTINPUT8 input_interface)
{
	// IDirectInputDevice8インターフェイスの取得
	HRESULT hr = input_interface->CreateDevice(GUID_SysKeyboard, &g_KeyboardDevice, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	// デバイスのフォーマットの設定
	hr = g_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}

	// 協調モードの設定
	hr = g_KeyboardDevice->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr),
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(hr))
	{
		return false;
	}

	// デバイスの取得開始
	g_KeyboardDevice->Acquire();

	return true;
}

void ReleaseKeyboard()
{
	if (g_KeyboardDevice != nullptr)
	{
		// 制御を停止
		g_KeyboardDevice->Unacquire();
		g_KeyboardDevice->Release();
		g_KeyboardDevice = nullptr;
	}
}

void UpdateKeyboard()
{
	// キー情報取格納用
	BYTE key_states[MaxKeyNum];
	HRESULT hr;
	const int KeyTrg = 0x80;

	if (g_KeyboardDevice == nullptr)
	{
		return;
	}

	// キーボードデバイスのゲッター
	hr = g_KeyboardDevice->GetDeviceState(MaxKeyNum, key_states);
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < MaxKeyNum; i++)
		{
			bool is_push = false;
			if (key_states[i] & KeyTrg)
			{
				is_push = true;
			}
			
			g_KeyboardState[i] = UpdateButtonState(is_push, g_KeyboardState[i]);
		}
	}
	else if (hr == DIERR_INPUTLOST)
	{
		g_KeyboardDevice->Acquire();
	}
}

bool IsHeldKeyboard(UINT key_code)
{
	if (g_KeyboardState[key_code] == ButtonState::ButtonStateHeld)
	{
		return true;
	}

	return false;
}

bool IsPushedKeyboard(UINT key_code)
{
	if (g_KeyboardState[key_code] == ButtonState::ButtonStatePushed)
	{
		return true;
	}

	return false;
}

bool IsReleasedKeyboard(UINT key_code)
{
	if (g_KeyboardState[key_code] == ButtonState::ButtonStateReleased)
	{
		return true;
	}

	return false;
}
