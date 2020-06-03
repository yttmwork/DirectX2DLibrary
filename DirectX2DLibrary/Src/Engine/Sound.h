/**
* @file Sound.h
* @brief サウンドに関する関数、定数の宣言
*/
#ifndef SOUND_H_
#define SOUND_H_

#include <Windows.h>

/**
* @brief サウンド機能初期化関数
* @details <pre>
* サウンドを使用するための初期化を行う
* ※Engine.cppのInitEngineで実行しているので使用者は実行する必要がない
* </pre>
* @retval true 初期化成功
* @retval false 初期化失敗
*/
bool InitSound();

/**
* @brief サウンド機能終了関数
* @details <pre>
* サウンド機能を終了させる
* ※Engine.cppのReleaseEngineで実行しているので使用者は実行する必要がない
* </pre>
*/
void ReleaseSound();

/**
* @brief サウンド再生関数
* @details 指定されたキーワードのサウンドファイルを再生する
* @param[in] keyword 再生するサウンドのキーワード
* @param[in] is_loop ループ設定(デフォルトは非ループ)
*/
void PlayMusic(const char* keyword, bool is_loop = false);

/**
* @brief 複製再生関数
* @details <pre>
* キーワード指定されたサウンドファイルを複製再生する
* SEのように同じサウンドを重複して再生する場合に効果的
* </pre>
* @param[in] keyword 再生するサウンドのキーワード
*/
void PlayDuplicateMusic(const char* keyword);

/**
* @brief サウンド停止関数
* @details <pre>
* キーワード指定されたサウンドを停止する
* ※PlayDuplicateMusicを使用して再生しているサウンドは対象外
* </pre>
* @param[in] keyword 停止するサウンドのキーワード
*/
void StopMusic(const char* keyword);

/**
* @brief サウンドファイルの読み込み関数
* @details 指定されたファイル名のサウンドファイルを読み込みkeywordの文字列で登録する
* @retval true 読み込み成功
* @retval false 読み込み失敗
* @param[in] keyword 登録用キーワード
* @param[in] file_name ファイル名
*/
bool LoadSoundFile(const char* keyword, const char* file_name);

/**
* @brief サウンドファイルの解放関数
* @details 指定されたキーワードのサウンドファイルを解放する
* @param[in] keyword 解放するサウンドファイルのキーワード
*/
void ReleaseSoundFile(const char* keyword);

/**
* @brief サウンドファイルの全解放関数
* @details サウンドファイルを全て解放する
*/
void ReleaseAllSoundFiles();

/**
* @brief 重複再生サウンド削除関数
* @details <pre>
* 再生が終了したPlayDuplicateMusicのサウンドに対して終了処理を行う
* 毎フレーム実行する必要がある
</pre>
*/
void EraseDuplicateMusic();

#endif
