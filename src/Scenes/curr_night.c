#include "main.h"

void createBlipFlashOnce(IMG_Anim *img, u8 *counter){
    if(*counter >= 8){return;}
    GRRLIB_DrawImg(0, 0, img->frames[*counter], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    *counter+=1;
}

void createNightNmb(IMG_Anim *img, u8 *alpha){
    GRRLIB_DrawImg(CenterX(img->frames[NIGHT]), CenterY(img->frames[NIGHT]), img->frames[NIGHT], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + *alpha);
}

int curr_nightSceneMain(){
    u64 frame = 0;
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Render();
    IMG_Anim blipflash_spr = {0};
    blipflash_spr.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/869.png"); // blip flash
    blipflash_spr.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/863.png");
    blipflash_spr.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/871.png");
    blipflash_spr.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/865.png");
    blipflash_spr.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/866.png");
    blipflash_spr.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/867.png");
    blipflash_spr.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/868.png"); // blip flash
    u8 blipflash_f = 0;

    IMG_Anim night_spr = {0};
    night_spr.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1098.png");
    night_spr.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/837.png");
    night_spr.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/838.png");
    night_spr.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/842.png");
    night_spr.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/843.png");
    night_spr.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/844.png");
    u8 alpha = 255;
    while(1){
        WPAD_ScanPads();
        u32 mote = WPAD_ButtonsDown(0);

        createNightNmb(&night_spr, &alpha);
        if(frame >= 160){
            if(alpha > 0) {alpha-=3;}
            else{break;}
        }
        createBlipFlashOnce(&blipflash_spr, &blipflash_f);

        if(mote & WPAD_BUTTON_A){
            break;
        }
        GRRLIB_Render();
        frame++;
    }
    FreeImgAnim(&blipflash_spr);
    FreeImgAnim(&night_spr);
    gameSceneMain();
    return 0;
}