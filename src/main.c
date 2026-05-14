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
        img_a->frames[i] = NULL;
    }
}

void FreeAllImgAnim2(IMG_Anim2* img_a){
    for(int i = 0; i < MAX_FRAMES; i++){
        if(img_a->loaded[i] != false){
            GRRLIB_FreeTexture(img_a->frames[i]);
            img_a->frames[i] = NULL;
        }
        img_a->loaded[i] = false;
    }    
}

void FreeOneImgAnim2(IMG_Anim2* img_a, u8 i){
    GRRLIB_FreeTexture(img_a->frames[i]);
    img_a->frames[i] = NULL;
    img_a->loaded[i] = false;
}

u8* loadBuf(const char* path, s32 *sizeOut)
{
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    rewind(f);

    u8 *buf = (u8*)memalign(32, size);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    fread(buf, 1, size, f);
    fclose(f);

    *sizeOut = size;
    return buf;
}


int main(int argc, char *argv[]){
    fatInitDefault();
    GRRLIB_Init();
    WPAD_Init();
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Render();
    gameSceneMain();
    //warningSceneMain();
    //menuSceneMain();
    //adSceneMain();
    GRRLIB_Exit();
    return 0;
}