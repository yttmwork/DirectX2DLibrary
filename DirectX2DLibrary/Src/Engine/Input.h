/**
* @file Input.h
* @brief 入力に関する関数、定数の宣言
*/
#ifndef INPUT_H_
#define INPUT_H_

#define DIRECTINPUT_VERSION 0x0800	//!< DirectInputのWarning対策

#include <dinput.h>
#include "../Utility/Vec.h"
#include "InputKeyboard.h"
#include "InputGamePad.h"
#include "InputMouse.h"

/** @brief キーボタンの種類 */
enum ButtonKind
{
	UpButton,
	DownButton,
	LeftButton,
	RightButton,
	Button01,
	Button02,
	Button03,
	Button04,
	Button05,
	Button06,
	Button07,
	Button08,
	ButtonKindMax,
};

/** @brief ボタンの状態 */
enum ButtonState
{
	ButtonStateNone,		//!< 未使用状態
	ButtonStatePushed,		//!< 押した瞬間
	ButtonStateHeld,		//!< 押している
	ButtonStateReleased,	//!< 離した瞬間
	ButtonStateMax,			//!< 最大数
};

/**
* @brief Input機能初期化関数
* @details <pre>
* 入力取得に必要な初期化を行う
* ※InitEngineで実行されるので使用者は実行する必要がない
* </pre>
* @retval true 初期化成功
* @retval false 初期化失敗
*/
bool InitInput();

/**
* @brief Input機能終了関数
* @details <pre>
* Input機能を終了させる
* ※ReleaseEngineで実行されるので使用者は実行する必要がない
* </pre>
*/
void ReleaseInput();

/**
* @brief 入力情報の更新
* @details <pre>
* デバイスの入力情報の更新を行う
* 毎フレーム実行する必要がある
* </pre>
*/
void UpdateInput();

/**
* @brief ボタン状態の更新
* @details <pre>
* ボタンの押下情報と状態から最新のボタンの状態を返す
* ※使用者が実行する必要はない
* </pre>
* @retval 更新されたボタンの状態
* @param[in] is_push ボタンの押下状態
* @param[in] state ボタンの状態
*/
ButtonState UpdateButtonState(bool is_push, ButtonState state);

#endif
