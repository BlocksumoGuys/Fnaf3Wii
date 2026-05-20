#include "main.h"
IMG_Anim CamStatic_over = {0};
GRRLIB_texImg *gameover = NULL;
u8 CamStatic_over_f = 0;
u8 CamStatic_over_alpha = 255;
bool goTitle = false;
int stare;
int gameoverSceneMain(){
    u64 frame = 0;
    u8 white_alpha = 255;
    CamStatic_over.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/37.png");
    CamStatic_over.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/33.png");
    CamStatic_over.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/36.png");
    CamStatic_over.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/34.png");
    CamStatic_over.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/35.png");
    CamStatic_over.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/36.png");
    gameover = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/339.png");
    stare = SM_LoadAudio("stare.wav");
    SM_PlayAudio(stare, 127, 127, false, false, 0);
    while (1)
    {
        WPAD_ScanPads();
        u32 mote = WPAD_ButtonsDown(0);
        if(frame>=(60*5)){
            SM_FreeAudio(stare);
            CamStatic_over_alpha = 50;
            GRRLIB_DrawImg(CenterX(gameover), CenterY(gameover), gameover, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }
        if(frame>=(60*10)){
            goTitle = true;
            break;
        }
        GRRLIB_DrawImg(0, 0, CamStatic_over.frames[CamStatic_over_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00+CamStatic_over_alpha);
        if(frame%2==0){
            CamStatic_over_f = (CamStatic_over_f+1) % 6;
        }
        GRRLIB_Rectangle(0, 0, 640, 480, 0xFFFFFF00+white_alpha, true);
        if(white_alpha > 0){
            white_alpha-=5;
        }
        if(mote & WPAD_BUTTON_HOME){
            goTitle = false;
            break;
        }
        frame++;
        GRRLIB_Render();
    }
    FreeImgAnim(&CamStatic_over);
    GRRLIB_FreeTexture(gameover);
    SM_FreeAllAudio();
    if(goTitle){
        menuSceneMain();
    }
    return 0;
}