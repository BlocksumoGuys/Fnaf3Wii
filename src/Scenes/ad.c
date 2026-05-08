#include "main.h"

int adSceneMain(){
    u64 frame = 0;
    //GRRLIB_FillScreen(0x000000FF);
    //GRRLIB_Render();
    GRRLIB_texImg *ad_img = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/0.png");
    u8 alpha = 0;
    bool isAlpha = true;
    while (1)
    {
        WPAD_ScanPads();
        u32 mote = WPAD_ButtonsDown(0);
        GRRLIB_DrawImg(0, 0, ad_img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + alpha);
        if(alpha < 255 && isAlpha){
            alpha+=3;
        }
        if(frame >= (60*9) || (mote & WPAD_BUTTON_A)){
            isAlpha = false;
        }
        if(!isAlpha && alpha > 0){
            alpha-=3;
        }
        if(alpha == 0 && !isAlpha){
            break;
        }
        GRRLIB_Render();
        frame++;
    }
    GRRLIB_FreeTexture(ad_img);
    curr_nightSceneMain();
    return 0;
}