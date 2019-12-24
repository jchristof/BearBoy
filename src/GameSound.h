#ifndef GAME_SOUND_H
#define GAME_SHOUND_H
#include "Sound.h"
#include <stdarg.h>

void PlaySong(const unsigned char* music[], unsigned char bank, unsigned char loop);
void PlaySound(SOUND_CHANNEL channel, UINT8 mute_frames,...);
void MuteMusic();
void MuteSfx();
void UnMuteMusic();
void UnMuteSfx();
UINT8 IsMusicMuted();
UINT8 IsSfxMuted();
void StopAudio();

#endif