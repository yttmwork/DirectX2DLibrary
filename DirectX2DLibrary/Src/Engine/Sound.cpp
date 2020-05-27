#include <dsound.h>
#include <stdio.h>
#include <map>
#include <vector>
#include "Window.h"
#include "Sound.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
// mmioで必要
#pragma comment(lib, "winmm.lib")

// Wavデータ格納用
struct WavData
{
	WAVEFORMATEX WavFormat;
	char* SoundBuffer;
	DWORD Size;
};

// サウンドエンジンデータ
struct SoundEngineData
{
	// DIRECTSOUND8のポインタ
	LPDIRECTSOUND8 Interface = nullptr;

	// サウンドデータ保存用
	std::map<const char*, LPDIRECTSOUNDBUFFER> BufferList;

	// 複製バッファ保存用
	std::vector<LPDIRECTSOUNDBUFFER> DuplicateList;
};

static SoundEngineData* Sound; // サウンドデータ

// WavFile読み込み
bool LoadWavFile(const char* file_name, WavData* out_wave_data);

bool InitSound()
{
	Sound = new SoundEngineData();

	// DirectSoundの生成
	if (FAILED(DirectSoundCreate8(
			nullptr,			// GUID
			&Sound->Interface,	// 生成したDirectSound保存先
			nullptr)))			// NULL固定
	{
			return false;
	}

	// 協調レベルの設定
	if (FAILED(Sound->Interface->SetCooperativeLevel(
			FindWindow(WINDOW_CLASS_NAME, nullptr),		// ウィンドウハンドル
			DSSCL_NORMAL)))								// 協調レベル
	{
			return false;
	}

	return true;
}

void ReleaseSound()
{
	AllReleaseSoundFile();

	// 全解放
	for (auto itr = Sound->DuplicateList.begin(); itr != Sound->DuplicateList.end(); itr++)
	{
		if (*itr == nullptr)
		{
			continue;
		}

		(*itr)->Stop();
		(*itr)->Release();
		(*itr) = nullptr;
	}
	Sound->DuplicateList.clear();

	// DirectSoundインターフェースの解放
	if (Sound->Interface != nullptr)
	{
		Sound->Interface->Release();
		Sound->Interface = nullptr;
	}

	delete Sound;
	Sound = nullptr;
}

bool LoadSoundFile(const char* keyword, const char* file_name)
{
	WavData wav_data;
	
	if (LoadWavFile(file_name, &wav_data) == false)
	{
		return false;
	}

	// バッファ情報の設定
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = wav_data.Size;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = &wav_data.WavFormat;

	// セカンダリバッファ作成
	if (FAILED(Sound->Interface->CreateSoundBuffer(
			&dsbd,							// バッファ情報
			&Sound->BufferList[keyword],	// 作成されたバッファの保存先
			nullptr)))
	{
		// 作成失敗
		delete[] wav_data.SoundBuffer;
		return false;
	}

	// 波形データを書き込むためにセカンダリバッファをロックする
	void* buffer;
	DWORD buffer_size;
	if (FAILED(Sound->BufferList[keyword]->Lock(
		0,				// オフセット
		wav_data.Size,	// ロックするバッファサイズ
		&buffer,		// ロックされたバッファパート１の保存先
		&buffer_size,	// ロックされたバッファパート１のサイズ
		nullptr,		// ロックされたバッファパート２の保存先
		nullptr,		// ロックされたバッファパート２のサイズ
		0)))			// ロックオプション
	{
		// ロック失敗
		delete[] wav_data.SoundBuffer;
		return false;
	}

	memcpy(buffer, wav_data.SoundBuffer, buffer_size);

	Sound->BufferList[keyword]->Unlock(
		&buffer,		// アンロックするバッファパート１
		buffer_size,	// パート１のバッファサイズ
		nullptr,		// アンロックするバッファパート２
		0);				// パート２のバッファサイズ

	// コピーが終わったのでサウンドデータを解放
	delete[] wav_data.SoundBuffer;

	return true;
}

void ReleaseSoundFile(const char* keyword)
{
	if (Sound->BufferList.count(keyword) > 0)
	{
		Sound->BufferList[keyword]->Stop();
		Sound->BufferList[keyword]->Release();
		Sound->BufferList[keyword] = nullptr;
		Sound->BufferList.erase(keyword);
	}
}

void AllReleaseSoundFile()
{
	// セカンダリバッファの解放
	for (auto itr = Sound->BufferList.begin(); itr != Sound->BufferList.end(); itr++)
	{
		if (itr->second != nullptr)
		{
			itr->second->Stop();
			itr->second->Release();
			itr->second = nullptr;
		}
	}

	Sound->BufferList.clear();
}

void StartPlayingSound(const char* keyword, bool is_loop)
{
	if (Sound->BufferList.count(keyword) == 0)
	{
		return;
	}

	int loop_bit = is_loop == true ? 1 : 0;
	// 再生
	Sound->BufferList[keyword]->Play(
		0,
		0,
		DSBPLAY_LOOPING & loop_bit);
}

void StartPlayingDuplicateSound(const char* keyword)
{
	if (Sound->BufferList.count(keyword) == 0)
	{
		return;
	}

	// バッファの複製と追加
	Sound->DuplicateList.emplace_back();
	int last_id = (int)Sound->DuplicateList.size() - 1;
	Sound->Interface->DuplicateSoundBuffer(
		Sound->BufferList[keyword],	// 複製元
		&Sound->DuplicateList[last_id]);		// 複製先

	// 複製したバッファで再生
	Sound->DuplicateList[last_id]->Play(0, 0, 0);
}

void StopSound(const char* keyword)
{
	if (Sound->BufferList[keyword] == nullptr)
	{
		return;
	}

	// 停止
	Sound->BufferList[keyword]->Stop();
	// 再生位置を先頭に戻す
	Sound->BufferList[keyword]->SetCurrentPosition(0);
}

void EraseDuplicateSound()
{
	for (auto itr = Sound->DuplicateList.begin(); itr != Sound->DuplicateList.end(); )
	{
		bool is_erase = false;

		if (*itr == nullptr)
		{
			is_erase = true;
		}
		else
		{
			DWORD status;

			// 状態取得
			(*itr)->GetStatus(&status);

			if (!(status & DSBSTATUS_PLAYING))
			{
				(*itr)->Release();
				(*itr) = nullptr;

				is_erase = true;
			}
		}

		if (is_erase == true)
		{
			itr = Sound->DuplicateList.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}


bool LoadWavFile(const char* file_name, WavData* out_wave_data)
{
	// WindowsマルチメディアAPIのハンドル
	HMMIO mmio_handle = nullptr;

	// チャンク情報
	MMCKINFO ck_info;
	// RIFFチャンク用
	MMCKINFO riffck_info;

	// WAVファイル内音サンプルのサイズ
	DWORD dwWavSize = 0;

	// WAVファイルを開く
	mmio_handle = mmioOpen(
		(char*)file_name,	// ファイル名
		nullptr,			// MMIO情報
		MMIO_READ);			// オープンモード

	if (mmio_handle == nullptr)
	{
		// オープン失敗
		return false;
	}

	// RIFFチャンクに進入するためにfccTypeにWAVEを設定する
	riffck_info.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// RIFFチャンクに侵入する
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,	// MMIOハンドル
		&riffck_info,	// 取得したチャンクの情報
		nullptr,		// 親チャンク
		MMIO_FINDRIFF))	// 取得情報の種類
	{
		// 失敗
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// 進入先のチャンクを"fmt "として設定する
	ck_info.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(mmio_handle, &ck_info, &riffck_info, MMIO_FINDCHUNK))
	{
		// fmtチャンクがない
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// fmtデータの読み込み
	LONG read_size = mmioRead(
		mmio_handle,						// ハンドル
		(HPSTR)&out_wave_data->WavFormat,	// 読み込み用バッファ
		sizeof(out_wave_data->WavFormat));	// バッファサイズ

	if (read_size != sizeof(out_wave_data->WavFormat))
	{
		// 読み込みサイズが一致してないのでエラー
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// フォーマットチェック
	if (out_wave_data->WavFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		// フォーマットエラー
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// fmtチャンクを退出する
	if (mmioAscend(mmio_handle, &ck_info, 0) != MMSYSERR_NOERROR)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// dataチャンクに進入する
	ck_info.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(mmio_handle, &ck_info, &riffck_info, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		// 進入失敗
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// サイズを保存
	out_wave_data->Size = ck_info.cksize;

	// dataチャンク読み込み
	out_wave_data->SoundBuffer = new char[ck_info.cksize];
	read_size = mmioRead(mmio_handle, (HPSTR)out_wave_data->SoundBuffer, ck_info.cksize);
	if (read_size != ck_info.cksize)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		delete[] out_wave_data->SoundBuffer;
		return false;
	}

	// ファイルを閉じる
	mmioClose(mmio_handle, MMIO_FHOPEN);

	return true;
}
