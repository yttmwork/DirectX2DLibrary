﻿#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <D3dx9math.h>
#include "../Useful/Vec.h"
#include "Window.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体
struct DeviceEnumParameter
{
	LPDIRECTINPUT8 InputInterface;
	LPDIRECTINPUTDEVICE8* GamePadDevice;
	int FindCount;
};

static LPDIRECTINPUTDEVICE8 g_GamePadDevice;						// ゲームパッド用Deviceのポインタ
static ButtonState g_GamePadStates[GamePadKind::GamePadKindMax];	// ゲームパッドの入力状態

// 協調モード設定
BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device);

// 入力受付開始
BOOL StartGamePadControl();

// プロパティ設定
BOOL SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device);

// デバイス探索結果受信用
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

bool InitGamePad(LPDIRECTINPUT8 input_interface)
{
	DeviceEnumParameter parameter;

	parameter.FindCount = 0;
	parameter.GamePadDevice = &g_GamePadDevice;
	parameter.InputInterface = input_interface;

	// GAMEPADを調べる
	input_interface->EnumDevices(
		DI8DEVTYPE_GAMEPAD,			// 検索するデバイスの種類
		DeviceFindCallBack,			// 発見時に実行する関数
		&parameter,					// 関数に渡す値
		DIEDFL_ATTACHEDONLY			// 検索方法
	);

	// JOYSTICKを調べる
	input_interface->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	);

	// どちらも見つけることが出来なかったら失敗
	if (parameter.FindCount == 0)
	{
		return false;
	}

	int count = 0;
	// 制御開始
	while (StartGamePadControl() == false)
	{
		Sleep(10);
		count++;
		if (count >= 5)
		{
			break;
		}
	}

	// 入力情報の初期化
	for (int i = 0; i < GamePadKind::GamePadKindMax; i++)
	{
		g_GamePadStates[i] = ButtonState::ButtonStateNone;
	}

	return true;
}

void ReleaseGamePad()
{
	// デバイスの解放
	if (g_GamePadDevice != nullptr)
	{
		// 制御を停止
		g_GamePadDevice->Unacquire();
		g_GamePadDevice->Release();
		g_GamePadDevice = nullptr;
	}
}

bool IsHeldGamePad(GamePadKind button)
{
	if (g_GamePadStates[button] == ButtonState::ButtonStateHeld)
	{
		return true;
	}

	return false;
}

bool IsPushedGamePad(GamePadKind button)
{
	if (g_GamePadStates[button] == ButtonState::ButtonStatePushed)
	{
		return true;
	}

	return false;
}

bool IsReleasedGamePad(GamePadKind button)
{
	if (g_GamePadStates[button] == ButtonState::ButtonStateReleased)
	{
		return true;
	}

	return false;
}

void UpdateGamePad()
{
	if (g_GamePadDevice == nullptr)
	{
		return;
	}

	DIJOYSTATE pad_data;

	// デバイス取得
	HRESULT hr = g_GamePadDevice->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
	if (FAILED(hr))
	{
		// 再度制御開始
		if (FAILED(g_GamePadDevice->Acquire()))
		{
			for (int i = 0; i < GamePadKind::GamePadKindMax; i++)
			{
				g_GamePadStates[i] = ButtonState::ButtonStateNone;
			}
			g_GamePadDevice->Poll();
		}
		return;
	}

	bool is_push[GamePadKind::GamePadKindMax];
	ZeroMemory(is_push, sizeof(bool) * GamePadKind::GamePadKindMax);

	// スティック判定
	int unresponsive_range = 200;
	if (pad_data.lX < -unresponsive_range)
	{
		is_push[ButtonKind::LeftButton] = true;
	}
	else if (pad_data.lX > unresponsive_range)
	{
		is_push[ButtonKind::RightButton] = true;
	}

	if (pad_data.lY < -unresponsive_range)
	{
		is_push[ButtonKind::UpButton] = true;
	}
	else if (pad_data.lY > unresponsive_range)
	{
		is_push[ButtonKind::DownButton] = true;
	}

	// 十字キー判定
	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		float rad = D3DXToRadian((pad_data.rgdwPOV[0] / 100.0f));
		float x = sinf(rad);
		float y = cosf(rad);

		if (x < -0.01f)
		{
			is_push[ButtonKind::LeftButton] = true;
		}
		else if (x > 0.01f)
		{
			is_push[ButtonKind::RightButton] = true;
		}

		if (y > 0.01f)
		{
			is_push[ButtonKind::UpButton] = true;
		}
		else if (y < -0.01f)
		{
			is_push[ButtonKind::DownButton] = true;
		}
	}

	int base_index = ButtonKind::Button01;
	const int ButtonTrg = 0x80;
	// ボタン判定
	for (int i = 0; i < ButtonKind::ButtonKindMax - base_index; i++)
	{
		if (!(pad_data.rgbButtons[i] & ButtonTrg))
		{
			continue;
		}

		is_push[base_index + i] = true;
	}

	// 入力情報からボタンの状態を更新する
	for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
	{
		g_GamePadStates[i] = UpdateButtonState(is_push[i], g_GamePadStates[i]);
	}
}

BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device)
{
	//協調モードの設定
	if (FAILED(device->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr), 
		DISCL_EXCLUSIVE | DISCL_FOREGROUND)
	))
	{
		return false;
	}

	return true;
}

BOOL StartGamePadControl()
{
	// デバイスが生成されてない
	if (g_GamePadDevice == nullptr)
	{
		return false;
	}
	
	// 制御開始
	if (FAILED(g_GamePadDevice->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	g_GamePadDevice->GetCapabilities(&cap);
	// ポーリング判定
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// ポーリング開始
		/*
			PollはAcquireの前に行うとされていたが、
			Acquireの前で実行すると失敗したので
			後で実行するようにした
		*/
		if (FAILED(g_GamePadDevice->Poll()))
		{
			return false;
		}
	}

	return true;
}

BOOL SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device)
{
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	return true;
}

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef;
	LPDIRECTINPUTDEVICE8 device = nullptr;

	// 既に発見しているなら終了
	if (parameter->FindCount >= 1)
	{
		return DIENUM_STOP;
	}

	// デバイス生成
	HRESULT hr = parameter->InputInterface->CreateDevice(
		lpddi->guidInstance,
		parameter->GamePadDevice,
		nullptr);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// 入力フォーマットの指定
	device = *parameter->GamePadDevice;
	hr = device->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// プロパティの設定
	if (SetUpGamePadProperty(device) == false)
	{
		return DIENUM_STOP;
	}

	//協調モードの設定
	if (FAILED(device->SetCooperativeLevel(
			FindWindow(WINDOW_CLASS_NAME, nullptr),
			DISCL_EXCLUSIVE | DISCL_FOREGROUND)
		))
	{
		return false;
	}

	// 発見数をカウント
	parameter->FindCount++;

	return DIENUM_CONTINUE;
}
