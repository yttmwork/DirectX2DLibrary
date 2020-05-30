/**
* @file Texture.h
* @brief テクスチャの読み込み、解放など、テクスチャに関する関数、定数の宣言
*/
#ifndef TEXTURE_H_
#define TEXTURE_H_

/** @brief テクスチャデータやサイズを保持する構造体 */
struct Texture
{
	LPDIRECT3DTEXTURE9 TexutreData;	//!< テクスチャデータ
	int Width;						//!< 横幅
	int Height;						//!< 縦幅
};

/**
* @brief テクスチャデータ初期化関数
* @details ゲームで使用するテクスチャデータを保存出来るようにする
*/
void InitTexture();

/**
* @brief テクスチャの全解放
* @details 読み込んでいるすべてのテクスチャを解放する
*/
void ReleaseAllTextures();


/**
* @brief テクスチャの解放
* @details 指定されたーキーワードのテクスチャを解放する
* @param[in] keyword 解放するテクスチャのキーワード
*/
void ReleaseTexture(const char* keyword);

/**
* @brief テクスチャの読み込み
* @details 指定したされたテクスチャファイルを読み込み、keywordの文字列で登録する
* @retval true 読み込み成功
* @retval false 読み込み失敗
* @param[in] keyword 登録用キーワード
* @param[in] file_name 読み込むテクスチャ名(パス込み)
*/
bool LoadTexture(const char* keyword, const char* file_name);

/**
* @brief テクスチャデータの取得
* @details 指定されたキーワードのテクスチャデータを取得する
* @retval Texture* テクスチャデータ(取得失敗時はnullptr)
* @param[in] keyword 取得したいテクスチャのキーワード
*/
Texture* GetTexture(const char* keyword);

#endif
