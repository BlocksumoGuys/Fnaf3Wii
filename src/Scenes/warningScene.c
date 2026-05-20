#include "main.h"

#define FADE_OUT_T

int warningSceneMain(){
    GRRLIB_texImg *img = GRRLIB_LoadTextureFromFile(WARNING_IMG);
    u8 alpha = 0;
    u32 t1 = time(NULL);
    u32 t2;
    bool isClicked = false;
    while(1){
        WPAD_ScanPads();
        u32 mote = WPAD_ButtonsDown(0);
        if(imgCheck(img)){
            GRRLIB_DrawImg(CenterX(img), CenterY(img), img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); 
        }
        t2 = time(NULL);
        if(t2 - t1 >= 2 || isClicked){
            GRRLIB_Rectangle(0, 0, 640, 480, 0x00000000 + alpha, 1);
            if(alpha < 255){
                alpha+=3;
            }
            else{
                break;
            }
        }
        GRRLIB_Render();
        if(mote & WPAD_BUTTON_A){
            isClicked = true;
        }
    }
    GRRLIB_FreeTexture(img);
    menuSceneMain();

    return 0;
}