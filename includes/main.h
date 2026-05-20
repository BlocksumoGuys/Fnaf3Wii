#pragma once

#define IMG_SCALE 1
#define MAX_FRAMES 34
#define DEBUG false

#define WARNING_IMG "sd:/fnaf3/assets/images/329.png"
#define SD_SAVE "sd:/fnaf3/fnaf3save.save"
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
#include <asndlib.h>
#include <mp3player.h>
#include <malloc.h>
#include "SoundManager.h"
#include "warningScene.h"
#include "menu.h"
#include "ad.h"
#include "curr_night.h"
#include "game.h"
#include "gameover.h"
#include "next_day.h"

#include "font_png.h"
#include "pointer_png.h"

u8 imgCheck(GRRLIB_texImg* img);
float CenterX(GRRLIB_texImg *img);
float CenterY(GRRLIB_texImg *img);
u8* loadBuf(const char* path, s32* sizeOut);

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
GRRLIB_texImg *GRRLIB_FakeLoad(const char* path);
char* read_line(const char *path, int target_line);
void write_line(const char *path, int target_line, const char *new_text);