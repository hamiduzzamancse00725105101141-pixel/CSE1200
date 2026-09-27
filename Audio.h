#ifndef SHADOW_SPRINT_AUDIO_H
#define SHADOW_SPRINT_AUDIO_H

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void stopAllSounds();
void playGameSound(const char* command);

#endif
