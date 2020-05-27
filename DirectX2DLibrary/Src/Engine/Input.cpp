#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <D3dx9math.h>
#include "Window.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

static LPDIRECTINPUT8 g_InputInterface;	// DIRECTINPUT8のポインタ

// 入力インターフェースの作成
bool CreateInputInterface();

bool InitInput()
{
	// インターフェース作成
	if (CreateInputInterface() == false)
	{
		return false;
	}

	// キーボード初期化
	if (InitKeyboard(g_InputInterface) == false)
	{
		ReleaseInput();
		return false;
	}

	// マウス初期化
	if (InitMouse(g_InputInterface) == false)
	{
		ReleaseInput();
		return false;
	}

	// ゲームパッド初期化
	// ※接続されていないこともあるので、失敗の判定はしていない
	InitGamePad(g_InputInterface);

	return true;
}

void ReleaseInput()
{
	ReleaseKeyboard();
	ReleaseMouse();
	ReleaseGamePad();

	// インターフェースの解放
	if (g_InputInterface != nullptr)
	{
		g_InputInterface->Release();
		g_InputInterface = nullptr;
	}
}

void UpdateInput()
{
	UpdateKeyboard();
	UpdateMouse();
	UpdateGamePad();
}

bool CreateInputInterface()
{
	// インターフェース作成
	HRESULT result = DirectInput8Create(
		GetModuleHandle(nullptr),	// インスタンスハンドル
		DIRECTINPUT_VERSION,		// DirectInputのバージョン
		IID_IDirectInput8,			// 使用する機能
		(void**)&g_InputInterface,	// 作成されたインターフェース代入用
		nullptr						// NULL固定
	);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

ButtonState UpdateButtonState(bool is_push, ButtonState state)
{
	if (is_push == true)
	{
		if (state == ButtonState::ButtonStateNone)
		{
			return ButtonState::ButtonStateDown;
		}
		else
		{
			return ButtonState::ButtonStatePush;
		}
	}
	else
	{
		if (state == ButtonState::ButtonStatePush)
		{
			return ButtonState::ButtonStateUp;
		}
		else
		{
			return ButtonState::ButtonStateNone;
		}
	}

	return ButtonState::ButtonStateNone;
}
