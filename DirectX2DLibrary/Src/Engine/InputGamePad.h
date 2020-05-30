/**
* @file InputGamePad.h
* @brief ゲームパッド入力に関する関数、定数の宣言
*/
#ifndef INPUT_GAME_PAD_H_
#define INPUT_GAME_PAD_H_

#include "../Utility/Vec.h"

/** @brief ゲームパッドボタンの種類 */
enum GamePadKind
{
	GamePadKindUp,
	GamePadKindDown,
	GamePadKindLeft,
	GamePadKindRight,
	GamePadKindButton01,
	GamePadKindButton02,
	GamePadKindButton03,
	GamePadKindButton04,
	GamePadKindButton05,
	GamePadKindButton06,
	GamePadKindButton07,
	GamePadKindButton08,
	GamePadKindMax,
};

/**
* @brief ゲームパッド入力初期化関数@n
* ゲームパッドの入力取得に必要な初期化を行う@n
* ※InitInputで実行されるので使用者は実行する必要がない
* @retval true 初期化成功
* @retval false 初期化失敗
* @param[in] input_interface DirectInputのインターフェース
*/
bool InitGamePad(LPDIRECTINPUT8 input_interface);

/**
* @brief ゲームパッド入力解放関数@n
* ゲームパッドの入力機能を終了する@n
* ※ReleaseInputで実行されるので使用者は実行する必要がない
*/
void ReleaseGamePad();

/**
* @brief 入力情報の更新@n
* ゲームパッドの入力情報の更新を行う@n
* 毎フレーム実行する必要がある@n
* ※UpdateInputで実行されるので使用者は実行する必要がない
*/
void UpdateGamePad();

/**
* @brief ゲームパッドボタンの押下状態判定関数
* @retval true 押されている
* @retval false 押されていない
* @param[in] button 判定したいボタンの種類
*/
bool IsHeldGamePad(GamePadKind button);

/**
* @brief ゲームパッドのボタンが押された瞬間の判定関数
* @retval true 押した瞬間
* @retval false 押した瞬間以外
* @param[in] button 判定したいボタンの種類
*/
bool IsPushedGamePad(GamePadKind button);

/**
* @brief ゲームパッドのボタンを離した瞬間の判定関数
* @retval true 離した瞬間
* @retval false 離した瞬間以外
* @param[in] button 判定したいボタンの種類
*/
bool IsReleasedGamePad(GamePadKind button);

#endif
