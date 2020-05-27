#include <d3dx9.h>
#include <stdlib.h>
#include <map>
#include "Graphics.h"
#include "Texture.h"

// テクスチャデータとりまとめ用
struct TextureData
{
	std::map<const char*, Texture> TextureList;
};

static TextureData* g_Texture;

void InitTexture()
{
	AllReleaseTexture();

	g_Texture = new TextureData();
}

void ReleaseTexture(const char* keyword)
{
	if (g_Texture == nullptr)
	{
		return;
	}

	if (g_Texture->TextureList.count(keyword) > 0 &&
		g_Texture->TextureList[keyword].TexutreData != nullptr)
	{
		g_Texture->TextureList[keyword].TexutreData->Release();
		g_Texture->TextureList[keyword].TexutreData = nullptr;
		g_Texture->TextureList.erase(keyword);
	}
}

void AllReleaseTexture()
{
	if (g_Texture == nullptr)
	{
		return;
	}

	for (auto itr : g_Texture->TextureList)
	{
		if (itr.second.TexutreData == nullptr)
		{
			continue;
		}

		itr.second.TexutreData->Release();
		itr.second.TexutreData = nullptr;
	}

	delete g_Texture;
	g_Texture = nullptr;
}

bool LoadTexture(const char* keyword, const char* file_name)
{
	if (file_name == nullptr ||
		keyword == nullptr)
	{
		return false;
	}

	if (g_Texture->TextureList.count(keyword) > 0)
	{
		return true;
	}

	return CreateTexture(file_name, &g_Texture->TextureList[keyword]);
}

Texture* GetTexture(const char* keyword)
{
	if (g_Texture->TextureList.count(keyword) == 0)
	{
		return nullptr;
	}

	return &g_Texture->TextureList[keyword];
}
