#include "main.h"
char path_b[50];

SM_Audio SM_List[SM_MAX_AUDIO];

void SM_Init(){
    memset(SM_List, 0, sizeof(SM_List));
}

int SM_next_free(){
    for(int i = 1; i < SM_MAX_AUDIO; i++){
        if(SM_List[i].aud_b == NULL){
            return i;
        }
    }
}

int SM_LoadAudio(char* audio_name){
    sprintf(path_b, "sd:/fnaf3/assets/sounds/%s", audio_name);
    int id = SM_next_free();
    SM_List[id].aud_b = loadBuf(path_b, &SM_List[id].aud_s);
    SM_List[id].aud_id = id;
    return id;
}

void* SM_FreeAudio(int id){
    if(ASND_StatusVoice(id)){
        ASND_StopVoice(id);
    }
    free(SM_List[id].aud_b);
    memset(&SM_List[id], 0, sizeof(SM_List[id]));
}

void SM_StopAudio(int id){
    ASND_StopVoice(id);
}

void SM_PlayAudio(int id, int l_volume, int r_volume, bool looping, bool doDestroy, int delay){
    if(ASND_StatusVoice(id)) return;
    SM_List[id].looping = looping;
    SM_List[id].l_volume = l_volume;
    SM_List[id].r_volume = r_volume;
    SM_List[id].doDestroy = doDestroy;
    SM_List[id].delay = delay;
    ASND_SetVoice(id, VOICE_MONO_16BIT_LE, ASND_GetAudioRate(), delay, SM_List[id].aud_b, SM_List[id].aud_s, l_volume, r_volume, 0);
}

void SM_UpdateAudio(){
    for(int i = 1; i < SM_MAX_AUDIO; i++){
        if(SM_List[i].aud_b != NULL){
            if(SM_List[i].looping){
                if(!ASND_StatusVoice(i)){
                    SM_PlayAudio(i, SM_List[i].l_volume, SM_List[i].r_volume, true, 0, SM_List[i].delay);
                }
            }
            if(SM_List[i].doDestroy){
                if(!ASND_StatusVoice(i)){
                    SM_FreeAudio(i);
                }
            }
        }   
    }
}

void SM_ChangeVolume(int id, int l_volume, int r_volume){
    ASND_ChangeVolumeVoice(id, l_volume, r_volume);
}
