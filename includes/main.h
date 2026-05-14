#pragma once

#define IMG_SCALE 1
#define MAX_FRAMES 30
#define NIGHT 6
#define DEBUG false

#define WARNING_IMG "sd:/fnaf3/assets/images/329.png"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fat.h>
#include <ogcsys.h>
#include <ogc/lwp_watchdog.h>
#include <gctypes.h>
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <string.h>

#include "warningScene.h"
#include "menu.h"
#include "ad.h"
#include "curr_night.h"
#include "game.h"

#include "font_png.h"
#include "pointer_png.h"

u8 imgCheck(GRRLIB_texImg* img);
float CenterX(GRRLIB_texImg *img);
float CenterY(GRRLIB_texImg *img);
u8* loadBuf(const char* path, s32 *sizeOut);

typedef struct
{
    GRRLIB_texImg *frames[MAX_FRAMES];
    bool loaded;
}IMG_Anim;

typedef struct
{
    GRRLIB_texImg *frames[MAX_FRAMES];
    bool loaded[MAX_FRAMES];
}IMG_Anim2;


void FreeImgAnim(IMG_Anim* img_a);
void FreeAllImgAnim2(IMG_Anim2* img_a);
void FreeOneImgAnim2(IMG_Anim2* img_a, u8 i);