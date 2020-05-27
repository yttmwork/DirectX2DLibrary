/**
* @file InputMouse.h
* @brief マウス入力に関する関数、定数の宣言
*/
#ifndef INPUT_MOUSE_H_
#define INPUT_MOUSE_H_

#include <dinput.h>
#include "../Utility/Vec.h"

/** @brief マウスボタンの種類 */
enum MouseButton
{
	Left,		//!< 左
	Right,		//!< 右
	Center,		//!< 真ん中
};

/**
* @brief マウス入力初期化関数@n
* マウスの入力取得に必要な初期化を行う@n
* ※InitInputで実行されるので使用者は実行する必要がない
* @retval true 初期化成功
* @retval false 初期化失敗
* @param[in] input_interface DirectInputのインターフェース
*/
bool InitMouse(LPDIRECTINPUT8 input_interface);

/**
* @brief マウス入力解放関数@n
* マウスの入力機能を終了する@n
* ※ReleaseInputで実行されるので使用者は実行する必要がない
*/
void ReleaseMouse();

/**
* @brief 入力情報の更新@n
* マウスの入力情報の更新を行う@n
* 毎フレーム実行する必要がある@n
* ※UpdateInputで実行されるので使用者は実行する必要がない
*/
void UpdateMouse();

/**
* @brief マウスボタンの押下状態判定関数
* @retval true 押されている
* @retval false 押されていない
* @param[in] button_type 判定したいボタンの種類
*/
bool GetMouse(MouseButton button_type);

/**
* @brief マウスボタンが押された瞬間の判定関数
* @retval true 押した瞬間
* @retval false 押した瞬間以外
* @param[in] button_type 判定したいボタンの種類
*/
bool GetMouseDown(MouseButton button_type);

/**
* @brief マウスボタンを離した瞬間の判定関数
* @retval true 離した瞬間
* @retval false 離した瞬間以外
* @param[in] button_type 判定したいボタンの種類
*/
bool GetMouseUp(MouseButton button_type);

#endif
