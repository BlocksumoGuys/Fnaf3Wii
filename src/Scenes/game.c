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
u8 NIGHT = 0;
char buffer[100];
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
bool hasMangleSpawned = false; //hehe
bool hasMangleJumspcared = false;
bool canMangleSpawn = true;
bool hasFreddySpawned = false; // change to false
bool canFreddySpawn = true;
bool hasFreddyDukedDown = false;
bool isFreddyJumpscaring = false;
bool canPlayLure = true;
bool isLurePlaying = false;
bool canMute = true;
bool destroyedMute = false;
bool hasPuppetSpawned = false; // change to false
bool canPuppetSpawn = true;
bool isPuppetAttacking = false;
bool puppetLoaded = false;
bool BBcamLoaded = false;
bool hasChicaCamSpawned = false; // change to false
bool hasChicaOfficeSpawned = false;
bool canChicaSpawn = true;
bool isChicaJumpscaring = false;
bool hasFoxySpawned = false; // change to false
bool canFoxySpawn = true;
bool isFoxyJumpscaring = false;
bool hasSeenFoxy = false;
int foxy_chance = 0;
u8 foxy_f = 0;
u8 foxy_alpha = 255;
u8 chica_f = 0;
u8 chica_alpha = 255;
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
IMG_Anim freddy = {0};
IMG_Anim freddy_jumpscare = {0};
IMG_Anim lureLoad = {0};
IMG_Anim chica = {0};
IMG_Anim foxy = {0};
u8 freddy_f = 0;
GRRLIB_texImg *cursor = NULL;
GRRLIB_texImg *font = NULL;
GRRLIB_texImg *panelClick = NULL;
GRRLIB_texImg *camClick = NULL;
GRRLIB_texImg *sceneTex = NULL;
GRRLIB_texImg *mangle = NULL;
GRRLIB_texImg *mute = NULL;
GRRLIB_texImg *puppet = NULL;
GRRLIB_texImg *bb_cam = NULL;
u8 camButton_alpha = 100;
u8 selected_cam = 2;
u8 picrandom;
int mon_cam = 0;
const u8 Springtrap_AI[6] = {0, 2, 3, 4, 5, 7}; // for last, 7 instead of 10
const u8 BB_Ai[6] = {100, 90, 80, 70, 60, 50};
int camHealth = 10;
int ventHealth = 10;
int audioHealth = 10;
int camProgress = 0;
int ventProgress = 0;
int audioProgress = 0;
int allProgress = 0;
u64 CamErrorTimer = 0;
u64 VentErrorTimer = 0;
u64 VentErrorTimer2 = 0;
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
int BBSpawn = -1; // change back to -1
u64 BBcounter = 0;
u64 MangleCounter = 0;
u64 MangleJumspcareCounter = 0;
u64 freddyCounter = 0;
u8 BBAlpha = 255;
u8 freddyjumpscare_f = 0;
u8 freddy_alpha = 255;
int lureDelay = 0;
u64 PuppetCamCounter = 0;
u64 PuppetOfficeCounter = 0;
u64 chicaCounter = 0;
u64 IdleCounter = 0;

s8 x_pad = -30; // FOR CAM LAYOUT BUTTONS
s8 y_pad = 0; // FOR CAM LAYOUT BUTTONS
u16 cam_x_l[15] = {361, 495, 547, 547, 445, 357, 357, 424, 465, 532, 363, 399, 454, 501, 529}; // FOR CAM LAYOUT BUTTONS
u16 cam_y_l[15] = {407, 391, 364, 324, 333, 338, 297, 283, 257, 280, 236, 313, 352, 301, 392}; // FOR CAM LAYOUT BUTTONS

int Desolate_Underworld2;
int tablefan;
int crank1;
int crank2;
int static_sound;
int lever1;
int lever2;
int select_sound;
int reboot_sound;
int alarm;
int vent_quiet1;
int vent_quiet2;
int vent_closer1;
int vent_closer2;
int done;
int scream3;
int garble1;
int mask_sound;
int danger2b;
int stop_sound;
u8 play_effect;
u8 echo;
bool isPlay1;
bool isPlay2;
bool isPlay3;
bool isPlay4;

int nextScene = -1;

const char *sp_cams_arr[33] = {
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
    "sd:/fnaf3/assets/images/832.png",
    "sd:/fnaf3/assets/images/38.png" // mangle
    "sd:/fnaf3/assets/images/297.png", // phantom puppet
    "sd:/fnaf3/assets/images/298.png",
    "sd:/fnaf3/assets/images/300.png",
    "sd:/fnaf3/assets/images/298.png" // phantom puppet
    "sd:/fnaf3/assets/images/387.png", // phantom chica
    "sd:/fnaf3/assets/images/388.png",
    "sd:/fnaf3/assets/images/389.png",
    "sd:/fnaf3/assets/images/388.png" // phantom chica
};

const char *nt_cams_arr[33] = {
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
    "sd:/fnaf3/assets/images/38.png", // mangle
    "sd:/fnaf3/assets/images/297.png", // phantom puppet
    "sd:/fnaf3/assets/images/298.png",
    "sd:/fnaf3/assets/images/300.png",
    "sd:/fnaf3/assets/images/298.png", // phantom puppet
    "sd:/fnaf3/assets/images/387.png", // phantom chica
    "sd:/fnaf3/assets/images/388.png",
    "sd:/fnaf3/assets/images/389.png",
    "sd:/fnaf3/assets/images/388.png" // phantom chica
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

void InitAllImg(){
    office.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/203(1).png"); // normal 1
    office.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/203(2).png"); // normal 2
    office.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/204(1).png"); // alarm 1
    office.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/204(2).png"); // alarm 2
    fan.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/18.png");
    fan.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/19.png");
    fan.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/20.png");
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

    loadGreen.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/611.png"); // seal green bean
    loadGreen.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/612.png");
    loadGreen.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/629.png");
    loadGreen.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/632.png");
    loadGreen.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/637.png"); // seal green bean

    chica.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/399.png");
    chica.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/428.png");
    chica.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/434.png");
    chica.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/439.png");
    //chica.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/428.png");
    chica.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/459.png");
    chica.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/462.png");
    chica.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/464.png");
    chica.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/466.png");
    chica.frames[8] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/385.png");
    chica.loaded = false; // NEEDS TO BE FALSE

    foxy.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/184.png");
    foxy.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/183.png");
    foxy.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/186.png");
    foxy.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/175.png");
    foxy.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/176.png");
    foxy.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/178.png");
    foxy.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/181.png");
    foxy.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/182.png");
    foxy.loaded = false; // NEEDS TO BE FALSE

    mute = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/699.png");

    mangle = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/202.png");
    Desolate_Underworld2 = SM_LoadAudio("Desolate_Underworld2.wav");
    tablefan = SM_LoadAudio("tablefan.wav");
    static_sound = SM_LoadAudio("static_sound.wav");
    select_sound = SM_LoadAudio("select.wav");
    scream3 = SM_LoadAudio("scream3.wav");
    danger2b = SM_LoadAudio("danger2b.wav");
    SM_PlayAudio(danger2b, 0, 0, true, false, 0);
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
        if(hasChicaCamSpawned){v = cam7_f + 29;}
        if(selected_cam == mon_cam){
            dcv(v, 0);
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, springtrap_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for anims
        }else {dcv(v, 1); GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, neutral_cams.frames[v], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
        break;
    case 8:
        v = cam8_f + 13;
        if(hasPuppetSpawned){v = cam8_f + 25;}
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
        if(hasMangleSpawned){
            v = 24;
        }else{v = selected_cam;}
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
        if(!PanelCommands.loaded){
            PanelCommands.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/599.png"); // "system restart menu"
            PanelCommands.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/544.png"); // "audio devices"
            PanelCommands.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/545.png"); // "camera system"
            PanelCommands.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/555.png"); // "ventilation"
            PanelCommands.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/594.png"); // "reboot all"
            PanelCommands.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/600.png"); // "exit"
            PanelCommands.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/596.png"); // green select cursor
            PanelCommands.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/601.png"); // error
            PanelCommands.loaded = true;
        }
        GRRLIB_DrawImg(10, SCREEN_H - (controlPanel.frames[controlPanel_f]->h) + 5, controlPanel.frames[controlPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        if(frame%2==0){
            if(controlPanel_f!= 10){
                controlPanel_f+=1;
                canPanelButtonBeClicked = false;           
            }
            else{PanelButtonVisible = false; isControlPanelOpen = true;}
        }
    }
    else if(isControlPanelOpen == 2){
        if(PanelCommands.loaded){
            PanelCommands.loaded = false;
            FreeImgAnim(&PanelCommands);
        }
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
        if(frame%30==0){
            alarm = SM_LoadAudio("alarm.wav");
            SM_PlayAudio(alarm, 120, 120, false, true, 0);
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
    static int old_select = 0;
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
    if(old_select != panelSelect){
        SM_PlayAudio(select_sound, 127, 127, false, false, 0);
    }
    old_select = panelSelect;
}

void drawCamButton(){
    if(fabs(x + HALF_REGION) > 1.0f) return;
    GRRLIB_DrawImg(540, 73, camClick, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + camButton_alpha);
}

void drawCamPanel(){
    if(isCamOpen && !isJumpscare && !IsBBJumpscaring && !isFreddyJumpscaring){
        CamPanel_alpha = 255;
        GRRLIB_DrawImg(SCREEN_W - (CamPanel.frames[CamPanel_f]->w), 60, CamPanel.frames[CamPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00+CamPanel_alpha);
        if(frame%2==0){
            if(CamPanel_f < 9){
                CamPanel_f+=1;
                camButton_alpha = 99;
            }else{camButton_alpha = 50;}
        }
    }else{
        FreeAllImgAnim2(&neutral_cams);
        FreeAllImgAnim2(&springtrap_cams);
        GRRLIB_DrawImg(SCREEN_W - (CamPanel.frames[CamPanel_f]->w), 60, CamPanel.frames[CamPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00+CamPanel_alpha);
        if(frame%2==0){
            if(CamPanel_f > 0){
                CamPanel_f-=1;
                camButton_alpha = 99;
            }else{camButton_alpha = 100; CamPanel_alpha = 0;}
        }
    }
}

void drawCamStatic(){
    if(camButton_alpha!=50){
        SM_StopAudio(static_sound);
        return;
    };
    SM_PlayAudio(static_sound, CamStatic_alpha, CamStatic_alpha, false, false, 0);
    static u8 CamStatic_f = 0;
    GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, CamStatic.frames[CamStatic_f], 0, 825.0f/1021.2f, 650.0f/768.0f, 0xFFFFFF00+CamStatic_alpha);
    CamStatic_f = (CamStatic_f+1)%3;
    if(CamStatic_alpha>80){
        SM_ChangeVolume(static_sound, CamStatic_alpha-60, CamStatic_alpha-60);
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
        int x_pad = -30;
        if(MapLayoutNormalMode == 0){
            GRRLIB_DrawImg(326, 222, CamLayout.frames[camLayoutSelection], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for the layout       
        }else{GRRLIB_DrawImg(326, 222, CamLayout.frames[camLayoutSelection + 2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);} // for the layout
        GRRLIB_DrawImg(420+x_pad, 413, triangle.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // triangles
        GRRLIB_DrawImg(528+x_pad, 376, triangle.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(560+x_pad, 370, triangle.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(560+x_pad, 330, triangle.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        // there's no fifth one
        GRRLIB_DrawImg(414+x_pad, 344, triangle.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(414+x_pad, 304, triangle.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(461+x_pad, 300, triangle.frames[2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(500+x_pad, 270, triangle.frames[2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_DrawImg(547+x_pad, 289, triangle.frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // triangles

        for(int i = 0; i<10; i++){
            if(selected_cam-1 == i){
                GRRLIB_DrawImg(cam_x_l[i], cam_y_l[i], CamLayoutNmb.frames[16], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
            }else{GRRLIB_DrawImg(cam_x_l[i], cam_y_l[i], CamLayoutNmb.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);}
            GRRLIB_DrawImg(cam_x_l[i] + (CamLayoutNmb.frames[1]->w/2), cam_y_l[i] + (CamLayoutNmb.frames[1]->h/2), CamLayoutNmb.frames[i + 1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // apparently this is the wrong way to scale it, but i'll let it be since it works
        }
        if(!isLurePlaying){
            GRRLIB_DrawImg(268, 332, CamLayoutNmb.frames[17], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for big buttons
            GRRLIB_DrawImg(268 + ((CamLayoutNmb.frames[17]->w-CamLayoutNmb.frames[18]->w)/2), 332+((CamLayoutNmb.frames[17]->h-CamLayoutNmb.frames[18]->h)/2), CamLayoutNmb.frames[18], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // this is the right way to do it
        }
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
    if(!springtrap_jumpscare.loaded){
        springtrap_jumpscare.frames[0] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/778.png");
        springtrap_jumpscare.frames[1] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/779.png");
        springtrap_jumpscare.frames[2] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/782.png");
        springtrap_jumpscare.frames[3] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/785.png");
        springtrap_jumpscare.frames[4] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/788.png"); // 11
        springtrap_jumpscare.frames[5] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/791.png"); // 14
        springtrap_jumpscare.frames[6] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/674.png"); // 17
        springtrap_jumpscare.frames[7] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/671.png"); // 20
        springtrap_jumpscare.frames[8] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/668.png"); // 23
        springtrap_jumpscare.frames[9] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/663.png"); // 26
        springtrap_jumpscare.frames[10] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/636.png"); // 29
        springtrap_jumpscare.frames[11] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/631.png"); // 32
        springtrap_jumpscare.frames[12] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/679.png"); // 35
        springtrap_jumpscare.frames[13] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/680.png"); // 38
        springtrap_jumpscare.frames[14] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/691.png"); // 40
        springtrap_jumpscare.frames[15] =  GRRLIB_FakeLoad("sd:/fnaf3/assets/images/692.png"); // 41
        springtrap_jumpscare.loaded = true;
        SM_PlayAudio(scream3, 200, 200, false, true, 0);
    }
    isControlPanelOpen = 2;
    static u8 sp_j = 0;
    GRRLIB_DrawImg(0, SCREEN_H - (springtrap_jumpscare.frames[sp_j]->h), springtrap_jumpscare.frames[sp_j], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    if(frame%5==0 && sp_j < 15){
        sp_j++;
    }
    if(sp_j >= 15){
        nextScene = 0;
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
        if(mon_cam == 10) {mon_cam = 14; action_selected = 0; play_effect = (rand()%2)+1;}
        if(mon_cam == 9) {mon_cam = 11; action_selected = 0; play_effect = (rand()%2)+1;}
        if(mon_cam == 8) {mon_cam = 5; action_selected = 0;}
        if(mon_cam == 7) {mon_cam = 12; action_selected = 0; play_effect = (rand()%2)+1;}
        if(mon_cam == 5 && picrandom == 0) {mon_cam = 4; action_selected = 0;}
        if(mon_cam == 5 && picrandom == 1) {mon_cam = 13; action_selected = 0; play_effect = (rand()%2)+1;}
        if(mon_cam == 2 && picrandom == 0) {mon_cam = 0; attack_stage = 1; action_selected = 0;}
        if(mon_cam == 2 && picrandom == 1) {mon_cam = 15; action_selected = 0; play_effect = (rand()%2)+1;}
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
        if(mon_cam == 11 && closed_vent == 11) {mon_cam = 9; action_selected = 0; play_effect = (rand()%2)+1;}
        if(mon_cam == 11 && closed_vent != 11) {mon_cam = 0; attack_stage = 3; action_selected = 0; play_effect = (rand()%2)+3;}
        if(mon_cam == 12 && closed_vent == 12) {mon_cam = 7; action_selected = 0; play_effect = (rand()%2)+1;}
        if(mon_cam == 12 && closed_vent != 12) {mon_cam = 0; attack_stage = 3; action_selected = 0; play_effect = (rand()%2)+3;}   
        if(mon_cam == 13 && closed_vent == 13) {mon_cam = 5; action_selected = 0; play_effect = (rand()%2)+1;}
        if(mon_cam == 13 && closed_vent != 13) {mon_cam = 0; attack_stage = 1; action_selected = 0; play_effect = (rand()%2)+3;}  
        if(mon_cam == 14 && closed_vent == 14) {mon_cam = 10; action_selected = 0; play_effect = (rand()%2)+1;}
        if(mon_cam == 14 && closed_vent != 14) {/* DIE */ isJumpscare = true; play_effect = (rand()%2)+3;}      
        if(mon_cam == 15 && closed_vent == 15) {mon_cam = 2; action_selected = 0; play_effect = (rand()%2)+1;}
        if(mon_cam == 15 && closed_vent != 15) {/* DIE */ isJumpscare = true; play_effect = (rand()%2)+3;}         
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
        springtrap_attack.frames[18] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/529.png"); // stage 4
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
    if(attack_stage == 4 && !isJumpscare){
        GRRLIB_DrawImg(x+80, 85, springtrap_attack.frames[18], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
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
        done = SM_LoadAudio("done.wav");
        SM_PlayAudio(done, 50, 50, false, true, 0);
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
            SM_FreeAudio(reboot_sound);
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
            SM_FreeAudio(reboot_sound);
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
            SM_FreeAudio(reboot_sound);
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
            SM_FreeAudio(reboot_sound);
        }
    }
    if(frame%20==0){
        seal_f = (seal_f + 1) % 6;
    }
    if(frame%120==0){
        if(isRebooting){
            reboot_sound = SM_LoadAudio("wait.wav");
            SM_PlayAudio(reboot_sound, 50, 50, false, true, 0);
        }
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
        if(!BB.loaded){
            BB.frames[0] = NULL; // bb on cam (loaded dynamically)
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
        int y_pad = 40;
        int x_pad = 100;
        if(camButton_alpha != 100){
            GRRLIB_DrawImg(0 + x_pad, SCREEN_H - (BB.frames[bb_f]->h) + y_pad, BB.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }else{
            if(bb_f == 1){
                SM_PlayAudio(scream3, 200, 200, false, false, 0);
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
                    if(BB.loaded){BB.loaded = false; FreeImgAnim(&BB);}
                }
            }
        }
    }
    if(selected_cam == BBSpawn){BBcounter++;}
}

void Mangle_calc(){
    //GRRLIB_DrawImg(x+HALF_REGION, 190, GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/202.png"), 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    static int mangle_y = 250;
    static bool JumpAudio = false;
    if((selected_cam != 4 || !isCamOpen) && hasMangleSpawned){
        if(MangleCounter > 0){
            MangleCounter = 0;
            hasMangleSpawned = false;
            mangle_y = 250;
        }
        return;
    }
    if(MangleCounter >= BB_Ai[NIGHT-1] && !hasMangleJumspcared){
        canMangleSpawn = false;
        hasMangleJumspcared = true;
        hasMangleSpawned = false;
        isCamOpen = false;
    }
    if(hasMangleJumspcared){
        if(mangle_y > 190 && frame%2==0){
            mangle_y--;
        }
        if(!JumpAudio){
            garble1 = SM_LoadAudio("garble1.wav");
            SM_PlayAudio(garble1, 120, 120, true, false, 0);
            JumpAudio = true;
        }
        MangleJumspcareCounter++;
        GRRLIB_DrawImg(x+HALF_REGION, mangle_y, mangle, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    }
    if(MangleJumspcareCounter >= (60*10)){
        hasMangleSpawned = false;
        MangleCounter = 0;
        if(mangle_y < 250){
            mangle_y++;
        }else{MangleJumspcareCounter = 0; hasMangleJumspcared = false; audioHealth = 0; MangleCounter = 0; JumpAudio = false; SM_FreeAudio(garble1); GRRLIB_FreeTexture(mangle);}
    }
    if(selected_cam == 4 && hasMangleSpawned && isCamOpen){MangleCounter++;}
}

void Freddy_calc(){
    if(!hasFreddySpawned){
        if(freddy.loaded){
            freddy.loaded = false;
            FreeImgAnim(&freddy);
        }
        return;
    }
    if(!freddy.loaded){
        freddy.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/653.png");
        freddy.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/655.png");
        freddy.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/641.png");
        freddy.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/642.png");
        freddy.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/643.png");
        freddy.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/644.png");
        freddy.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/646.png");
        freddy.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/647.png");
        freddy.frames[8] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/648.png");
        freddy.frames[9] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/649.png");
        freddy.frames[10] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/650.png");
        freddy.frames[11] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/651.png");
        freddy.frames[12] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/652.png");
        freddy.loaded = true;
    }
    static int freddy_x = 0;
    static int freddy_y = 80;
    freddy_x = x+HALF_REGION + 300;
    static int freddy_x2 = 0; // ends at -560
    GRRLIB_DrawImg(freddy_x + freddy_x2, freddy_y, freddy.frames[freddy_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    if(frame%4==0 && !hasFreddyDukedDown){
        freddy_f = (freddy_f + 1) % 13;
        freddy_x2-=2;
    }
    if(freddyCounter >= (BB_Ai[NIGHT-1] * 3)){
        canFreddySpawn = false;
        if(freddy_y < 250){
            freddy_y+=5;
        }else{hasFreddyDukedDown = true; hasFreddySpawned = false;}
    }else{
        if(freddy_x2 <= -560){
            hasFreddySpawned = false;
            hasFreddyDukedDown = false;
            //isFreddyJumpscaring = false;
        }
    }
    if(hasFreddyDukedDown){
        if(freddy.loaded){
            freddy.loaded = false;
            FreeImgAnim(&freddy);
        }
    }
    if(!ScreenOpen && !hasFreddyDukedDown){freddyCounter++;}
}

void playLure(){
    static u8 lure_cam = 0;
    static int working = -1;
    static u8 lure_f = 0;
    static u8 lure_radio_f = 0;
    static bool hasTped = false;
    static u64 lureCounter = 0;
    if(lure_cam == 0){
        lure_cam = selected_cam;
    }
    if(working == -1){
        working = (rand() % 7) + 1;
    }
    if(isLurePlaying){
        canPlayLure = false;
        if(!lureLoad.loaded){
            lureLoad.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/533.png");
            lureLoad.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/534.png");
            lureLoad.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/535.png");
            lureLoad.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/536.png");
            lureLoad.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/537.png");
            lureLoad.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/538.png");
            lureLoad.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/539.png"); // this and onward is the "radio"
            lureLoad.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/540.png");
            lureLoad.frames[8] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/541.png");
            lureLoad.frames[9] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/542.png");
            lureLoad.loaded = true;
        }
        if(working >= 2){
            if(working >= 2 && lure_cam == 1 && attack_stage == 4){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 2 && attack_stage == 1){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 2 && mon_cam == 3){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 2 && mon_cam == 4){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 2 && mon_cam == 5){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 3 && mon_cam == 2){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 3 && mon_cam == 4){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 4 && mon_cam == 2){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 4 && mon_cam == 3){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 5 && mon_cam == 2){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 5 && mon_cam == 6){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 5 && mon_cam == 7){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 5 && mon_cam == 8){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 6 && mon_cam == 5){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 6 && mon_cam == 7){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 7 && mon_cam == 6){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 7 && mon_cam == 8){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 8 && mon_cam == 7){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 8 && mon_cam == 5){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 8 && mon_cam == 9){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 9 && mon_cam == 8){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 9 && mon_cam == 10){lureDelay = (rand()%100); working = -2;}
            if(working >= 2 && lure_cam == 10 && mon_cam == 9){lureDelay = (rand()%100); working = -2;}
            action_selected = 0;
            total_turns = 0;
        }
        if(working == -2 || working == 1){
            if(working == 1){
                hasTped = true;
            }
            if(lureDelay == 0){
                if(!hasTped){
                    mon_cam = lure_cam;
                    CamStatic_alpha = 255;
                    hasTped = true;
                }
            }
        }
        if(lureCounter >= (90*6)){
            working = -1;
            canPlayLure = true;
            isLurePlaying = false;
            lure_cam = 0;
            lure_f = 0;
            lure_radio_f = 0;
            hasTped = false;
            lureDelay = 0;
            lureCounter = 0;
            lureLoad.loaded = false;
            FreeImgAnim(&lureLoad);
        }
        if(camButton_alpha == 50 && camLayoutSelection == 0){
            GRRLIB_DrawImg(260, 360, lureLoad.frames[lure_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
            if(lure_radio_f != 4){
                GRRLIB_DrawImg(cam_x_l[lure_cam-1]-50, cam_y_l[lure_cam-1]-15, lureLoad.frames[6+lure_radio_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
            }
        }
        if(frame%90==0 && lure_f < 5){
            lure_f++;
        }
        if(frame%30==0 && lure_radio_f < 4){
            lure_radio_f++;
        }
        lureCounter++;
    }
}

s32  my_reader(void *fp,void *dat, s32 size) // found this on a forum for the mp3 player thingy
{
	return fread(dat, 1, size, fp);
}

void Puppet_calc(){
    static int puppet_x = 0;
    static int puppet_y = 0;
    static int dir = -5;
    static int dir2 = -2;
    static int blackout_dir_puppet = 5;
    static int blackout_puppet_alpha = 0;
    static bool hasSoundPlayed = false;
    if((selected_cam != 8 || !isCamOpen) && hasPuppetSpawned){
        if(PuppetCamCounter > 0){
            PuppetCamCounter = 0;
            PuppetOfficeCounter = 0;
            hasPuppetSpawned = false;
            isPuppetAttacking = false;
        }
        return;
    }
    if(PuppetCamCounter >= BB_Ai[NIGHT-1]){
        if(!puppetLoaded){
            puppet = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/334.png");
            puppetLoaded = true;
        }
        isCamOpen = false;
        hasPuppetSpawned = false;
        isPuppetAttacking = true;
    }
    if(isPuppetAttacking){
        PuppetOfficeCounter++;
        GRRLIB_DrawImg(puppet_x, puppet_y, puppet, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        GRRLIB_Rectangle(0, 0, SCREEN_W, SCREEN_H, 0x00020f00 + blackout_puppet_alpha, true);
        if(!hasSoundPlayed){
            mask_sound = SM_LoadAudio("mask.wav");
            SM_PlayAudio(mask_sound, 127, 127, true, false, 0);
            hasSoundPlayed = true;
        }
        if(puppet_y == 0){
            x += dir;
            if(x<=-(HALF_REGION-20)){
                x = -(HALF_REGION-20);
                dir = 5;
            }else if(x >= 0){
                x = 0;
                dir = -5;
            }
            puppet_x += dir2;
            if (puppet_x <= -25) {
                puppet_x = -25;
                dir2 = 2;
            }
            else if (puppet_x >= 40) {
                puppet_x = 40;
                dir2 = -2;
            }
        }

        blackout_puppet_alpha += blackout_dir_puppet;
        if (blackout_puppet_alpha <= 50) {
            blackout_puppet_alpha = 50;
            blackout_dir_puppet = 5;
        }
        else if (blackout_puppet_alpha >= 255) {
            blackout_puppet_alpha = 255;
            blackout_dir_puppet = -5;
        }
    }
    if(PuppetOfficeCounter>=60*5){
        if(puppet_y < 480){
            puppet_y+=15;
            if(blackout_puppet_alpha <= 0){
                blackout_puppet_alpha = 0;
            }else{blackout_puppet_alpha-=5;}
        }else{isPuppetAttacking = false; blackout_puppet_alpha = 0; PuppetCamCounter = 0; PuppetOfficeCounter = 0; if(puppetLoaded){puppetLoaded = false; GRRLIB_FreeTexture(puppet);}}
        canPuppetSpawn = false;
        if(hasSoundPlayed){
            hasSoundPlayed = false;
            SM_FreeAudio(mask_sound);
        }
    }
    if(isCamOpen && selected_cam == 8 && hasPuppetSpawned) {PuppetCamCounter++;}
}

void chica_calc(){
    if((selected_cam != 7 || !isCamOpen) && hasChicaCamSpawned){
        if(chicaCounter > 0){
            chicaCounter = 0;
            hasChicaCamSpawned = false;
            canChicaSpawn = true;
        }
        return;
    }
    if(chicaCounter >= BB_Ai[NIGHT-1]){
        canChicaSpawn = false;
        hasChicaOfficeSpawned = true;
    }
    if(selected_cam == 7 && hasChicaCamSpawned && isCamOpen){chicaCounter++;}
}

int gameSceneMain(){
    char* night_text = read_line(SD_SAVE, 0);
    if (strcmp(night_text, "NIGHT_NMB=1") == 0) NIGHT = 1;
    else if (strcmp(night_text, "NIGHT=2") == 0) NIGHT = 2;
    else if (strcmp(night_text, "NIGHT=3") == 0) NIGHT = 3;
    else if (strcmp(night_text, "NIGHT=4") == 0) NIGHT = 4;
    else if (strcmp(night_text, "NIGHT=5") == 0) NIGHT = 5;
    else if (strcmp(night_text, "NIGHT=6") == 0) NIGHT = 6;
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Render();
    InitAllImg();
    srand(time(NULL));
    sceneTex = GRRLIB_CreateEmptyTexture(SCREEN_W, SCREEN_H);
    u64 buttonCamDelay = 0;
    picrandom = rand()%2;
    canFreddySpawn = true;
    if(NIGHT!=1){
        mon_cam = (rand() % 5) + 6;
    }
    FILE *night_call;
    SM_PlayAudio(tablefan, 127, 127, true, false, 0);
    SM_PlayAudio(Desolate_Underworld2, 127, 127, true, false, 0);
    if(NIGHT==1){
        night_call = fopen("sd:/fnaf3/assets/sounds/night1final.mp3", "r");
    }
    if(NIGHT==2){
        night_call = fopen("sd:/fnaf3/assets/sounds/night2final2.mp3", "r");
    }
    if(NIGHT==3){
        night_call = fopen("sd:/fnaf3/assets/sounds/night3final.mp3", "r");
    }
    if(NIGHT==4){
        night_call = fopen("sd:/fnaf3/assets/sounds/night4final.mp3", "r");
    }
    if(NIGHT==5){
        night_call = fopen("sd:/fnaf3/assets/sounds/night5final.mp3", "r");
    }
    if(NIGHT==6){
        night_call = fopen("sd:/fnaf3/assets/sounds/night6final.mp3", "r");
    }
    MP3Player_PlayFile(night_call, my_reader, NULL);
    //fclose(night_call);
    while (1)
    {
        if(time_am == 6){
            break;
        }
        WPAD_ScanPads();
        mote = WPAD_ButtonsDown(0);
        WPAD_IR(0, &ir);
        SM_UpdateAudio();
        IR_Y = ir.y * (480.0f / 460.0f);
        if(camButton_alpha == 99 || camButton_alpha == 50 || isJumpscare == true || isControlPanelOpen == 1 || IsBBJumpscaring || isPuppetAttacking || isChicaJumpscaring || isFoxyJumpscaring){
            canPan = false;
        }
        if(camButton_alpha != 99 && camButton_alpha != 50 && isJumpscare == false && isControlPanelOpen != 1 && !IsBBJumpscaring && !isFreddyJumpscaring && !isPuppetAttacking && !isChicaJumpscaring && !isFoxyJumpscaring){
            canPan = true;
        }
        if(isCamOpen || isControlPanelOpen){
            ScreenOpen = true;
        }
        if(!isCamOpen && !isControlPanelOpen){
            ScreenOpen = false;
        }
        if(hasBBSpawned){
            if(BB.loaded){
                BB.loaded = false;
                FreeImgAnim(&BB);
            }
        }
        if(play_effect == 1 && !isPlay1){
            vent_quiet1 = SM_LoadAudio("vent_quiet1.wav");
            SM_PlayAudio(vent_quiet1, 50, 50, false, true, 0);
            isPlay1 = true;
        }
        if(play_effect == 2 && !isPlay2){
            vent_quiet2 = SM_LoadAudio("vent_quiet2.wav");
            SM_PlayAudio(vent_quiet2, 50, 50, false, true, 0);
            isPlay2 = true;
        }
        if(play_effect == 3 && !isPlay3){
            vent_closer1 = SM_LoadAudio("vent_closer1.wav");
            SM_PlayAudio(vent_closer1, 50, 50, false, true, 0);
            isPlay3 = true;
        }
        if(play_effect == 4 && !isPlay4){
            vent_closer2 = SM_LoadAudio("vent_closer2.wav");
            SM_PlayAudio(vent_closer2, 50, 50, false, true, 0);
            isPlay4 = true;
        }
        if(mon_cam == 13 || mon_cam == 5 || mon_cam == 3 || mon_cam == 5){
            SM_ChangeVolume(danger2b, 50, 50);
        }else if(mon_cam == 1 || mon_cam == 2 || attack_stage > 0){
            SM_ChangeVolume(danger2b, 100, 100);
        }else{SM_ChangeVolume(danger2b, 0, 0);}
        //if(attack_stage > 0){mon_cam = 0;} // fail safe, might remove if its broken
        drawBackdrop();
        Stage1_2();
        Freddy_calc();
        Mangle_calc();
        drawOffice();
        drawFan();
        if(hasFoxySpawned){
            if(x >= -522 && x <= -450){
                hasSeenFoxy = true;
            }
            if(x > -450 && foxy_f != 7){
                isFoxyJumpscaring = true;
            }
            GRRLIB_DrawImg(x+420-(foxy.frames[foxy_f]->w / 2), SCREEN_H-(foxy.frames[foxy_f]->h), foxy.frames[foxy_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00+foxy_alpha);
            if(isFoxyJumpscaring){
                canFoxySpawn = false;
                if(!foxy.loaded){
                    foxy.loaded = false;
                    SM_PlayAudio(scream3, 200, 200, false, false, 0);
                }
                if(x < -115){
                    if(x>-115){x = -115;}
                    x+=20;
                }
            }
            if(frame%7==0 && isFoxyJumpscaring && foxy_f < 7){
                foxy_f++;
            }
            if(foxy_f == 7){
                isFoxyJumpscaring = false;
                if(foxy_alpha > 0){
                    ventHealth = 0;
                    foxy_alpha-=5;
                }else{if(foxy.loaded){foxy.loaded = false; FreeImgAnim(&foxy);}}
            }
        }
        GRRLIB_Screen2Texture(0, 0, sceneTex, false);
        DrawWarpedScene(); //anything drew before this are getting warped
        drawPanelButton();
        drawControlPanel();
        BB_calc();
        Puppet_calc();
        chica_calc();
        if(hasChicaOfficeSpawned && x >= -500){
            hasChicaOfficeSpawned = false;
            isChicaJumpscaring = true;
        }
        if(isChicaJumpscaring){
            if(!chica.loaded){
                chica.loaded = true;
                SM_PlayAudio(scream3, 200, 200, false, false, 0);
            }
            if(x < 0){
                if(x > 0){x = 0;}
                x+=20;
            }
            GRRLIB_DrawImg(x, SCREEN_H-(chica.frames[chica_f]->h), chica.frames[chica_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00+chica_alpha);
            if(frame%5==0 && chica_f < 8){
                chica_f++;
            }
            if(chica_f==8){
                chica_alpha-=5;
                ventHealth = 0;
            }
            if(chica_alpha <= 0){
                if(chica.loaded){
                    chica.loaded = false;
                    FreeImgAnim(&chica);
                }
                isChicaJumpscaring = false;
                canChicaSpawn = false;
                hasChicaCamSpawned = false;
                hasChicaOfficeSpawned = false;
            }
        }
        drawCamPanel();
        drawCams();
        if(selected_cam == BBSpawn && isCamOpen && !videoError){
            if(!BBcamLoaded){
                bb_cam = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/70.png");
                BBcamLoaded = true;
            }
            GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, bb_cam, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }else{
            if(BBcamLoaded){
                BBcamLoaded = false;
                GRRLIB_FreeTexture(bb_cam);
            }
        }
        drawCamStatic();
        if(isSealing && !ventilationError){
            sealVent();
        }
        if(ventilationError){
            closed_vent = 0;
        }
        if(blip_f < 4){
            drawBlip();
        }
        if(isFreddyJumpscaring){
            canFreddySpawn = false;

            if(!freddy_jumpscare.loaded){
                freddy_jumpscare.frames[0] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/362.png");
                freddy_jumpscare.frames[1] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/475.png");
                freddy_jumpscare.frames[2] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/477.png");
                freddy_jumpscare.frames[3] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/479.png");
                freddy_jumpscare.frames[4] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/481.png");
                freddy_jumpscare.frames[5] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/483.png");
                //freddy_jumpscare.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/485.png");
                freddy_jumpscare.frames[6] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/491.png");
                //freddy_jumpscare.frames[8] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/499.png");
                //freddy_jumpscare.frames[9] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/504.png");
                freddy_jumpscare.frames[7] = GRRLIB_FakeLoad("sd:/fnaf3/assets/images/769.png");
                freddy_jumpscare.loaded = true;
            }
            if(freddyjumpscare_f == 0){
                SM_PlayAudio(scream3, 200, 200, false, false, 0);
            }
            GRRLIB_DrawImg(0, SCREEN_H - (freddy_jumpscare.frames[freddyjumpscare_f]->h), freddy_jumpscare.frames[freddyjumpscare_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + freddy_alpha);
        }
        if(freddyjumpscare_f >= 7 && isFreddyJumpscaring){
            ventHealth = 0;
            if(freddy_alpha > 0){
                freddy_alpha-=5;
            }else{isFreddyJumpscaring = false; canFreddySpawn = false; if(freddy_jumpscare.loaded){freddy_jumpscare.loaded = false; FreeImgAnim(&freddy_jumpscare);}}
        }
        if(frame%5==0 && freddyjumpscare_f < 7 && isFreddyJumpscaring){
        //    freddyjumpscare_f++;
            freddyjumpscare_f = freddyjumpscare_f + 1;
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
        if(canMute){
            GRRLIB_DrawImg(20, 20, mute, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        }else{
            if(!destroyedMute){
                GRRLIB_FreeTexture(mute);
                stop_sound = SM_LoadAudio("stop.wav");
                SM_PlayAudio(stop_sound, 50, 50, false, true, 0);
                destroyedMute = true;
            }
        }
        drawCamLayout();
        drawCamButton();
        if(isLurePlaying){
            playLure();
        }
        
        //sprintf(buffer, "%d, %d, %d, %d, %d, %d", mon_cam, action_selected, attack_stage, total_turns, isCamOpen, freddy_f);
        //sprintf(buffer, "%d, %d, %d, %d, %d", isRebootingAudio, isRebootingCam, isRebootingVent, isRebootingAll, isRebooting);
        //struct mallinfo mi = mallinfo();
        //float used_heap = (float)mi.uordblks / 1024 / 1024;
        //float free_heap = (float)mi.fordblks / 1024 / 1024;
        //sprintf(buffer, "%.2f, %.2f", used_heap, free_heap);
        //sprintf(buffer, "%f", x);
        //sprintf(buffer, "%d, %d, %d, %d", hasFreddyDukedDown, isFreddyJumpscaring, freddyjumpscare_f, freddy_alpha);
        //GRRLIB_Printf(10, 10, font, 0xFFFFFFFF, IMG_SCALE, buffer);
        if(mote & WPAD_BUTTON_A){
            if(canPanelButtonBeClicked){
                if(IsCursorWithinImg(panelClick, CONTROL_PANEL_X - (panelClick->w / 2), CONTROL_PANEL_Y - (panelClick->h))){
                    isControlPanelOpen = 1;
                    lever1 = SM_LoadAudio("lever1.wav");
                    SM_PlayAudio(lever1, 127, 127, false, true, 0);
                }
            }
            if(!PanelButtonVisible && !isRebooting){
                if(panelSelect == 4){
                    isControlPanelOpen = 2;
                    lever2 = SM_LoadAudio("lever2.wav");
                    SM_PlayAudio(lever2, 127, 127, false, true, 0);
                }
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
                    crank1 = SM_LoadAudio("crank1.wav");
                    SM_PlayAudio(crank1, 127, 127, false, true, 0);
                    isCamOpen = !isCamOpen;
                    if(NIGHT == 2){foxy_chance = (rand()%1000)+1;}
                    if(NIGHT == 3){foxy_chance = (rand()%50)+1;}
                    if(NIGHT == 4){foxy_chance = (rand()%25)+1;}
                    if(NIGHT >= 5){foxy_chance = (rand()%10)+1;}
                    if(foxy_chance == 1 && canFoxySpawn && !hasFoxySpawned){
                        hasFoxySpawned = true;
                    }
                }
            }
            if(camButton_alpha == 50){
                if(IsCursorWithinImg(camClick, 540, 73)){
                    crank2 = SM_LoadAudio("crank2.wav");
                    SM_PlayAudio(crank2, 127, 127, false, true, 0);
                    isCamOpen = !isCamOpen;
                    if(hasSeenFoxy){hasFoxySpawned = false; hasSeenFoxy = false;}
                }
                for(int i = 0; i < 10; i++){
                    if(camLayoutSelection == 0){
                        if(IsCursorWithinImg(CamLayoutNmb.frames[0], cam_x_l[i], cam_y_l[i]) && buttonCamDelay >= 20){
                            selected_cam = i + 1;
                            blip_f = 0;
                            CamStatic_alpha = 255;
                            buttonCamDelay = 0;
                        }  
                    } 
                }
                for(int i = 0; i < 5; i++){
                    if(camLayoutSelection == 1 && !isSealing){
                        if(IsCursorWithinImg(CamLayoutNmb.frames[0], cam_x_l[i + 10], cam_y_l[i + 10]) && buttonCamDelay >= 20){
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
                if(IsCursorWithinImg(CamLayoutNmb.frames[19], 268 + ((CamLayoutNmb.frames[17]->w-CamLayoutNmb.frames[19]->w)/2), 381+((CamLayoutNmb.frames[17]->h-CamLayoutNmb.frames[19]->h)/2)) && camButton_alpha == 50){ // for map toggle, SON :sob:
                    SM_PlayAudio(select_sound, 127, 127, false, false, 0);
                    if(camLayoutSelection == 0){
                        selected_cam = 11;
                    }else{selected_cam = 2;}
                    camLayoutSelection = !camLayoutSelection;
                }
                if(IsCursorWithinImg(CamLayoutNmb.frames[18], 268 + ((CamLayoutNmb.frames[17]->w-CamLayoutNmb.frames[18]->w)/2), 332+((CamLayoutNmb.frames[17]->h-CamLayoutNmb.frames[18]->h)/2)) && canPlayLure && camLayoutSelection == 0 && !isLurePlaying && isCamOpen && audioHealth > 0 && !audioError){ // for audio lures
                    if(echo == 0){
                        int echo_sound = SM_LoadAudio("echo1.wav");
                        SM_PlayAudio(echo_sound, 100, 100, false, true, 0);
                    }
                    if(echo == 1){
                        int echo_sound = SM_LoadAudio("echo3b.wav");
                        SM_PlayAudio(echo_sound, 100, 100, false, true, 0);
                    }
                    if(echo == 2){
                        int echo_sound = SM_LoadAudio("echo4b.wav");
                        SM_PlayAudio(echo_sound, 100, 100, false, true, 0);
                    }
                    echo = (echo + 1) % 3;
                    isLurePlaying = true;
                    audioHealth -= Springtrap_AI[NIGHT-1];
                }
            }
            if(IsCursorWithinImg(mute, 20, 20) && canMute){
                MP3Player_Stop();
                canMute = false;
            }
        }
        drawPanelText();
        if(isJumpscare){
            jumpscare();
        }
        if(!MP3Player_IsPlaying()){
            canMute = false;
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
            }
        }
        if(frame%60==0){ //global each second timer
            VentErrorTimer++;
            if(NIGHT == 2 && VentErrorTimer >= (60*12)) {ventHealth--; VentErrorTimer = 0;}
            if(NIGHT == 3 && VentErrorTimer >= (60*10)) {ventHealth--; VentErrorTimer = 0;}
            if(NIGHT == 4 && VentErrorTimer >= (60*9)) {ventHealth--; VentErrorTimer = 0;}
            if(NIGHT == 5 && VentErrorTimer >= (60*8)) {ventHealth--; VentErrorTimer = 0;}
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
            if(hasFreddyDukedDown && !isFreddyJumpscaring){
                if((rand() % 2) == 0){
                    freddyjumpscare_f = 0;
                    freddy_alpha = 255;
                    isFreddyJumpscaring = true;
                    hasFreddyDukedDown = false;
                }
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
        if(frame%(60*20)==0){ // global office 20 seconds timer
            if(!hasBBSpawned && ((rand() % 11) + 1) <= Springtrap_AI[NIGHT-1] && time_am >= 1 && time_am < 6 && BBSpawn == -1 && !videoError && !ScreenOpen && NIGHT != 1){ // BB has an AI in 10 chance to spawn ever 20 seconds if you are in the office
                u8 bb_cams[4] = {1, 7, 9, 10};
                BBSpawn = bb_cams[(rand() % 4)];
            }
            if(canMangleSpawn && ((rand()%8)+1) <= Springtrap_AI[NIGHT-1] && NIGHT!=1 && !videoError && time_am != 12){
                hasMangleSpawned = true;
            }
            if(canPuppetSpawn && selected_cam != 8 && !hasPuppetSpawned && !isPuppetAttacking && time_am != 12 && (rand()%10)+1 <= Springtrap_AI[NIGHT-1] && NIGHT >= 4){
                hasPuppetSpawned = true;
            }
            if(canChicaSpawn && selected_cam != 7 && !hasChicaCamSpawned && !isChicaJumpscaring && time_am != 12 && (rand()%10)+1 <= Springtrap_AI[NIGHT-1] && NIGHT >= 3){
                hasChicaCamSpawned = true;
            }
        }
        if(frame%(60*60)==0){ // global 1 minute timer
            if((rand()%12)+1 <= Springtrap_AI[NIGHT-1] && time_am != 12 && NIGHT >= 3 && canFreddySpawn){
                hasFreddySpawned = true;
            }
        }
        if(time_am == 3 && !hasBBSpawned && BBSpawn == -1 && !videoError && !isCamOpen && NIGHT!=1){ // force spawn bb at 3 am
            u8 bb_cams[4] = {1, 7, 9, 10};
            BBSpawn = bb_cams[(rand() % 4)];
        }
        if(time_am == 5 && canMangleSpawn && !videoError && NIGHT!=1){ // force spawn mangle at 5 am
            hasMangleSpawned = true;
        }
        if(time_am == 4 && NIGHT >= 3 && canFreddySpawn && !hasFreddySpawned){ // force spawn freddy at 4 am
            hasFreddySpawned = true;
        }
        if(time_am == 5 && NIGHT >= 4 && canPuppetSpawn && !hasPuppetSpawned && !isPuppetAttacking){ // force spawn puppet at 5 am
            hasPuppetSpawned = true;
        }
        if(time_am == 5 && canChicaSpawn && !hasChicaCamSpawned && !isChicaJumpscaring && NIGHT >= 3){
            hasChicaCamSpawned = true;
        }
        // springtrap go to attack stage 2 when: looking at any screen and action selected = 2

        if(time_am >= 4){
            aggresive = true;
        }
        if(lureDelay > 0){
            lureDelay--;
        }
        if(time_am == 6){break;}
        if(ventilationError){
            static u8 blackout_dir = 1;
            blackout_alpha += blackout_dir;
            if (blackout_alpha <= 50) {
                blackout_alpha = 50;
                blackout_dir = 1;
            }
            else if (blackout_alpha >= 250) {   
                blackout_alpha = 250;
                blackout_dir = -1;
            }
        }else{if(blackout_alpha > 0){blackout_alpha--;}}
        GRRLIB_Rectangle(0, 0, SCREEN_W, SCREEN_H, 0x0002000+blackout_alpha, true);
        if(!ScreenOpen && !ventilationError){
            VentErrorTimer2++;
            if(frame%60==0 && VentErrorTimer2 >= (60*10)){ventHealth--;}
        }
        if(ScreenOpen){
            VentErrorTimer2 = 0;
        }
        drawCursor();
        GRRLIB_Render();
        if(time_am == 6){nextScene = 1; break;}
        if(nextScene != -1) break;
        frame++;
        buttonCamDelay++;
        double_click_timer_vent++;
    }
    SM_FreeAudio(tablefan);
    SM_FreeAudio(Desolate_Underworld2);
    SM_FreeAudio(static_sound);
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
    FreeImgAnim(&freddy);
    FreeImgAnim(&freddy_jumpscare);
    FreeImgAnim(&lureLoad);
    GRRLIB_FreeTexture(bb_cam);
    GRRLIB_FreeTexture(puppet);
    GRRLIB_FreeTexture(mute);
    GRRLIB_FreeTexture(font);
    GRRLIB_FreeTexture(cursor);
    GRRLIB_FreeTexture(mangle);
    FreeImgAnim(&foxy);
    FreeImgAnim(&chica);
    SM_FreeAllAudio();
    return 0;
}