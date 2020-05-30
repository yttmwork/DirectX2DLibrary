/**
* @file Graphics.h
* @brief 描画に関する関数、定数の宣言
*/
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <Windows.h>
#include <d3d9.h>
#include "Texture.h"

/** @brief フォントカラー */
enum FontColor
{
	Black,			//!< 黒
	White,			//!< 白
	Red,			//!< 赤
};

/** @brief フォントサイズ */
enum FontSize
{
	Small,			//!< 小
	Regular,		//!< 中
	Large,			//!< 大
	FontSizeMax,	//!< サイズ最大数
};

const int SmallFontSize = 16;	//!< フォントサイズ(小)
const int RegularFontSize = 24;	//!< フォントサイズ(中)
const int LargeFontSize = 32;	//!< フォントサイズ(大)

/**
* @brief Graphics機能初期化関数
* @details <pre>
* 描画を使用するための初期化を行う
* ※Engine.cppのInitEngineで実行しているので使用者は実行する必要がない
* </pre>
* @retval true 初期化成功
* @retval false 初期化失敗
* @param[in] is_window_mode ウィンドウ or Fullスクリーン設定フラグ
*/
bool InitGraphics(bool is_window_mode);

/**
* @brief Graphics機能終了関数
* @details <pre>
* 描画関連の処理の後始末を行う処理
* ※Engine.cppのReleaseEngineで実行しているので使用者は実行する必要がない
* </pre>
*/
void ReleaseGraphics();

/**
* @brief 描画開始関数
* @details <pre>
* 描画開始を宣言し、バックバッファのクリアを行う
* この関数実行後は必ずDrawEndを実行する必要がある
* </pre>
* @retval true 描画開始成功
* @retval false 描画開始失敗
* @param[in] color クリアカラー
*/
bool StartDraw(DWORD color);

/**
* @brief 描画終了関数
* @details <pre>
* 描画の終了を宣言し、バックバッファとフロントバッファを入れ替える
* ※この関数は必ずDrawStartの後に実行する
* </pre>
*/
void FinishDraw();

/**
* @brief テクスチャ描画関数
* @details 指定された位置にテクスチャを描画する
* @param[in] x X軸描画座標
* @param[in] y Y軸描画座標
* @param[in] texture_keyword 描画で使うテクスチャのキーワード
* @param[in] alpha 透過値(オプション)
* @param[in] angle 回転角度(オプション)
* @param[in] scale_x 拡縮率X(オプション)
* @param[in] scale_y 拡縮率Y(オプション)
*/
void DrawTexture(float x, float y, const char* texture_keyword, UCHAR alpha = 255, float angle = 0.0f, float scale_x = 1.0f, float scale_y = 1.0f);

/**
* @brief テクスチャ描画関数UV指定バージョン
* @details 指定された位置にUV指定されたテクスチャを描画する
* @param[in] x X軸描画座標
* @param[in] y Y軸描画座標
* @param[in] texture_keyword 描画で使うテクスチャのキーワード
* @param[in] tex_x テクスチャのX座標
* @param[in] tex_y テクスチャのY座標
* @param[in] sprite_width スプライト横幅
* @param[in] sprite_height スプライト縦幅
* @param[in] alpha 透過値(オプション)
* @param[in] angle 回転角度(オプション)
* @param[in] scale_x 拡縮率X(オプション)
* @param[in] scale_y 拡縮率Y(オプション)
*/
void DrawTextureUV(float x, float y, const char* texture_keyword, float tex_x, float tex_y, float sprite_width, float sprite_height, UCHAR alpha = 255, float angle = 0.0f, float scale_x = 1.0f, float scale_y = 1.0f);

/**
* @brief フォント描画関数
* @details 指定された位置にフォントを描画する
* @param[in] x X軸描画座標
* @param[in] y Y軸描画座標
* @param[in] text 描画するテキスト
* @param[in] size フォントサイズの種類
* @param[in] color フォントの色
*/
void DrawFont(float x, float y, const char* text, FontSize size, FontColor color);

/**
* @brief テクスチャ作成
* @details <pre>
* 指定された情報から読み込み、テクスチャを作成する
* ※使用者はこの関数は使用せず、File.hのLoadTextureを使用してください。
* </pre>
* @retval true 作成成功
* @retval false 作成失敗
* @param[in] file_name 読み込むテクスチャの名前(パス込み)
* @param[out] texture_data 読み込まれたテクスチャを反映するデータ
*/
bool CreateTexture(const char* file_name, Texture* texture_data);

#endif
