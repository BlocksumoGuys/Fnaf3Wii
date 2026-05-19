#pragma once
#include "main.h"
#define SM_MAX_AUDIO 20


typedef struct
{
    s32 aud_s;
    u8* aud_b;
    u8 aud_id;
    bool looping;
    int l_volume;
    int r_volume;
    bool doDestroy;
    int delay;
}SM_Audio;


void SM_Init();
int SM_next_free();
int SM_LoadAudio(char* audio_name);
void* SM_FreeAudio(int id);
void SM_StopAudio(int id);
void SM_PlayAudio(int id, int l_volume, int r_volume, bool looping, bool doDestroy, int delay);
void SM_UpdateAudio();
void SM_ChangeVolume(int id, int l_volume, int r_volume);


