#include "main.h"
#define L_PAN_W 214
#define H_PAN_W 130
#define L_PAN_SPEED 6
#define H_PAN_SPEED 8

void drawCursor(GRRLIB_texImg *img, ir_t *ir){
    GRRLIB_DrawImg(ir->x, ir->y, img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
}

void drawOffice(IMG_Anim* office, ir_t *ir, int *x, u32 *mote){
    GRRLIB_DrawImg(*x, 0, office->frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(*x+ (office->frames[1]->w), 0, office->frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    if(DEBUG){
        GRRLIB_Rectangle(0, 0, L_PAN_W, 480, 0xFF00FFFF, true); // light left pan
        GRRLIB_Rectangle(640 - L_PAN_W, 0, L_PAN_W, 480, 0xFF00FFFF, true); // ligth right pan
        GRRLIB_Rectangle(0, 0, H_PAN_W, 480, 0x680068FF, true); // heavy left pan
        GRRLIB_Rectangle(640 - H_PAN_W, 0, H_PAN_W, 480, 0x680068FF, true); // heavy right pan
    }
    // size of pans: 160x480
    if(*mote & WPAD_BUTTON_A && *x < 0 || ir->x <= L_PAN_W && ir->x >= H_PAN_W && *x < 0){
        *x+=L_PAN_SPEED;
    }
    if(*mote & WPAD_BUTTON_B && *x > -624 || ir->x >= (640-L_PAN_W) && ir->x <= (640-H_PAN_W) && *x > -624){
        *x-=L_PAN_SPEED;
    }
    if(ir->x <= H_PAN_W && *x < 0){
        *x+=H_PAN_SPEED;
    }
    if(ir->x >= (640-H_PAN_W) && *x > -624){
        *x-=H_PAN_SPEED;
    }
    if(*x <= -624){
        *x = -624;
    }
    if(*x >= 0){
        *x = 0;
    }
}

int gameSceneMain(){
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Render();
    u64 frame = 0;
    IMG_Anim office = {0};
    office.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/203(1).png");
    office.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/203(2).png");
    office.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/204(1).png");
    office.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/204(2).png");
    GRRLIB_texImg *cursor = GRRLIB_LoadTexture(pointer_png);
    GRRLIB_texImg *font = GRRLIB_LoadTexture(font_png);
    GRRLIB_InitTileSet(cursor, 32, 32, 32);
    GRRLIB_InitTileSet(font, 32, 36, 32);
    char buffer[100];
    int x = 0;
    ir_t ir;
    while (1)
    {
        WPAD_ScanPads();
        u32 mote = WPAD_ButtonsHeld(0);
        WPAD_IR(0, &ir);
        drawOffice(&office, &ir, &x, &mote);
        if(DEBUG){
            sprintf(buffer, "X: %d \n mX: %.1f, mY: %.1f", x, ir.x, ir.y);
            GRRLIB_Printf(10, 10, font, 0xFFFFFFFF, .4, buffer);
        }
        drawCursor(cursor, &ir);
        GRRLIB_Render();
        frame++;
    }
    FreeImgAnim(&office);
    return 0;
}