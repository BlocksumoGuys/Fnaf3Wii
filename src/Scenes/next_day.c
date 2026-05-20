#include "main.h"
IMG_Anim night_time = {0};
int win_song;
int child_cheering;
int next_night;
bool quitMenu = false;
int next_daySceneMain(){
    u64 frame = 0;
    u8 night_t_f = 0;
    night_time.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/1129.png");
    night_time.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/1145.png");
    night_time.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/1144.png");
    night_time.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/1146.png");
    night_time.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/1147.png");
    night_time.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/697.png");
    night_time.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/1148.png");
    char* night_text = read_line(SD_SAVE, 0);
    if (strcmp(night_text, "NIGHT=1") == 0) {write_line(SD_SAVE, 0, "NIGHT=2");}
    else if (strcmp(night_text, "NIGHT=2") == 0) {write_line(SD_SAVE, 0, "NIGHT=3");}
    else if (strcmp(night_text, "NIGHT=3") == 0) {write_line(SD_SAVE, 0, "NIGHT=4");}
    else if (strcmp(night_text, "NIGHT=4") == 0) {write_line(SD_SAVE, 0, "NIGHT=5");}
    else if (strcmp(night_text, "NIGHT=5") == 0) {write_line(SD_SAVE, 0, "NIGHT=6");}
    else if (strcmp(night_text, "NIGHT=6") == 0) {quitMenu = true;}
    win_song = SM_LoadAudio("GingleBells.wav");
    child_cheering = SM_LoadAudio("CrownSmall.wav");
    SM_PlayAudio(win_song, 127, 127, false, true, 0);
    while (1)
    {
        if(frame==100){
            SM_PlayAudio(child_cheering, 127, 127, false, true, 0);
        }
        SM_UpdateAudio();
        GRRLIB_DrawImg(CenterX(night_time.frames[0]), CenterY(night_time.frames[0]), night_time.frames[night_t_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        if(frame>=90){
            if(frame%2==0){
                if(night_t_f < 6){
                    night_t_f++;
                }else{night_t_f = 1;}
            }
        }
        if(frame>=(60*10)){
            break;
        }
        GRRLIB_Render();
        frame++;
    }
    SM_FreeAllAudio();
    FreeImgAnim(&night_time);
    if(!quitMenu){
        gameSceneMain();
    }else{menuSceneMain();}
    return 0;
}