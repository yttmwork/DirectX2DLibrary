/**
* @file Window.h
* @brief ウィンドウに関する外部公開関数、定数の宣言
*/
#ifndef WINDOW_H_
#define WINDOW_H_

#include <Windows.h>

#define WINDOW_CLASS_NAME "Window"	//!< ウィンドウクラス名

/**
* @brief ウィンドウ生成関数
* @details 引数で指定された内容でウィンドウを作成する
* @retval true 生成成功
* @retval false 生成失敗
* @param[in] width 横幅
* @param[in] height 縦幅
* @param[in] title タイトルバーに表示される文字列
*/
bool MakeWindow(int width, int height, const char* title);

#endif
