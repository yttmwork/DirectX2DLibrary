/**
* @file InputMouse.h
* @brief マウス入力に関する関数、定数の宣言
*/
#ifndef INPUT_MOUSE_H_
#define INPUT_MOUSE_H_

#include <dinput.h>
#include "../Useful/Vec.h"

/** @brief マウスボタンの種類 */
enum MouseButton
{
	Left,		//!< 左
	Right,		//!< 右
	Center,		//!< 真ん中
};

/**
* @brief マウス入力初期化関数
* @details <pre>
* マウスの入力取得に必要な初期化を行う
* ※InitInputで実行されるので使用者は実行する必要がない
* </pre>
* @retval true 初期化成功
* @retval false 初期化失敗
* @param[in] input_interface DirectInputのインターフェース
*/
bool InitMouse(LPDIRECTINPUT8 input_interface);

/**
* @brief マウス入力解放関数
* @details <pre>
* マウスの入力機能を終了する
* ※ReleaseInputで実行されるので使用者は実行する必要がない
* </pre>
*/
void ReleaseMouse();

/**
* @brief 入力情報の更新
* @details <pre>
* マウスの入力情報の更新を行う
* 毎フレーム実行する必要がある
* ※UpdateInputで実行されるので使用者は実行する必要がない
* </pre>
*/
void UpdateMouse();

/**
* @brief マウスボタンの押下状態判定関数
* @retval true 押されている
* @retval false 押されていない
* @param[in] button_type 判定したいボタンの種類
*/
bool IsHeldMouse(MouseButton button_type);

/**
* @brief マウスボタンが押された瞬間の判定関数
* @retval true 押した瞬間
* @retval false 押した瞬間以外
* @param[in] button_type 判定したいボタンの種類
*/
bool IsPushedMouse(MouseButton button_type);

/**
* @brief マウスボタンを離した瞬間の判定関数
* @retval true 離した瞬間
* @retval false 離した瞬間以外
* @param[in] button_type 判定したいボタンの種類
*/
bool IsReleasedMouse(MouseButton button_type);

#endif
