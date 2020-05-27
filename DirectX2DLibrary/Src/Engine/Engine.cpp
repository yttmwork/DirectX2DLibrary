#include "Window.h"
#include "Graphics.h"
#include "Input.h"
#include "Texture.h"
#include "Sound.h"
#include "Engine.h"

bool InitEngine(int width, int height, const char* title_str, bool is_window_mode)
{
	if (MakeWindow(width, height, title_str) == false)
	{
		return false;
	}

	if (InitGraphics(is_window_mode) == false)
	{
		return false;
	}

	if (InitInput() == false)
	{
		return false;
	}

	if (InitSound() == false)
	{
		return false;
	}

	InitTexture();

	return true;
}

void ReleaseEngine()
{
	AllReleaseTexture();
	AllReleaseSoundFile();

	ReleaseGraphics();
	ReleaseInput();
	ReleaseSound();
}
