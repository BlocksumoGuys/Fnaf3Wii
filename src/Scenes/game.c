#include "main.h"
#define L_PAN_W 214
#define H_PAN_W 130
#define PAN_SPEED 12
#define HALF_REGION 624
#define SCREEN_W 640
#define SCREEN_H 480
#define CONTROL_PANEL_X 266
#define CONTROL_PANEL_Y 453
#define STRIP_W 4
float x = 0;
float tx = 0.0f;
ir_t ir;
u8 isControlPanelOpen = 0;
bool PanelButtonVisible = true;
bool canPanelButtonBeClicked = true;
bool canPan = true;
bool isCamOpen = false;
bool camLayoutSelection = 0; // 0 is normal, 1 is vent 
bool hallucinating = false;
bool ScreenOpen = false;
bool stage3scare = false;
bool IsCurrentlyClosingPanel = false;
bool isJumpscare = false;
bool isSealing = false;
bool isVentCamPressed = false;
bool audioError = false;
bool videoError = false;
bool ventilationError = false;
bool isRebootingCam = false;
bool isRebootingAudio = false;
bool isRebootingVent = false;
bool isRebootingAll = false;
bool isRebooting = false;
bool IsBBJumpscaring = false;
bool hasBBSpawned = false;
u8 panelSelect = 0;
float IR_Y;
u8 time_am = 12;
u64 frame = 0;
u32 mote;
IMG_Anim office = {0};
IMG_Anim fan = {0};
u8 fan_f = 0;
IMG_Anim controlPanel = {0}; // the physical panel
u8 controlPanel_f = 0;
IMG_Anim PanelCommands = {0}; // the commands
IMG_Anim NightTimeFont = {0};
IMG_Anim CamPanel = {0};
IMG_Anim triangle = {0};
u8 CamPanel_f = 0;
u8 CamPanel_alpha = 50;
IMG_Anim CamLayout = {0};
u8 CamLayout_f = 0;
IMG_Anim CamLayoutNmb = {0};
IMG_Anim CamStatic = {0};
u8 CamStatic_alpha = 50;
IMG_Anim white_blip = {0};
u8 blip_f = 10;
IMG_Anim2 neutral_cams = {0};
IMG_Anim2 springtrap_cams = {0};
IMG_Anim backdrop = {0};
IMG_Anim springtrap_attack = {0};
IMG_Anim springtrap_jumpscare = {0};
IMG_Anim loadGreen = {0};
IMG_Anim BB = {0};
GRRLIB_texImg *cursor = NULL;
GRRLIB_texImg *font = NULL;
GRRLIB_texImg *panelClick = NULL;
GRRLIB_texImg *camClick = NULL;
GRRLIB_texImg *sceneTex = NULL;
u8 camButton_alpha = 100;
u8 selected_cam = 2;
u8 picrandom;
int mon_cam = 0;
const u8 Springtrap_AI[6] = {0, 2, 3, 4, 5, 7}; // for last, 7 instead of 10
const u8 BB_Ai[6] = {90, 80, 70, 60, 120}; // 50 for lasr
int camHealth = 10;
int ventHealth = 10;
int audioHealth = 10;
int camProgress = 0;
int ventProgress = 0;
int audioProgress = 0;
int allProgress = 0;
u64 CamErrorTimer = 0;
u64 VentErrorTimer = 0;
bool MapLayoutNormalMode = false;

int move_counter = 0; // for sprintrap ai
bool aggresive = false;
u8 action_selected = 0;
u8 attack_stage = 0;
u8 total_turns = 0; // for springtrap ai

u8 blackout_alpha = 0;
u8 closed_vent = 0;
u64 double_click_timer_vent = 0;
u8 seal_f = 0;
int BBSpawn = -1;
u64 BBcounter = 0;
u8 BBAlpha = 255;
u64 OfficeIdle = 0;

const char *sp_cams_arr[24] = {
    "sd:/fnaf3/assets/images/295.png",
    "sd:/fnaf3/assets/images/132.png",
    "sd:/fnaf3/assets/images/133.png",
    "sd:/fnaf3/assets/images/121.png",
    "sd:/fnaf3/assets/images/122.png",
    "sd:/fnaf3/assets/images/119.png",
    "sd:/fnaf3/assets/images/120.png",
    "sd:/fnaf3/assets/images/117.png",
    "sd:/fnaf3/assets/images/118.png",
    "sd:/fnaf3/assets/images/124.png",
    "sd:/fnaf3/assets/images/125.png",
    "sd:/fnaf3/assets/images/126.png",
    "sd:/fnaf3/assets/images/125.png",
    "sd:/fnaf3/assets/images/127.png",
    "sd:/fnaf3/assets/images/128.png",
    "sd:/fnaf3/assets/images/129.png",
    "sd:/fnaf3/assets/images/128.png",
    "sd:/fnaf3/assets/images/130.png",
    "sd:/fnaf3/assets/images/131.png",
    "sd:/fnaf3/assets/images/829.png",
    "sd:/fnaf3/assets/images/831.png",
    "sd:/fnaf3/assets/images/835.png",
    "sd:/fnaf3/assets/images/841.png",
    "sd:/fnaf3/assets/images/832.png"
};

const char *nt_cams_arr[24] = {
    "sd:/fnaf3/assets/images/106.png", // 106
    "sd:/fnaf3/assets/images/97.png", // 97
    "sd:/fnaf3/assets/images/103.png", // 103
    "sd:/fnaf3/assets/images/104.png", // 104
    "sd:/fnaf3/assets/images/105.png", // 105
    "sd:/fnaf3/assets/images/109.png", // 109
    "sd:/fnaf3/assets/images/110.png", // 110
    "sd:/fnaf3/assets/images/98.png", // 98
    "sd:/fnaf3/assets/images/99.png", // 99
    "sd:/fnaf3/assets/images/100.png", // 100
    "sd:/fnaf3/assets/images/101.png", // 101
    "sd:/fnaf3/assets/images/102.png", // 102
    "sd:/fnaf3/assets/images/101.png", // 101
    "sd:/fnaf3/assets/images/112.png", // 112
    "sd:/fnaf3/assets/images/113.png", // 113
    "sd:/fnaf3/assets/images/114.png", // 114
    "sd:/fnaf3/assets/images/113.png", // 113
    "sd:/fnaf3/assets/images/115.png", // 115
    "sd:/fnaf3/assets/images/116.png", // 116
    "sd:/fnaf3/assets/images/623.png",
    "sd:/fnaf3/assets/images/624.png",
    "sd:/fnaf3/assets/images/628.png",
    "sd:/fnaf3/assets/images/824.png",
    "sd:/fnaf3/assets/images/818.png",
};

bool IsCursorWithinImg(GRRLIB_texImg *img, int x, int y){
    if(ir.x >= x && ir.x <= x + img->w && IR_Y >= y && IR_Y <= y + img->h){return true;}
    return false;
}

void DrawWarpedScene(){
    for(int sx = 0; sx < SCREEN_W; sx += STRIP_W){
        float u = (float)sx / SCREEN_W;
        float dist = powf(fabsf(u - 0.5f) * 2.0f, 2.0f);
        float scaleY = 1.0f + dist * 0.2f; // for intensity
        float h = SCREEN_H * scaleY;
        float y = (SCREEN_H - h) * 0.5f;
        GRRLIB_DrawPart(sx, y, sx, 0, STRIP_W, SCREEN_H, sceneTex, 0, 1.0f, scaleY, 0xFFFFFFFF);
    }
}

GRRLIB_texImg *GRRLIB_FakeLoad(const char* path){
    GRRLIB_texImg *tex_img = GRRLIB_LoadTextureFromFile(path);
    if(tex_img){
        return tex_img;
    }else{time_am = 6; return NULL;}
}

void InitAllImg(){
    office.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/203(1).png"); // normal 1
    office.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/203(2).png"); // normal 2
    office.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/204(1).png"); // alarm 1
    office.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/204(2).png"); // alarm 2
    fan.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/18.png");
    fan.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/19.png");
    fan.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/20.png");
    PanelCommands.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/599.png"); // "system restart menu"
    PanelCommands.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/544.png"); // "audio devices"
    PanelCommands.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/545.png"); // "camera system"
    PanelCommands.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/555.png"); // "ventilation"
    PanelCommands.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/594.png"); // "reboot all"
    PanelCommands.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/600.png"); // "exit"
    PanelCommands.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/596.png"); // green select cursor
    PanelCommands.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/601.png"); // error
    NightTimeFont.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/746.png"); // 0
    NightTimeFont.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/747.png"); // 1
    NightTimeFont.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/748.png"); // 2
    NightTimeFont.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/749.png"); // 3
    NightTimeFont.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/750.png"); // 4
    NightTimeFont.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/751.png"); // 5
    NightTimeFont.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/752.png"); // 6
    NightTimeFont.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/245.png"); // "night" text
    NightTimeFont.frames[8] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/246.png"); // "am" text
    CamPanel.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/74.png"); // normal grey boxes
    CamPanel.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/72.png"); // cam num
    CamPanel.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/71.png");
    CamPanel.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/69.png");
    CamPanel.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/68.png");
    CamPanel.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/63.png");
    CamPanel.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/62.png");
    CamPanel.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/61.png");
    CamPanel.frames[8] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/60.png");
    CamPanel.frames[9] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/59.png");
    CamLayout.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/532.png"); // cam layout
    CamLayout.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/626.png"); // vent layout
    CamLayout.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/574.png"); // cam layout 2
    CamLayout.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/627.png"); // vent layout 2
    triangle.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/95.png");
    triangle.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/93.png");
    triangle.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/96.png");
    triangle.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/94.png");
    CamStatic.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/291.png");
    CamStatic.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/292.png");
    CamStatic.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/33.png");
    white_blip.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/150.png");
    white_blip.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/151.png");
    white_blip.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/152.png");
    white_blip.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/153.png");
    white_blip.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/154.png");
    backdrop.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/205(1).png");
    backdrop.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/205(2).png");
    cursor = GRRLIB_LoadTexture(pointer_png);
    font = GRRLIB_LoadTexture(font_png);
    panelClick = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/77.png");
    camClick = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/32.png");
    GRRLIB_InitTileSet(cursor, 32, 32, 32);
    GRRLIB_InitTileSet(font, 32, 36, 32);

    controlPanel.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/744.png");
    controlPanel.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/743.png");
    controlPanel.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/742.png");
    controlPanel.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/741.png");
    controlPanel.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/740.png");
    controlPanel.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/739.png");
    controlPanel.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/738.png");
    controlPanel.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/737.png");
    controlPanel.frames[8] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/736.png");
    controlPanel.frames[9] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/735.png");
    controlPanel.frames[10] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/734.png");

    springtrap_jumpscare.frames[0] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/792.png");
    springtrap_jumpscare.frames[1] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/794.png");
    springtrap_jumpscare.frames[2] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/796.png");
    springtrap_jumpscare.frames[3] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/798.png");
    springtrap_jumpscare.frames[4] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/800.png");
    springtrap_jumpscare.frames[5] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/802.png");
    springtrap_jumpscare.frames[6] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/804.png");
    springtrap_jumpscare.frames[7] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/806.png");
    springtrap_jumpscare.frames[8] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/825.png");
    springtrap_jumpscare.frames[9] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/822.png");
    springtrap_jumpscare.frames[10] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/820.png");
    springtrap_jumpscare.frames[11] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/817.png");
    springtrap_jumpscare.frames[12] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/815.png");
    springtrap_jumpscare.frames[13] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/813.png");
    springtrap_jumpscare.frames[14] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/811.png");
    springtrap_jumpscare.frames[15] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/809.png");

    loadGreen.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/611.png"); // seal green bean
    loadGreen.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/612.png");
    loadGreen.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/629.png");
    loadGreen.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/632.png");
    loadGreen.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/637.png"); // seal green bean

    BB.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/70.png"); // bb on cam
    BB.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/67.png"); // in ofice idle
    BB.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/328.png"); // jumpscare
    BB.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/338.png");
    BB.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/341.png");
    BB.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/344.png");
    BB.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/349.png");
    BB.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/342.png");
    BB.frames[8] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/343.png");
    BB.frames[9] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/345.png");
    BB.frames[10] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/347.png");
    BB.frames[11] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/667.png"); // jumpscare (also white flash)
    BB.loaded = true;
}

void dcv(int v, bool mode){
    static int cur_cam = -1;
    static bool cur_mode = -1;

    IMG_Anim2* set = mode ? &neutral_cams : &springtrap_cams;
    const char** src = mode ? nt_cams_arr : sp_cams_arr;

    if(v < 0 || v >= MAX_FRAMES) return;
    int cam_id = selected_cam;

    if(cur_cam != cam_id || cur_mode != mode){
        FreeAllImgAnim2(&springtrap_cams);
        FreeAllImgAnim2(&neutral_cams);

        cur_cam = cam_id;
        cur_mode = mode;
    }

    if(!set->frames[v]){
        set->frames[v] = GRRLIB_LoadTextureFromFile(src[v]);
        set->loaded[v] = (set->frames[v] != NULL);
    }
}

void drawCams(){
    //SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68
    // CAM2: 2
    // CAM5: 2
    // CAM6: 2
    // CAM7: 4
    // CAM8: 4
    static u8 cam2_f = 0;
    static u8 cam5_f = 0;
    static u8 cam6_f = 0;
    static u8 cam7_f = 0;
    static u8 cam8_f = 0;
    if(camButton_alpha != 50 || videoError){
        return;
    };
    u8 v = 0;
    switch (selected_cam)
    {
    case 2:
        v = cam2_f + 1;
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
    case 5:
        v = cam5_f + 5;
        if(selected_cam == mon_cam){          
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break; 
    case 6:
        v = cam6_f + 7;
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
    case 7:
        v = cam7_f + 9;
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
    case 8:
        v = cam8_f + 13;
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
        
    case 1:
        v = 0;
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for non-anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
    case 9:
        v = 17;
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for non-anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
    case 10:
        v = 18;
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for non-anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
    case 3:
        v = selected_cam;
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for non-anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
    case 4:
        v = selected_cam;
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for non-anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
    default:
        v = selected_cam + 8;
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for vents iirc
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
    }
    //GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, BB.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); for BB
    if(frame%25==0){
        cam2_f = (cam2_f + 1) % 2;
    }
    if(frame%4==0){
        cam5_f = (cam5_f + 1) % 2;
        cam6_f = (cam6_f + 1) % 2;
    }
    if(frame%6==0){
        cam7_f = (cam7_f + 1) % 4;
        cam8_f = (cam8_f + 1) % 4;
    }
}

void drawNightInf(){
    GRRLIB_DrawImg(570, 20, NightTimeFont.frames[7], 0, IMG_SCALE + .4, IMG_SCALE + .4, 0xFFFFFFFF);
    GRRLIB_DrawImg(570+50, 20-2.5, NightTimeFont.frames[NIGHT], 0, .6, .6, 0xFFFFFFFF);
    u16 tx = 590;
    u16 ty = 35;
    GRRLIB_DrawImg(612, ty + 5, NightTimeFont.frames[8], 0, IMG_SCALE + .4, IMG_SCALE + .4, 0xFFFFFFFF);
    if(time_am!=12){
        GRRLIB_DrawImg(tx, ty, NightTimeFont.frames[time_am], 0, .6, .6, 0xFFFFFFFF);
    }
    else{
        GRRLIB_DrawImg(tx, ty, NightTimeFont.frames[1], 0, .6, .6, 0xFFFFFFFF);
        GRRLIB_DrawImg(tx + (NightTimeFont.frames[1]->w/2), ty, NightTimeFont.frames[2], 0, .6, .6, 0xFFFFFFFF);        
    }
}

void drawBlip(){
    GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, white_blip.frames[blip_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    if(blip_f<4 && frame%2==0){
        blip_f++;
    }
}

void drawCursor(){
    float y = ir.y * (480.0f / 460.0f);
    GRRLIB_DrawImg(ir.x, y, cursor, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
}

void drawFan(){
    GRRLIB_DrawImg(x+HALF_REGION + 100, 180, fan.frames[fan_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    if(frame%2==0){
        fan_f = (fan_f + 1) % 3;
    }
}

void drawControlPanel(){
    if(isControlPanelOpen == 1){
        GRRLIB_DrawImg(10, SCREEN_H - (controlPanel.frames[controlPanel_f]->h) + 5, controlPanel.frames[controlPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        if(frame%2==0){
            if(controlPanel_f!= 10){
                controlPanel_f+=1;
                canPanelButtonBeClicked = false;           
            }
            else{PanelButtonVisible = false; ScreenOpen = true; isControlPanelOpen = true;}
        }
    }
    else if(isControlPanelOpen == 2){
        GRRLIB_DrawImg(10, SCREEN_H - (controlPanel.frames[controlPanel_f]->h) + 5, controlPanel.frames[controlPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        PanelButtonVisible = true;
        canPanelButtonBeClicked = false;
        if(frame%2==0){
            if(controlPanel_f!=0){
                controlPanel_f-=1;
                IsCurrentlyClosingPanel = true;
            }
            else{
                canPanelButtonBeClicked = true;
                isControlPanelOpen = false;
                ScreenOpen = false;
                IsCurrentlyClosingPanel = false;
            }
        }
    }
}

void drawPanelButton(){
    if(fabs(x)<1.0f){
        if(!isControlPanelOpen) canPanelButtonBeClicked = true;
        if(PanelButtonVisible){
            GRRLIB_DrawImg(CONTROL_PANEL_X - (panelClick->w / 2), CONTROL_PANEL_Y - (panelClick->h), panelClick, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + 127);
        }else{
            GRRLIB_DrawImg(CONTROL_PANEL_X - (panelClick->w / 2), CONTROL_PANEL_Y - (panelClick->h), panelClick, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00);
        }
    }
    else canPanelButtonBeClicked = false;
}

void drawOffice(){
    if(!ventilationError){
        GRRLIB_DrawImg(x, 0, office.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(x+(office.frames[1]->w), 0, office.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    }
    else{
        if((frame/15)%2==0){
            GRRLIB_DrawImg(x, 0, office.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
            GRRLIB_DrawImg(x+(office.frames[1]->w), 0, office.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);            
        }else{
            GRRLIB_DrawImg(x, 0, office.frames[2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
            GRRLIB_DrawImg(x+(office.frames[3]->w), 0, office.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }
    }
    if(DEBUG){
        GRRLIB_Rectangle(0, 0, L_PAN_W, 480, 0xFF00FFFF, true); // light left pan
        GRRLIB_Rectangle(640 - L_PAN_W, 0, L_PAN_W, 480, 0xFF00FFFF, true); // ligth right pan
        GRRLIB_Rectangle(0, 0, H_PAN_W, 480, 0x680068FF, true); // heavy left pan
        GRRLIB_Rectangle(640 - H_PAN_W, 0, H_PAN_W, 480, 0x680068FF, true); // heavy right pan
    }
    if(!canPan) return;
    if(x < 0 && ir.x <= L_PAN_W){
        x+=PAN_SPEED;
    }
    if(x >= -HALF_REGION && ir.x > (SCREEN_W - L_PAN_W)){
        x-=PAN_SPEED;
    }
    if(x < 0 && ir.x <= H_PAN_W){
        x+=(PAN_SPEED/2);
    }
    if(x >= -HALF_REGION && ir.x > (SCREEN_W - H_PAN_W)){
        x-=(PAN_SPEED/2);
    }

    if(x <= -HALF_REGION){
        x = -HALF_REGION;
    }
    if(x >= 0){
        x = 0;
    }
}

void drawPanelText(){
    if(PanelButtonVisible) return;
    GRRLIB_DrawImg(42, 162, PanelCommands.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(85, 235, PanelCommands.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(85, 273, PanelCommands.frames[2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(85, 306, PanelCommands.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(85, 373, PanelCommands.frames[4], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(85, 404, PanelCommands.frames[5], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    u8 y_pad = 4;
    u8 x_pad = 10;
    if(IsCursorWithinImg(PanelCommands.frames[1], 85, 235)) {panelSelect = 0;}
    if(IsCursorWithinImg(PanelCommands.frames[2], 85, 273)) {panelSelect = 1;}
    if(IsCursorWithinImg(PanelCommands.frames[3], 85, 306)) {panelSelect = 2;}
    if(IsCursorWithinImg(PanelCommands.frames[4], 85, 373)) {panelSelect = 3;}
    if(IsCursorWithinImg(PanelCommands.frames[5], 85, 404)) {panelSelect = 4;}
    if((frame/15)%2==0){
        if(audioError && !isRebootingAudio){
            GRRLIB_DrawImg(85+x_pad+180, 235+y_pad, PanelCommands.frames[7], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }
        if(videoError && !isRebootingCam){
            GRRLIB_DrawImg(85+x_pad+180, 273+y_pad, PanelCommands.frames[7], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }
        if(ventilationError && !isRebootingVent){
            GRRLIB_DrawImg(85+x_pad+180, 306+y_pad, PanelCommands.frames[7], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }
    }
    if(isRebooting) return;
    switch (panelSelect)
    {
    case 0:
        GRRLIB_DrawImg(66-PanelCommands.frames[6]->w + x_pad, 235 + y_pad, PanelCommands.frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    case 1:
        GRRLIB_DrawImg(66-PanelCommands.frames[6]->w + x_pad, 273 + y_pad, PanelCommands.frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    case 2:
        GRRLIB_DrawImg(66-PanelCommands.frames[6]->w + x_pad, 306 + y_pad, PanelCommands.frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    case 3:
        GRRLIB_DrawImg(66-PanelCommands.frames[6]->w + x_pad, 373 + y_pad, PanelCommands.frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    case 4:
        GRRLIB_DrawImg(66-PanelCommands.frames[6]->w + x_pad, 404 + y_pad, PanelCommands.frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    default:
        GRRLIB_DrawImg(66-PanelCommands.frames[6]->w + x_pad, 235 + y_pad, PanelCommands.frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    }
}

void drawCamButton(){
    if(fabs(x + HALF_REGION) > 1.0f) return;
    GRRLIB_DrawImg(540, 73, camClick, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + camButton_alpha);
}

void drawCamPanel(){
    if(isCamOpen && !isJumpscare && !IsBBJumpscaring){
        CamPanel_alpha = 255;
        GRRLIB_DrawImg(SCREEN_W - (CamPanel.frames[CamPanel_f]->w), 60, CamPanel.frames[CamPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00+CamPanel_alpha);
        if(frame%2==0){
            if(CamPanel_f < 9){
                CamPanel_f+=1;
                camButton_alpha = 99;
            }else{camButton_alpha = 50; ScreenOpen = true;}
        }
    }else{
        FreeAllImgAnim2(&neutral_cams);
        FreeAllImgAnim2(&springtrap_cams);
        GRRLIB_DrawImg(SCREEN_W - (CamPanel.frames[CamPanel_f]->w), 60, CamPanel.frames[CamPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00+CamPanel_alpha);
        if(frame%2==0){
            if(CamPanel_f > 0){
                CamPanel_f-=1;
                camButton_alpha = 99;
            }else{camButton_alpha = 100; CamPanel_alpha = 0; ScreenOpen = false;}
        }
    }
}

void drawCamStatic(){
    if(camButton_alpha!=50) return;
    static u8 CamStatic_f = 0;
    GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, CamStatic.frames[CamStatic_f], 0, 825.0f/1021.2f, 650.0f/768.0f, 0xFFFFFF00+CamStatic_alpha);
    CamStatic_f = (CamStatic_f+1)%3;
    if(CamStatic_alpha>80){
        CamStatic_alpha-=3;
    }
}

void drawCamLayout(){
    if(camButton_alpha != 50){
        if(CamLayoutNmb.loaded){
            CamLayoutNmb.loaded = false;
            FreeImgAnim(&CamLayoutNmb);
        }
        return;
    };
    if(!CamLayoutNmb.loaded){
        CamLayoutNmb.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/81.png");
        CamLayoutNmb.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/83.png"); // ALL IS CAM NUM
        CamLayoutNmb.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/84.png");
        CamLayoutNmb.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/85.png");
        CamLayoutNmb.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/86.png");
        CamLayoutNmb.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/87.png");
        CamLayoutNmb.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/88.png");
        CamLayoutNmb.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/89.png");
        CamLayoutNmb.frames[8] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/90.png");
        CamLayoutNmb.frames[9] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/91.png");
        CamLayoutNmb.frames[10] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/92.png");
        CamLayoutNmb.frames[11] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/613.png");
        CamLayoutNmb.frames[12] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/614.png");
        CamLayoutNmb.frames[13] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/617.png");
        CamLayoutNmb.frames[14] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/619.png");
        CamLayoutNmb.frames[15] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/621.png"); // cam num
        CamLayoutNmb.frames[16] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/82.png"); // normal grey boxes but selected so its green
        CamLayoutNmb.frames[17] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/426.png"); // big grey boxes
        CamLayoutNmb.frames[18] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/366.png"); // play audio text
        CamLayoutNmb.frames[19] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/431.png"); // map toggle text
        CamLayoutNmb.frames[20] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/531.png"); // vent normal
        CamLayoutNmb.frames[21] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/609.png"); // vent sealed
        CamLayoutNmb.frames[22] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/1139.png"); // seal instruction text
        CamLayoutNmb.frames[23] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/1140.png"); // sealing text
        CamLayoutNmb.loaded = true;
    }
    if(!camLayoutSelection){
        int y_pad = 0;
        int x_pad = -30;
        u16 x_l[10] = {391 + x_pad, 525 + x_pad, 577 + x_pad, 577 + x_pad, 475 + x_pad, 387 + x_pad, 387 + x_pad, 454 + x_pad, 495 + x_pad, 562 + x_pad};
        u16 y_l[10] = {407 + y_pad, 391 + y_pad, 364 + y_pad, 324 + y_pad, 333 + y_pad, 338 + y_pad, 297 + y_pad, 283 + y_pad, 257 + y_pad, 280 + y_pad};
        if(MapLayoutNormalMode == 0){
            GRRLIB_DrawImg(356+x_pad, 222+y_pad, CamLayout.frames[camLayoutSelection], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for the layout       
        }else{GRRLIB_DrawImg(356+x_pad, 222+y_pad, CamLayout.frames[camLayoutSelection + 2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);} // for the layout
        GRRLIB_DrawImg(420+x_pad, 413+y_pad, triangle.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // triangles
        GRRLIB_DrawImg(528+x_pad, 376+y_pad, triangle.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(560+x_pad, 370+y_pad, triangle.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(560+x_pad, 330+y_pad, triangle.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        // there's no fifth one
        GRRLIB_DrawImg(414+x_pad, 344+y_pad, triangle.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(414+x_pad, 304+y_pad, triangle.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(461+x_pad, 300+y_pad, triangle.frames[2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(500+x_pad, 270+y_pad, triangle.frames[2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(547+x_pad, 289+y_pad, triangle.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // triangles

        for(int i = 0; i<10; i++){
            if(selected_cam-1 == i){
                GRRLIB_DrawImg(x_l[i], y_l[i], CamLayoutNmb.frames[16], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
            }else{GRRLIB_DrawImg(x_l[i], y_l[i], CamLayoutNmb.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
            GRRLIB_DrawImg(x_l[i] + (CamLayoutNmb.frames[1]->w/2), y_l[i] + (CamLayoutNmb.frames[1]->h/2), CamLayoutNmb.frames[i + 1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // apparently this is the wrong way to scale it, but i'll let it be since it works
        }
        GRRLIB_DrawImg(268, 332, CamLayoutNmb.frames[17], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for big buttons
        GRRLIB_DrawImg(268 + ((CamLayoutNmb.frames[17]->w-CamLayoutNmb.frames[18]->w)/2), 332+((CamLayoutNmb.frames[17]->h-CamLayoutNmb.frames[18]->h)/2), CamLayoutNmb.frames[18], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // this is the right way to do it
    }
    else{ // for vent
        u16 x_l[5] = {363, 399, 454, 501, 529};
        u16 y_l[5] = {236, 313, 352, 301, 392};
        if(MapLayoutNormalMode == 0){
            GRRLIB_DrawImg(326, 222, CamLayout.frames[camLayoutSelection], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for the layout       
        }else{GRRLIB_DrawImg(326, 222, CamLayout.frames[camLayoutSelection + 2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);} // for the layout
        GRRLIB_DrawImg(375, 253, triangle.frames[2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // triangles
        GRRLIB_DrawImg(382, 319, triangle.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(438, 358, triangle.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(531, 307, triangle.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(512, 397, triangle.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for triangles

        for(int i = 0; i<5; i++){
            if(selected_cam-1 == i + 10){
                GRRLIB_DrawImg(x_l[i], y_l[i], CamLayoutNmb.frames[16], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
            }else GRRLIB_DrawImg(x_l[i], y_l[i], CamLayoutNmb.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
            GRRLIB_DrawImg(x_l[i] + (CamLayoutNmb.frames[i+11]->w/2), y_l[i] + (CamLayoutNmb.frames[i+11]->h/2), CamLayoutNmb.frames[i + 11], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }
        
        GRRLIB_DrawImg(350, 281, CamLayoutNmb.frames[20 + (closed_vent == 11)], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for closable vent cam 11
        GRRLIB_DrawImg(369, 328, CamLayoutNmb.frames[20 + (closed_vent == 12)], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for closable vent cam 12
        GRRLIB_DrawImg(426, 361, CamLayoutNmb.frames[20 + (closed_vent == 13)], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for closable vent cam 13
        GRRLIB_DrawImg(495, 406, CamLayoutNmb.frames[20 + (closed_vent == 15)], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for closable vent cam 15
        GRRLIB_DrawImg(543, 346, CamLayoutNmb.frames[20 + (closed_vent == 14)], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for closable vent cam 14

        GRRLIB_DrawImg(316, 193, CamLayoutNmb.frames[22 + isSealing], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);

        if(isSealing){
            GRRLIB_DrawImg(477, 434, loadGreen.frames[seal_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
            if(frame%20==0){
                seal_f = (seal_f+1) % 5;
            }
        }else{seal_f = 0;}
    }

    GRRLIB_DrawImg(268, 381, CamLayoutNmb.frames[17], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(268 + ((CamLayoutNmb.frames[17]->w-CamLayoutNmb.frames[19]->w)/2), 381+((CamLayoutNmb.frames[17]->h-CamLayoutNmb.frames[19]->h)/2), CamLayoutNmb.frames[19], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // end big buttons
}

void drawBackdrop(){
    GRRLIB_DrawImg(x, 0, backdrop.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(x+backdrop.frames[0]->w, 0, backdrop.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
}

void jumpscare(){
    if(camButton_alpha == 50){isCamOpen = false;}
    isControlPanelOpen = 2;
    static u8 sp_j = 0;
    GRRLIB_DrawImg(0, SCREEN_H - (springtrap_jumpscare.frames[sp_j]->h), springtrap_jumpscare.frames[sp_j], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    if(frame%4==0 && sp_j < 15){
        sp_j++;
    }
    if(sp_j >= 15){
        time_am = 6;
    }
}

void AC_calc(){ // yes, rewriting everything
    if(action_selected == 1){
        total_turns++;
        action_selected = 0;
    }else{total_turns = 0;}
    if(action_selected > 1 && action_selected < 4){
        if(mon_cam == 10){
            mon_cam = 9;
            action_selected = 0;
        }
    }
    if(action_selected == 2){
        if(mon_cam == 9) {mon_cam = 10; action_selected = 0;}
        if(mon_cam == 8) {mon_cam = 9; action_selected = 0;}
        if(mon_cam == 7) {mon_cam = 8; action_selected = 0;}
        if(mon_cam == 6) {mon_cam = 7; action_selected = 0;}
        if(mon_cam == 5) {mon_cam = 6; action_selected = 0;}
        if(mon_cam == 2) {mon_cam = 5; action_selected = 0;}
        if(mon_cam == 4) {mon_cam = 2; action_selected = 0;}
        if(mon_cam == 3) {mon_cam = 4; action_selected = 0;}
        if(attack_stage == 3 && ScreenOpen) {mon_cam = 4; action_selected = 0;}
        if(mon_cam == 1 && ScreenOpen) {mon_cam = 0; attack_stage = 3; action_selected = 0;}        
    }
    if(action_selected == 3){
        if(mon_cam == 9) {mon_cam = 8; action_selected = 0;}
        if(mon_cam == 8) {mon_cam = 7; action_selected = 0;}
        if(mon_cam == 7) {mon_cam = 6; action_selected = 0;}
        if(mon_cam == 5) {mon_cam = 2; action_selected = 0;}
        if(mon_cam == 2) {mon_cam = 4; action_selected = 0;}
    }
    if(action_selected == 4){
        if(mon_cam == 10) {mon_cam = 14; action_selected = 0;}
        if(mon_cam == 9) {mon_cam = 11; action_selected = 0;}
        if(mon_cam == 8) {mon_cam = 5; action_selected = 0;}
        if(mon_cam == 7) {mon_cam = 12; action_selected = 0;}
        if(mon_cam == 5 && picrandom == 0) {mon_cam = 4; action_selected = 0;}
        if(mon_cam == 5 && picrandom == 1) {mon_cam = 13; action_selected = 0;}
        if(mon_cam == 2 && picrandom == 0) {mon_cam = 0; attack_stage = 1; action_selected = 0;}
        if(mon_cam == 2 && picrandom == 1) {mon_cam = 15; action_selected = 0;}
    }
    if(action_selected > 2){
        if(mon_cam == 6) {mon_cam = 5; action_selected = 0;}
        if(mon_cam == 4) {mon_cam = 3; action_selected = 0;}
        if(mon_cam == 3) {mon_cam = 0; attack_stage = 1; action_selected = 0;}
        if(attack_stage == 1 && ScreenOpen) {mon_cam = 0; attack_stage = 2; action_selected = 0; return;} // ScreenOpen here is used as replacement as the r value of the red ball, might break idk
        if(attack_stage == 2) {mon_cam = 0; attack_stage = 3; action_selected = 0;}
        if(attack_stage == 3 && ScreenOpen) {mon_cam = 0; attack_stage = 4; action_selected = 0;}
        if(mon_cam == 1 && ScreenOpen) {mon_cam = 0; attack_stage = 4; action_selected = 0;}
    }
    if(action_selected > 1){
        if(attack_stage == 4 && isControlPanelOpen) {/* ROLL DOWN PANEL AND DIE */ isJumpscare = true;}
        if(attack_stage == 4 && camButton_alpha == 50) {/* SPRINGTRAP RUN TO THE OTHERSIDE, ROLL DOWN THE CAMS AND DIE */ isJumpscare = true;}
        if(attack_stage == 4 && !ScreenOpen && x < (-HALF_REGION + 30)) {/* PAN TO THE LEFT AND DIE */ isJumpscare = true;}
        if(mon_cam == 11 && closed_vent == 11) {mon_cam = 9; action_selected = 0;}
        if(mon_cam == 11 && closed_vent != 11) {mon_cam = 0; attack_stage = 3; action_selected = 0;}
        if(mon_cam == 12 && closed_vent == 12) {mon_cam = 7; action_selected = 0;}
        if(mon_cam == 12 && closed_vent != 12) {mon_cam = 0; attack_stage = 3; action_selected = 0;}   
        if(mon_cam == 13 && closed_vent == 13) {mon_cam = 5; action_selected = 0;}
        if(mon_cam == 13 && closed_vent != 13) {mon_cam = 0; attack_stage = 1; action_selected = 0;}  
        if(mon_cam == 14 && closed_vent == 14) {mon_cam = 10; action_selected = 0;}
        if(mon_cam == 14 && closed_vent != 14) {/* DIE */ isJumpscare = true;}      
        if(mon_cam == 15 && closed_vent == 15) {mon_cam = 2; action_selected = 0;}
        if(mon_cam == 15 && closed_vent != 15) {/* DIE */ isJumpscare = true;}         
    }
}

void Stage1_2(){
    static u8 springtrap_f = 1;
    if(attack_stage == 0){
        if(springtrap_attack.loaded){
            springtrap_attack.loaded = false;
            FreeImgAnim(&springtrap_attack);
        }
        return;
    }
    if(!springtrap_attack.loaded){
        springtrap_attack.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/206.png"); // stage 1
        springtrap_attack.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/207.png"); // stage 2
        springtrap_attack.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/208.png");
        springtrap_attack.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/209.png");
        springtrap_attack.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/211.png");
        springtrap_attack.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/212.png");
        springtrap_attack.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/213.png");
        springtrap_attack.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/214.png");
        springtrap_attack.frames[8] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/217.png");
        springtrap_attack.frames[9] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/218.png");
        springtrap_attack.frames[10] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/219.png");
        springtrap_attack.frames[11] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/220.png");
        springtrap_attack.frames[12] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/221.png");
        springtrap_attack.frames[13] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/222.png");
        springtrap_attack.frames[14] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/223.png");
        springtrap_attack.frames[15] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/224.png");
        springtrap_attack.frames[16] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/244.png"); // stage 2
        springtrap_attack.frames[17] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/519.png"); // stage 3
        springtrap_attack.loaded = true;
    }
    if(attack_stage == 1){
        GRRLIB_DrawImg(x+HALF_REGION+200, 40, springtrap_attack.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    }
    if(attack_stage == 2){
        GRRLIB_DrawImg(x+HALF_REGION+200 - (springtrap_f*26), 0, springtrap_attack.frames[springtrap_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        //time_am = 6;
        if(frame%2==0 && springtrap_f < 16){
            springtrap_f++;
        }
    }
    //GRRLIB_DrawImg(x+HALF_REGION+200, 40, springtrap_cams.frames[17], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    //if(attack_stage == 3 && IsCurrentlyClosingPanel){
        //GRRLIB_DrawImg(x+HALF_REGION, 40, springtrap_cams.frames[18], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); for later
    //}
}

void sealVent(){
    static int vent_g = 1000;
    static int vent_f = 0;
    int r = (rand() % 49) + 50;
    if(vent_f >= vent_g){
        closed_vent = selected_cam;
        vent_g = 1000;
        vent_f = 0;
        isSealing = false;
    }
    
    if(vent_g == 1000){
        vent_g = r;
    }
    vent_f++;
}

void reboot(){
    int x_pad = 200;
    if(isRebootingAudio){
        GRRLIB_DrawImg(85+x_pad, 235, loadGreen.frames[seal_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // yes, using seal_f
        if(audioProgress >= 10){
            audioProgress = 0;
            audioHealth = 10;
            audioError = false;
            isRebootingAudio = false;
            isRebooting = false;
        }
    }
    if(isRebootingCam){
        GRRLIB_DrawImg(85+x_pad, 273, loadGreen.frames[seal_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        if(camProgress >= 10){
            camProgress = 0;
            camHealth = 10;
            videoError = false;
            isRebootingCam = false;
            isRebooting = false;
        }
    }
    if(isRebootingVent){
        GRRLIB_DrawImg(85+x_pad, 306, loadGreen.frames[seal_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        if(ventProgress >= 10){
            ventProgress = 0;
            ventHealth = 10;
            ventilationError = false;
            isRebootingVent = false;
            isRebooting = false;
        }
    }
    if(isRebootingAll){
        GRRLIB_DrawImg(85+x_pad, 373, loadGreen.frames[seal_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        if(allProgress >= 10){
            allProgress = 0;
            audioHealth = 10;
            camHealth = 10;
            ventHealth = 10;
            audioError = false;
            videoError = false;
            ventilationError = false;
            isRebootingAll = false;
            isRebooting = false;
        }
    }
    if(frame%20==0){
        seal_f = (seal_f + 1) % 6;
    }
    if(!isRebooting){
        seal_f = 0;
    }
}

void BB_calc(){
    if(selected_cam != BBSpawn){
        if(BBcounter > 0){
            BBcounter = 0;
            BBSpawn = -1;
        }
        return;
    }
    static u8 bb_f = 1;
    if(BBcounter >= BB_Ai[NIGHT-1]){
        //BBcounter = 0;
        //time_am = 6;
        IsBBJumpscaring = true;
        isCamOpen = false;
    }
    if(IsBBJumpscaring){
        int y_pad = 40;
        int x_pad = 100;
        if(camButton_alpha != 100){
            GRRLIB_DrawImg(0 + x_pad, SCREEN_H - (BB.frames[bb_f]->h) + y_pad, BB.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }else{
            if(bb_f == 1){
                GRRLIB_DrawImg(0 + x_pad, SCREEN_H - (BB.frames[bb_f]->h) + y_pad, BB.frames[bb_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
            }else{GRRLIB_DrawImg(0, SCREEN_H - (BB.frames[bb_f]->h), BB.frames[bb_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + BBAlpha);}
            if(frame%3==0 && bb_f < 11){
                bb_f++;
            }
            if(bb_f >= 11){
                if(BBAlpha > 0){
                    BBAlpha-=5;
                }else{
                    BBcounter = 0;
                    BBSpawn = -1;
                    IsBBJumpscaring = false;
                    ventHealth = 0;
                    ventilationError = true;
                    hasBBSpawned = true;
                }
            }
        }
    }
    if(selected_cam == BBSpawn){BBcounter++;}
}

int gameSceneMain(){
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Render();
    InitAllImg();
    srand(time(NULL));
    s8 x_pad = -30; // FOR CAM LAYOUT BUTTONS
    s8 y_pad = 0; // FOR CAM LAYOUT BUTTONS
    u16 x_l[15] = {391 + x_pad, 525 + x_pad, 577 + x_pad, 577 + x_pad, 475 + x_pad, 387 + x_pad, 387 + x_pad, 454 + x_pad, 495 + x_pad, 562 + x_pad, 363, 399, 454, 501, 529}; // FOR CAM LAYOUT BUTTONS
    u16 y_l[15] = {407 + y_pad, 391 + y_pad, 364 + y_pad, 324 + y_pad, 333 + y_pad, 338 + y_pad, 297 + y_pad, 283 + y_pad, 257 + y_pad, 280 + y_pad, 236, 313, 352, 301, 392}; // FOR CAM LAYOUT BUTTONS
    char buffer[100];
    sceneTex = GRRLIB_CreateEmptyTexture(SCREEN_W, SCREEN_H);
    u64 buttonCamDelay = 0;
    picrandom = rand()%2;
    if(NIGHT!=1){
        mon_cam = (rand() % 5) + 6;
    }
    while (1)
    {
        if(time_am == 6){
            break;
        }
        WPAD_ScanPads();
        mote = WPAD_ButtonsDown(0);
        WPAD_IR(0, &ir);
        IR_Y = ir.y * (480.0f / 460.0f);
        if(camButton_alpha == 99 || camButton_alpha == 50 || isJumpscare == true || isControlPanelOpen == 1 || IsBBJumpscaring){
            canPan = false;
        }
        if(camButton_alpha != 99 && camButton_alpha != 50 && isJumpscare == false && isControlPanelOpen != 1 && !IsBBJumpscaring){
            canPan = true;
        }
        if(hasBBSpawned){
            if(BB.loaded){
                BB.loaded = false;
                FreeImgAnim(&BB);
            }
        }
        //if(attack_stage > 0){mon_cam = 0;} // fail safe, might remove if its broken
        drawBackdrop();
        Stage1_2();
        drawOffice();
        drawFan();
        if(!isJumpscare){
            GRRLIB_Screen2Texture(0, 0, sceneTex, false);
        }
        DrawWarpedScene(); //anything drew before this are getting warped
        drawPanelButton();
        drawControlPanel();
        BB_calc();
        drawCamPanel();
        drawCams();
        if(selected_cam == BBSpawn && isCamOpen && !videoError){
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, BB.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }
        drawCamStatic();
        if(isSealing){
            sealVent();
        }
        if(blip_f < 4){
            drawBlip();
        }
        if(audioHealth <= 0){
            audioError = true;
        }
        if(camHealth <= 0){
            videoError = true;
        }
        if(ventHealth <= 0){
            ventilationError = true;
        }
        if(isRebooting){
            reboot();
        }
        drawCamLayout();
        drawCamButton();
        
        sprintf(buffer, "%d, %d, %d, %d, %d, %d, %d", mon_cam, action_selected, attack_stage, total_turns, isCamOpen, neutral_cams.frames[0] == NULL, neutral_cams.loaded[0]);
        //sprintf(buffer, "%d, %d, %d, %d, %d", isRebootingAudio, isRebootingCam, isRebootingVent, isRebootingAll, isRebooting);
        //sprintf(buffer, "%d, %d", SYS_GetArena1Size(), SYS_GetArena2Size());
        //sprintf(buffer, "%d", selected_cam == BBSpawn);
        GRRLIB_Printf(10, 10, font, 0xFFFFFFFF, IMG_SCALE, buffer);
        if(mote & WPAD_BUTTON_A){
            if(canPanelButtonBeClicked){
                if(IsCursorWithinImg(panelClick, CONTROL_PANEL_X - (panelClick->w / 2), CONTROL_PANEL_Y - (panelClick->h))){isControlPanelOpen = 1;}
            }
            if(!PanelButtonVisible && !isRebooting){
                if(panelSelect == 4){isControlPanelOpen = 2;}
                if(panelSelect == 0) {
                    isRebootingAudio = true;
                    isRebooting = true;
                }
                if(panelSelect == 1) {
                    isRebootingCam = true;
                    isRebooting = true;
                }
                if(panelSelect == 2) {
                    isRebootingVent = true;
                    isRebooting = true;
                }
                if(panelSelect == 3) {
                    isRebootingAll = true;
                    isRebooting = true;
                }
            }
            if(camButton_alpha == 100 && fabs(x + HALF_REGION) < 1.0f){
                if(IsCursorWithinImg(camClick, 540, 73)){
                    isCamOpen = !isCamOpen;
                }
            }
            if(camButton_alpha == 50){
                if(IsCursorWithinImg(camClick, 540, 73)){
                    isCamOpen = !isCamOpen;
                }
                for(int i = 0; i < 10; i++){
                    if(camLayoutSelection == 0){
                        if(IsCursorWithinImg(CamLayoutNmb.frames[0], x_l[i], y_l[i]) && buttonCamDelay >= 20){
                            selected_cam = i + 1;
                            blip_f = 0;
                            CamStatic_alpha = 255;
                            buttonCamDelay = 0;
                        }  
                    } 
                }
                for(int i = 0; i < 5; i++){
                    if(camLayoutSelection == 1 && !isSealing){
                        if(IsCursorWithinImg(CamLayoutNmb.frames[0], x_l[i + 10], y_l[i + 10]) && buttonCamDelay >= 20){
                            blip_f = 0;
                            CamStatic_alpha = 255;
                            buttonCamDelay = 0;
                            if(double_click_timer_vent < 60 && !isSealing && selected_cam == (i+11)){
                                isSealing = true;
                            }
                            selected_cam = i + 11;
                            double_click_timer_vent = 0;
                        }  
                    }                 
                }
                if(IsCursorWithinImg(CamLayoutNmb.frames[18], 268 + ((CamLayoutNmb.frames[17]->w-CamLayoutNmb.frames[19]->w)/2), 381+((CamLayoutNmb.frames[17]->h-CamLayoutNmb.frames[19]->h)/2)) && camButton_alpha == 50){ // for map toggle, SON :sob:
                    if(camLayoutSelection == 0){
                        selected_cam = 11;
                    }else{selected_cam = 2;}
                    camLayoutSelection = !camLayoutSelection;
                }          
            }
        }
        drawPanelText();
        if(isJumpscare){
            jumpscare();
        }
        drawNightInf();
        if(NIGHT == 1){
            if(frame%(60*40)==0 && frame != 0){
                if(time_am == 12){
                    time_am = 1;
                }else{time_am += 1;}
            }
        }
        else{
            if(frame%(60*60)==0 && frame != 0){
                if(time_am == 12){
                    time_am = 1;
                }else{time_am += 1;}
            }
        }
        if(camButton_alpha == 50){ // for the cam health
            if(!camLayoutSelection){
                CamErrorTimer++;
                if(CamErrorTimer >= (60*12)){ // each 12 seconds, springtrap's AI level is deducted from the cam's health
                    camHealth -= Springtrap_AI[NIGHT - 1];
                    CamErrorTimer = 0;
                }
            }else{
                VentErrorTimer++;
                if(NIGHT == 2 && VentErrorTimer >= (60*12)) {ventHealth-=Springtrap_AI[NIGHT - 1]; VentErrorTimer = 0;}
                if(NIGHT == 3 && VentErrorTimer >= (60*10)) {ventHealth-=Springtrap_AI[NIGHT - 1]; VentErrorTimer = 0;}
                if(NIGHT == 4 && VentErrorTimer >= (60*9)) {ventHealth-=Springtrap_AI[NIGHT - 1]; VentErrorTimer = 0;}
                if(NIGHT == 5 && VentErrorTimer >= (60*8)) {ventHealth-=Springtrap_AI[NIGHT - 1]; VentErrorTimer = 0;}
            }
        }
        if(frame%60==0){ //global each second timer
            MapLayoutNormalMode = !MapLayoutNormalMode;
            if(move_counter>(10 - Springtrap_AI[NIGHT-1] - aggresive)+((rand()%16)-total_turns)){ // ai blabla
                action_selected = rand() % (3 + aggresive + 1);
                move_counter = 0;
                CamStatic_alpha = 255;
                AC_calc();
            }
            if(isRebootingAudio){
                audioProgress += (rand() % 4) + 1;
            }
            if(isRebootingCam){
                camProgress += (rand() % 4) + 1;
            }
            if(isRebootingVent){
                ventProgress += (rand() % 4) + 1;
            }
            move_counter++;
        }
        if(frame%(60*2)==0){ // global 2 seconds timer
            if(isRebootingAll){
                allProgress += (rand() % 4) + 1;
            }
        }
        if(frame%(60*10)==0){ //global 10 seconds timer
            if(selected_cam != mon_cam){
                picrandom = rand()%3; // idk what is this used for  but ig
            }
        }
        if(ScreenOpen == false){
            OfficeIdle++;
        }
        if(OfficeIdle%(60*20)==0){ // global office 20 seconds timer
            if(!hasBBSpawned && ((rand() % 11) + 1) <= Springtrap_AI[NIGHT-1] && time_am >= 1 && time_am < 6 && BBSpawn == -1 && !videoError){
                u8 bb_cams[4] = {1, 7, 9, 10};
                BBSpawn = bb_cams[(rand() % 4)];
            }
        }
        if(time_am == 3 && !hasBBSpawned && BBSpawn == -1 && !videoError){ // force spawn bb at 3 am
            u8 bb_cams[4] = {1, 7, 9, 10};
            BBSpawn = bb_cams[(rand() % 4)];
        }
        // springtrap go to attack stage 2 when: looking at any screen and action selected = 2
        if(time_am >= 4){
            aggresive = true;
        }
        if(time_am == 6){break;}
        drawCursor();
        GRRLIB_Render();
        frame++;
        buttonCamDelay++;
        double_click_timer_vent++;
    }
    GRRLIB_FreeTexture(panelClick);
    GRRLIB_FreeTexture(camClick);
    GRRLIB_FreeTexture(sceneTex);
    FreeImgAnim(&office);
    FreeImgAnim(&fan);
    FreeImgAnim(&controlPanel);
    FreeImgAnim(&PanelCommands);
    FreeImgAnim(&NightTimeFont);
    FreeImgAnim(&CamPanel);
    FreeImgAnim(&CamLayout);
    FreeImgAnim(&CamLayoutNmb);
    FreeImgAnim(&triangle);
    FreeImgAnim(&CamStatic);
    FreeImgAnim(&white_blip);
    FreeAllImgAnim2(&neutral_cams);
    FreeAllImgAnim2(&springtrap_cams);
    FreeImgAnim(&backdrop);
    FreeImgAnim(&springtrap_attack);
    FreeImgAnim(&springtrap_jumpscare);
    FreeImgAnim(&loadGreen);
    FreeImgAnim(&BB);
    return 0;
}