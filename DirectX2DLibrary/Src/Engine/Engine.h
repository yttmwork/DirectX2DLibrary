/**
* @file Engine.h
* @brief エンジン処理(描画、入力、サウンド)に関する関数、定数の宣言
*/
#ifndef ENGINE_H_
#define ENGINE_H_

#include "Graphics.h"
#include "Texture.h"
#include "Input.h"
#include "Sound.h"

/**
* @brief エンジン初期化関数@n
* ゲームで使うエンジン(描画、入力、サウンド)の初期化とウィンドウの作成を行う@n
* ※この関数はゲームループ開始前に１度だけ実行する
* @retval true 初期化成功
* @retval false 初期化失敗
* @param[in] width ウィンドウの横幅
* @param[in] height ウィンドウの縦幅
* @param[in] title_str タイトルバーに表示する文字列
* @param[in] is_window_mode ウィンドウ or Fullスクリーン設定フラグ(オプション)
*/
bool InitEngine(int width, int height, const char* title_str, bool is_window_mode = true);

/**
* @brief エンジン解放関数@n
* ゲーム終了後にエンジンの解放を行う@n
* ※この関数はゲームループ終了後に１度だけ実行する
*/
void ReleaseEngine();

#endif
