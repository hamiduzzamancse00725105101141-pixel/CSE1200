#pragma warning(disable:4996)
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "GameGlobals.h"
#include "Audio.h"

void stopAllSounds()
{
	mciSendString("stop introsong", NULL, 0, NULL);
	mciSendString("stop menusong", NULL, 0, NULL);
	mciSendString("stop easySong", NULL, 0, NULL);
	mciSendString("stop mediumsong", NULL, 0, NULL);
	mciSendString("stop ggsong", NULL, 0, NULL);
}

void playGameSound(const char* command)
{
	if (soundEnabled)
		mciSendString(command, NULL, 0, NULL);
}
