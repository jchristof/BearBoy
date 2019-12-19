#include "GameSound.h"
#include "main.h"
#include "gbt_player.h"
#include "BankManager.h"
#include "Sound.h"

typedef struct {
    const unsigned char** music;
    UINT8 bank:4;
    UINT8 loop:1;
    UINT8 musicMuted:1;
    UINT8 sfxMuted:1;
} GameAudio;

GameAudio audio = {0,0,0,0,0};

void EnableAudio(){
    NR52_REG = 0x80; //Enables sound, you should always setup this first
    NR51_REG = 0xFF; //Enables all channels (left and right)
    NR50_REG = 0x77; //Max volume
}

void PlaySong(const unsigned char* music[], unsigned char bank, unsigned char loop){
    audio.music = music;
    audio.bank = bank;
    audio.loop = loop;

    EnableAudio();
    if(audio.musicMuted)
        return;

    PlayMusic(music, bank, loop);
}

// void PlaySound(SOUND_CHANNEL channel, UINT8 mute_frames, ...){
//     va_list list;
//     va_start(list, mute_frames);

//     //if(!audio.sfxMuted){
//         PlayFx(channel, mute_frames, &(list[0]));
//    // }
    
//     va_end(list);
// }

void MuteMusic(){
    audio.musicMuted = 1;
    gbt_stop();
    EnableAudio();
}

void MuteSfx(){
    MuteFx(1);
    audio.sfxMuted = 1;
}

void UnMuteMusic(){
    audio.musicMuted = 0;
    if(audio.music == 0)
        return;

    PlayMusic(audio.music, audio.bank, audio.loop);
}

void UnMuteSfx(){
    MuteFx(0);
    audio.sfxMuted = 0;
}

UINT8 IsMusicMuted(){
    return audio.musicMuted;
}

UINT8 IsSfxMuted(){
    return audio.sfxMuted;
}

void StopAudio(){

}