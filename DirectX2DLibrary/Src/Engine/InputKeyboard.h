﻿/**
* @file InputKeyboard.h
* @brief キーボード入力に関する関数、定数の宣言
*/
#ifndef INPUT_KEYBOARD_H_
#define INPUT_KEYBOARD_H_

#include <dinput.h>

/**
* @brief キーボード入力初期化関数@n
* キーボードの入力取得に必要な初期化を行う@n
* ※InitInputで実行されるので使用者は実行する必要がない
* @retval true 初期化成功
* @retval false 初期化失敗
* @param[in] input_interface DirectInputのインターフェース
*/
bool InitKeyboard(LPDIRECTINPUT8 input_interface);

/**
* @brief キーボード入力解放関数@n
* キーボードの入力機能を終了する@n
* ※ReleaseInputで実行されるので使用者は実行する必要がない
*/
void ReleaseKeyboard();

/**
* @brief 入力情報の更新@n
* キーボードの入力情報の更新を行う@n
* 毎フレーム実行する必要がある@n
* ※UpdateInputで実行されるので使用者は実行する必要がない
*/
void UpdateKeyboard();

/**
* @brief キーボードのキーの押下状態判定関数
* @retval true 押されている
* @retval false 押されていない
* @param[in] key_code 判定したいキーの種類(DIK_～を使用する)
*/
bool GetKeyboard(UINT key_code);

/**
* @brief キーボードのキーが押された瞬間の判定関数
* @retval true 押された瞬間
* @retval false 押された瞬間ではない
* @param[in] key_code 判定したいキーの種類(DIK_～を使用する)
*/
bool GetKeyboardDown(UINT key_code);

/**
* @brief キーボードのキーが離された瞬間の判定関数
* @retval true 離された瞬間
* @retval false 離された瞬間ではない
* @param[in] key_code 判定したいキーの種類(DIK_～を使用する)
*/
bool GetKeyboardUp(UINT key_code);

#endif
