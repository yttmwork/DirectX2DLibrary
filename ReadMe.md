# ReadMe

## 概要
DirectX9を使用して作成した2Dゲーム用の簡易ライブラリです。  
テクスチャ描画、キーボードやマウスの入力、サウンドの再生を行うことができます。

## 素材
サンプルで使用している音素材は[魔王魂](https://maoudamashii.jokersounds.com/)の素材を使用しております。

## 環境構築
ライブラリにはDirectX9を使用しています。 
- DirectX9は[こちら](https://yttm-work.jp/directx/directx_0023.html)を参考にインストールまで行って下さい。  
- 各機能を使用するためのパス指定は以下のURLから確認できます。
 - [DirectGraphics](https://yttm-work.jp/directx/directx_0003.html#common_headline_direct_graphics_setting)
 - [DirectInput](https://yttm-work.jp/directx/directx_0011.html#common_headline_direct_input_setting)
 - [DirectSound](https://yttm-work.jp/directx/directx_0033.html#head_line_02)

## 使用方法
### include
使用するソースファイルでEngine.hを#includeに設定して下さい。

```
#include "Engine.h"
```

### 初期化
プログラム開始直後にInitEngine関数を使用してDirectGraphics、DirectInput、DirectSoundの初期化を行います。  
この関数はプログラム内で1度しか実行しないでください。

```
// エンジンの初期化
if (InitEngine(
	640, 		// ウィンドウサイズ(横幅)
	480, 		// ウィンドウサイズ(縦幅)
	"Sample"	// タイトルバーのタイトル
	true		// ウィンドウモード
	) == false)
{
	return 0;
}
```

### 解放
プログラム終了直前にReleaseEngine関数を使用してDirectGraphics、DirectInput、DirectSoundの解放を行います。  
この関数もInitEngine同様にプログラム中に1度しか実行しません。

```
// エンジンの解放
ReleaseEngine();

```

### DirectGraphics
#### テクスチャ読み込み
```
// テクスチャ読み込み
// 読み込むファイル名と登録用のキーワードを設定する
// 読み込みが完了後は登録したキーワードを使用してデータの取得や解放を行う
bool is_success = LoadTexture("Enemy", "Res/Enemy.png");

```

#### テクスチャ取得
```
// テクスチャ情報の取得
// 指定されたキーワードが存在したらデータが返る
// Textureにはテクスチャデータやテクスチャのサイズが保存されている
Texture* GetTexutre("Enemy");
```

#### 描画開始/終了
```
// 描画開始
// 描画処理を実行する場合、必ず最初実行する
// 1フレームに1度だけ実行する
DrawStart(0);

// テクスチャなどの描画処理を実行する

// 描画終了
// 描画処理を終了する場合に必ず実行する
DrawEnd();
```

#### テクスチャ描画
``` 通常
// テクスチャ描画
// 描画は座標を矩形の左上の位置として描画する
// 回転角度、拡縮率はオプション
DrawTexture(
	g_Position.X,  // 描画座標X
	g_Position.Y,  // 描画座標Y
	"Enemy",       // 使用するテクスチャのキーワード
	g_Angle,       // 回転角度
	g_Scale.X,     // 拡縮率X
	g_Scale.Y);    // 拡縮率Y
```

``` UV座標指定
DrawTextureUV(
	g_Position.X, // 描画座標X
	g_Position.Y, // 描画座標Y
	"Enemy",      // 使用するテクスチャのキーワード
	0.0f,         // テクスチャ座標
	0.0f,         // テクスチャ座標
	64.0f,        // テクスチャの描画サイズ(横幅)
	64.0f,        // テクスチャの描画サイズ(縦幅)
	g_Angle,      // 回転角度
	g_Scale.X,    // 拡縮率X
	g_Scale.Y);   // 拡縮率Y

```

#### フォント描画
```
// フォント描画
DrawFont(
	0.0f,              // 描画座標X
	0.0f,              // 描画座標Y
	"FontSize:Small",  // 描画する文字列
	FontSize::Small,   // フォントサイズ
	FontColor::Black); // フォントカラー
```

#### テクスチャ解放
```単体解放
// 指定されたキーワードのテクスチャを解放する
ReleaseTexture("Enemy")
```

```全解放
// 読み込まれているテクスチャを全て解放する
AllReleaseTexture();
```

### DirectInput
#### 更新処理
```
// 入力情報を更新する
// 毎フレーム実行する必要あり
UpdateInput();
```

#### 入力情報
以下の入力情報を取得できる
- 押された瞬間
- 押している間
- 離された瞬間

#### キーボード入力取得
```
// キーの指定はDIK～で行う
// 押された瞬間
if (GetKeyboardDown(DIK_A) == true)
{
	// Aキーが押された瞬間
}

// 押している間
if (GetKeyboard(DIK_A) == true)
{
	// Aキーが押されている
}

// 離された瞬間
if (GetKeyboardUp(DIK_A) == true)
{
	// Aキーが離された瞬間
}
```

#### マウス入力取得
```
// 押された瞬間
if (GetMouseDown(MouseButton::Left) == true)
{
	// マウスの左ボタンを押した瞬間
}

// 押している間
if (GetMouse(MouseButton::Left) == true)
{
	// マウスの左ボタンを押している
}

// 離された瞬間
if (GetMouseUp(MouseButton::Left) == true)
{
	// マウスの左ボタンを離した瞬間
}
```

#### ゲームパッド入力取得
```
// 押された瞬間
if (GetGamePadKeyDown(GamePadKind::GamePadKindButton01) == true)
{
	// ゲームパッドのボタン1を押した瞬間
}

// 押している間
if (GetGamePadKey(GamePadKind::GamePadKindButton01) == true)
{
	// ゲームパッドのボタン1を押している
}

// 離された瞬間
if (GetGamePadKeyUp(GamePadKind::GamePadKindButton01) == true)
{
	// ゲームパッドのボタン1を離した瞬間
}
```

### DirectSound
#### サウンドファイル読み込み
``` 
// ファイルの読み込み
// 読み込むファイル名と登録用のキーワードを設定する
// 読み込みが完了後は登録したキーワードを使用してデータの取得や解放を行う
// 対応フォーマットはwav
LoadSoundFile("Bgm", "Res/Bgm.wav");
```

#### サウンドファイル解放
```
// 指定したキーワードのサウンドファイルを解放する
ReleaseSoundFile("Bgm");
```

```
// サウンドファイルを全て解放する
AllReleaseSoundFile();
```

#### サウンド再生
```
// 通常再生
// 停止時に先頭に戻す処理を実装しているため再生は先頭から行われる
// Bgmの再生に効果的
// Stop関数で再生を止めることが出来る
StartPlayingSound("Bgm", true);
```

```
// 重複再生
// 同じ音を同時に再生することが出来る
// SEの再生に効果的
// Stopによる停止はできない
StartPlayingDuplicateSound("Se");
```

#### サウンド停止
```
// 停止
// 指定したキーワードの音を停止する
SoundStop("Bgm");
```

#### 重複再生サウンド削除
```
// 重複再生サウンドの削除
// 重複再生が終了したサウンドを削除する
// 毎フレームチェックをする必要があるので毎フレーム実行する必要あり
EraseDuplicateSound();
```

