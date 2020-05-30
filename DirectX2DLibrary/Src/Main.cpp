﻿#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Utility/Vec.h"

Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmpLine,
	_In_ INT       nCmdShow)
{
	// エンジンの初期化
	// ゲームループ開始前に1度だけ実行する
	if (InitEngine(640, 480, "Sample") == false)
	{
		return 0;
	}

	// テクスチャ読み込み
	// 第一引数の文字列で読み込んだテクスチャを登録する
	// 描画や取得は登録した文字列で指定する
	LoadTexture("Enemy", "Res/Enemy.png");

	// サウンド読み込み
	// 第一引数の文字列で読み込んだサウンドを登録する
	// 描画や取得は登録した文字列で指定する
	LoadSoundFile("Bgm", "Res/Bgm.wav");
	LoadSoundFile("Se", "Res/Se.wav");

	// サウンド再生
	// 指定されたキーワードのサウンドファイルを再生する
	PlayMusic("Bgm", true);

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// ゲーム処理
			GameProcessing();

			// 描画開始
			DrawProcessing();
		}
	}

	// エンジン終了
	// ゲームループ終了後に1度だけ実行する
	// テクスチャとサウンドファイルも全て解放する
	ReleaseEngine();

	// メモリリーク検出
	_CrtDumpMemoryLeaks();

	return 0;
}
void GameProcessing()
{
	// 入力データの更新
	UpdateInput();
	// 重複サウンドの削除
	EraseDuplicateMusic();

	float speed = 2.0f;

	g_Angle += 1.0f;

	// キーボードの入力取得
	if (IsHeldKeyboard(DIK_LEFT) == true)
	{
		g_Position.X -= speed;
	}
	else if (IsHeldKeyboard(DIK_RIGHT) == true)
	{
		g_Position.X += speed;
	}

	// ゲームパッドの入力取得
	if (IsHeldGamePad(GamePadKind::GamePadKindUp))
	{
		g_Position.Y -= speed;
	}
	else if (IsHeldGamePad(GamePadKind::GamePadKindDown))
	{
		g_Position.Y += speed;
	}

	// マウスの入力取得
	if (IsPushedMouse(MouseButton::Left))
	{
		g_Position = Vec2(200, 200);
	}
	else if (IsPushedMouse(MouseButton::Right))
	{
		g_Position = Vec2(400, 300);
	}

	if (IsReleasedKeyboard(DIK_A))
	{
		// 重複再生
		// 指定されたキーワードのサウンドファイル再生する
		// StartPlayingDuplicateSoundでは同じサウンドファイルでも重複して再生可能
		PlayDuplicateMusic("Se");
		PlayDuplicateMusic("Se");
	}
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	StartDraw(0);

	// テクスチャ描画
	// キーワードで指定されたテクスチャを描画する
	// DrawTextureはテクスチャをそのまま描画する
	// 一部切り取って描画する場合はDrawTextureUVを使用する
	DrawTexture(g_Position.X, g_Position.Y, "Enemy", 128, g_Angle, g_Scale.X, g_Scale.Y);

	// フォント描画
	DrawFont(0.0f, 0.0f, "FontSize:Small", FontSize::Small, FontColor::White);
	DrawFont(0.0f, 30.0f, "FontSize:Regular", FontSize::Regular, FontColor::White);
	DrawFont(0.0f, 60.0f, "FontSize:Large", FontSize::Large, FontColor::White);

	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	FinishDraw();
}
