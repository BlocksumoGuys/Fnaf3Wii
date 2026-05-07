#include "main.h"

IMG_Anim green_lines = {0};

void createStatic(IMG_Anim *static_spr, u8 *static_f){
    if(static_spr->frames[*static_f]){
        GRRLIB_DrawImg(CenterX(static_spr->frames[*static_f]), CenterY(static_spr->frames[*static_f]), static_spr->frames[*static_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + ((rand() % 50)));
        *static_f = (*static_f + 1) % 6;
    }
}

void createTitle(GRRLIB_texImg* img){
    GRRLIB_DrawImg(63, 37, img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
}

void createNewGame(GRRLIB_texImg* img){
    GRRLIB_DrawImg(60, 282, img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
}

void createLoadGame(GRRLIB_texImg* img){
    GRRLIB_DrawImg(60, 327, img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
}

void createCursor(GRRLIB_texImg* img, u8 *cursor_f, u64 *frame){
    const u64 toggleRate = 12;

    bool visible = ((*frame / toggleRate) % 2) == 0;
    if(visible){
        if(*cursor_f == 0){
            GRRLIB_DrawImg(33, 282, img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }
        else if(*cursor_f == 1){
            GRRLIB_DrawImg(33, 327, img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }
    }
}

void createBlipFlash(IMG_Anim *img, u8 *altA, u8 *counter, u64 *frame){
    u8 alp = 0;
    if(((*frame / 60) % 2) == 0){
        alp = 255 - ((rand() % 55) + 200);
    }
    if(((*frame / 18) % 2) == 0){
        *altA = rand() % 3;
    }
    if(*altA == 1){
        GRRLIB_DrawImg(CenterX(img->frames[*counter]), CenterY(img->frames[*counter]), img->frames[*counter], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + alp);
    }
    *counter = (*counter+1) % 7;
}

void createGreenLines(IMG_Anim *img, u8 *counter, u8 *random, u64 *frame, s32 x, s32 y){
    if(*frame%60==0){
        *random = (rand() % 10) + 2;
    }
    GRRLIB_DrawImg(x, y, green_lines.frames[*counter], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    if(*frame%(*random * 10) == 0){
        *counter = (*counter+1) % 16;
    }
}

void createGreenScan(GRRLIB_texImg *img, s32 *y, u64 *frame){
    GRRLIB_DrawImg(0, *y, img, 0, IMG_SCALE, IMG_SCALE * 0.9, 0xFFFFFF55);
    if(*frame%3==0){
        *y = (*y + 1) % 480;
    }
}

void createScottCredit(GRRLIB_texImg* img){
    GRRLIB_DrawImg(522, 463, img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
}

void createGameVer(GRRLIB_texImg* img){
    GRRLIB_DrawImg(572, 441, img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
}

void createSpringTrap(IMG_Anim *springtrap_spr, u8 *springtrap_f, u64 *frame, u8 *altA, u8 *altB){
        if(*frame % 60 == 0){
            *altB = rand() % 5;
        }
        if(*altB == 1){
            *altA = rand() % 5;
        }
        else{
            *altA = 5;
        }
        switch (*altA)
        {
            case 0:
                *springtrap_f = 1;
                break;
            case 1:
                *springtrap_f = 2;
                break;
            case 2:
                *springtrap_f = 3;
                break;
            default:
                *springtrap_f = 0;
                break;
        }
        GRRLIB_DrawImg(CenterX(springtrap_spr->frames[*springtrap_f]), CenterY(springtrap_spr->frames[*springtrap_f]), springtrap_spr->frames[*springtrap_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
}

typedef struct
{
    u8 random;
    u8 counter;
}GreenLine_t;

GreenLine_t createGreenLineObj(){
    GreenLine_t gl = {0};
    gl.random = 60;
    return gl;
}


int menuSceneMain(){
    u64 frame = 0;
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Render();
    IMG_Anim static_spr = {0};
    static_spr.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/291.png"); // static
    static_spr.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/292.png");
    static_spr.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/33.png");
    static_spr.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/34.png");
    static_spr.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/35.png");
    static_spr.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/36.png"); // static
    u8 static_f = 0;

    IMG_Anim springtrap_spr = {0};
    springtrap_spr.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/862.png"); // spring
    springtrap_spr.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/855.png");
    springtrap_spr.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/864.png");
    springtrap_spr.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/859.png");
    springtrap_spr.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/861.png"); // spring
    u8 springtrap_f = 3;

    IMG_Anim blipflash_spr = {0};
    blipflash_spr.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/869.png"); // blip flash
    blipflash_spr.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/863.png");
    blipflash_spr.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/871.png");
    blipflash_spr.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/865.png");
    blipflash_spr.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/866.png");
    blipflash_spr.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/867.png");
    blipflash_spr.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/868.png"); // blip flash
    u8 blipflash_altA = 0;
    u8 blipflash_f = 0;

    GRRLIB_texImg *greenScanBar = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1126.png"); // big green scan bar
    s32 greenScanBarY = -20;

    green_lines.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1028.png");
    green_lines.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1029.png");
    green_lines.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1030.png");
    green_lines.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1031.png");
    green_lines.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1032.png");
    green_lines.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1033.png");
    green_lines.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1034.png");
    green_lines.frames[7] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1035.png");
    green_lines.frames[8] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1036.png");
    green_lines.frames[9] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1037.png");
    green_lines.frames[10] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1038.png");
    green_lines.frames[11] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1039.png");
    green_lines.frames[12] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1040.png");
    green_lines.frames[13] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1041.png");
    green_lines.frames[14] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1042.png");
    green_lines.frames[15] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/1043.png");

    GreenLine_t green_line1 = createGreenLineObj();
    GreenLine_t green_line2 = createGreenLineObj();
    GreenLine_t green_line3 = createGreenLineObj();
    GreenLine_t green_line4 = createGreenLineObj();
    GreenLine_t green_line5 = createGreenLineObj();
    GreenLine_t green_line6 = createGreenLineObj();
    GreenLine_t green_line7 = createGreenLineObj();
    GreenLine_t green_line8 = createGreenLineObj();
    GreenLine_t green_line9 = createGreenLineObj();
    GreenLine_t green_line10 = createGreenLineObj();
    GreenLine_t green_line11 = createGreenLineObj();

    GRRLIB_texImg *title = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/155.png");
    GRRLIB_texImg *new_game = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/592.png");
    GRRLIB_texImg *load_game = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/301.png");
    GRRLIB_texImg *cursor = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/833.png");
    GRRLIB_texImg *scott_credits = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/849.png");
    GRRLIB_texImg *game_ver = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/160.png"); // original game version
    u8 altA = 0;
    u8 altB = 0;
    u8 cursor_f = 0;
    bool pastCursor = false;
    bool isQuitting = false;
    srand(time(NULL));
    while(1){
        WPAD_ScanPads();
        u32 mote = WPAD_ButtonsDown(0);
        createSpringTrap(&springtrap_spr, &springtrap_f, &frame, &altA, &altB);
        createGreenLines(&green_lines, &green_line1.counter, &green_line1.random, &frame, 0, 50);
        createGreenLines(&green_lines, &green_line2.counter, &green_line2.random, &frame, 0, 80);
        createGreenLines(&green_lines, &green_line3.counter, &green_line3.random, &frame, 0, 130);
        createGreenLines(&green_lines, &green_line4.counter, &green_line4.random, &frame, 0, 160);
        createGreenLines(&green_lines, &green_line5.counter, &green_line5.random, &frame, 0, 190);
        createGreenLines(&green_lines, &green_line6.counter, &green_line6.random, &frame, 0, 282);
        createGreenLines(&green_lines, &green_line7.counter, &green_line7.random, &frame, 0, 327);
        createGreenLines(&green_lines, &green_line8.counter, &green_line8.random, &frame, 0, 361);
        createGreenLines(&green_lines, &green_line9.counter, &green_line9.random, &frame, 0, 413);
        createGreenLines(&green_lines, &green_line10.counter, &green_line10.random, &frame, 300, 441);
        createGreenLines(&green_lines, &green_line11.counter, &green_line11.random, &frame, 300, 462);
        createTitle(title);
        createNewGame(new_game);
        createLoadGame(load_game);
        createGreenScan(greenScanBar, &greenScanBarY, &frame);
        if(mote & WPAD_BUTTON_DOWN || mote & WPAD_BUTTON_UP){
            cursor_f = !cursor_f;
        }
        if(mote & WPAD_BUTTON_A){
            isQuitting = false;
            break;
        }
        createCursor(cursor, &cursor_f, &frame);
        if(cursor_f != pastCursor){
            pastCursor = cursor_f; // for future audio
        }
        createScottCredit(scott_credits);
        createBlipFlash(&blipflash_spr, &blipflash_altA, &blipflash_f, &frame);
        createStatic(&static_spr, &static_f);
        createGameVer(game_ver);
        frame++;
        GRRLIB_Render();
        if(mote & WPAD_BUTTON_HOME){
            isQuitting = true;
            break;
        }
    };
    GRRLIB_FreeTexture(title);
    GRRLIB_FreeTexture(new_game);
    GRRLIB_FreeTexture(load_game);
    GRRLIB_FreeTexture(cursor);
    GRRLIB_FreeTexture(scott_credits);
    GRRLIB_FreeTexture(greenScanBar);
    FreeImgAnim(&blipflash_spr);
    FreeImgAnim(&static_spr);
    FreeImgAnim(&springtrap_spr);
    FreeImgAnim(&green_lines);
    if(!isQuitting){
        if(cursor_f == 0){
            adSceneMain();
        }
        else{
            curr_nightSceneMain();
        }
    }
    return 0;
}