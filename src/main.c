#include "main.h"
u8 imgCheck(GRRLIB_texImg* img){
    if(img){
        //GRRLIB_FillScreen(0x0000FFFF);
        return 1;
    }
    else{
        GRRLIB_FillScreen(0xFF0000FF);
        return 0;
    }
}

float CenterX(GRRLIB_texImg *img){
    return ((640 - (img->w * IMG_SCALE))/2);
}
float CenterY(GRRLIB_texImg *img){
    return ((480 - (img->h * IMG_SCALE))/2);
}

void FreeImgAnim(IMG_Anim* img_a){
    for(int i = 0; i < MAX_FRAMES; i++){
        GRRLIB_FreeTexture(img_a->frames[i]);
    }
}

int main(int argc, char *argv[]){
    fatInitDefault();
    GRRLIB_Init();
    WPAD_Init();
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Render();
    if(DEBUG){
        gameSceneMain();
    }
    else{
        warningSceneMain();
    }
    GRRLIB_Exit();
    return 0;
}