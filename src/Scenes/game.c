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
u8 CamStatic_alpha = 100;
GRRLIB_texImg *cursor = NULL;
GRRLIB_texImg *font = NULL;
GRRLIB_texImg *panelClick = NULL;
GRRLIB_texImg *camClick = NULL;
GRRLIB_texImg *sceneTex = NULL;
u8 camButton_alpha = 100;
u8 selected_cam = 2;
const u8 Springtrap_AI[6] = {0, 2, 3, 4, 5, 7};
u8 camHealth = 0;
u8 CamErrorTimer = 0;
bool MapLayoutNormalMode = false;
bool IsCursorWithinImg(GRRLIB_texImg *img, int x, int y){
    if(ir.x >= x && ir.x <= x + img->w && IR_Y >= y && IR_Y <= y + img->h){return true;}
    return false;
}

void DrawWarpedScene(){
    GRRLIB_Screen2Texture(0, 0, sceneTex, false);
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
    office.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/203(1).png"); // normal 1
    office.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/203(2).png"); // normal 2
    office.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/204(1).png"); // alarm 1
    office.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/204(2).png"); // alarm 2
    fan.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/18.png");
    fan.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/19.png");
    fan.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/20.png");
    controlPanel.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/744.png");
    controlPanel.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/743.png");
    controlPanel.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/742.png");
    controlPanel.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/741.png");
    controlPanel.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/740.png");
    controlPanel.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/739.png");
    controlPanel.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/738.png");
    controlPanel.frames[7] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/737.png");
    controlPanel.frames[8] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/736.png");
    controlPanel.frames[9] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/735.png");
    controlPanel.frames[10] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/734.png");
    PanelCommands.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/599.png"); // "system restart menu"
    PanelCommands.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/544.png"); // "audio devices"
    PanelCommands.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/545.png"); // "camera system"
    PanelCommands.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/555.png"); // "ventilation"
    PanelCommands.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/594.png"); // "reboot all"
    PanelCommands.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/600.png"); // "exit"
    PanelCommands.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/596.png"); // green select cursor
    NightTimeFont.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/746.png"); // 0
    NightTimeFont.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/747.png"); // 1
    NightTimeFont.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/748.png"); // 2
    NightTimeFont.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/749.png"); // 3
    NightTimeFont.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/750.png"); // 4
    NightTimeFont.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/751.png"); // 5
    NightTimeFont.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/752.png"); // 6
    NightTimeFont.frames[7] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/245.png"); // "night" text
    NightTimeFont.frames[8] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/246.png"); // "am" text
    CamPanel.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/74.png"); // normal grey boxes
    CamPanel.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/72.png"); // cam num
    CamPanel.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/71.png");
    CamPanel.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/69.png");
    CamPanel.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/68.png");
    CamPanel.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/63.png");
    CamPanel.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/62.png");
    CamPanel.frames[7] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/61.png");
    CamPanel.frames[8] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/60.png");
    CamPanel.frames[9] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/59.png");
    CamLayout.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/532.png"); // cam layout
    CamLayout.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/626.png"); // vent layout
    CamLayout.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/574.png"); // cam layout 2
    CamLayout.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/627.png"); // vent layout 2
    CamLayoutNmb.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/81.png");
    CamLayoutNmb.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/83.png"); // ALL IS CAM NUM
    CamLayoutNmb.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/84.png");
    CamLayoutNmb.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/85.png");
    CamLayoutNmb.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/86.png");
    CamLayoutNmb.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/87.png");
    CamLayoutNmb.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/88.png");
    CamLayoutNmb.frames[7] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/89.png");
    CamLayoutNmb.frames[8] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/90.png");
    CamLayoutNmb.frames[9] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/91.png");
    CamLayoutNmb.frames[10] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/92.png");
    CamLayoutNmb.frames[11] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/613.png");
    CamLayoutNmb.frames[12] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/614.png");
    CamLayoutNmb.frames[13] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/617.png");
    CamLayoutNmb.frames[14] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/619.png");
    CamLayoutNmb.frames[15] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/621.png"); // cam num
    CamLayoutNmb.frames[16] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/82.png"); // normal grey boxes but selected so its green
    CamLayoutNmb.frames[17] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/426.png"); // big grey boxes
    CamLayoutNmb.frames[18] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/366.png"); // play audio text
    CamLayoutNmb.frames[19] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/431.png"); // map toggle text
    triangle.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/95.png");
    triangle.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/93.png");
    triangle.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/96.png");
    triangle.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/94.png");
    CamStatic.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/291.png");
    CamStatic.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/292.png");
    CamStatic.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/33.png");
    cursor = GRRLIB_LoadTexture(pointer_png);
    font = GRRLIB_LoadTexture(font_png);
    panelClick = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/77.png");
    camClick = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/32.png");
    GRRLIB_InitTileSet(cursor, 32, 32, 32);
    GRRLIB_InitTileSet(font, 32, 36, 32);
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
        canPan = false;
        GRRLIB_DrawImg(10, SCREEN_H - (controlPanel.frames[controlPanel_f]->h) + 5, controlPanel.frames[controlPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        if(frame%2==0){
            if(controlPanel_f!= 10){
                controlPanel_f+=1;
                canPanelButtonBeClicked = false;           
            }
            else{PanelButtonVisible = false;}
        }
    }
    else if(isControlPanelOpen == 2){
        GRRLIB_DrawImg(10, SCREEN_H - (controlPanel.frames[controlPanel_f]->h) + 5, controlPanel.frames[controlPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        PanelButtonVisible = true;
        canPanelButtonBeClicked = false;
        if(frame%2==0){
            if(controlPanel_f!=0){
                controlPanel_f-=1;
            }
            else{canPanelButtonBeClicked = true; canPan = true; isControlPanelOpen = 0;}
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
    GRRLIB_DrawImg(x, 0, office.frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(x+(office.frames[1]->w), 0, office.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
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
    if(frame)
    GRRLIB_DrawImg(540, 73, camClick, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + camButton_alpha);
}

void drawCamPanel(){
    if(isCamOpen){
        canPan = false;
        CamPanel_alpha = 255;
        GRRLIB_DrawImg(SCREEN_W - (CamPanel.frames[CamPanel_f]->w), 60, CamPanel.frames[CamPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00+CamPanel_alpha);
        if(frame%2==0){
            if(CamPanel_f < 9){
                CamPanel_f+=1;
                camButton_alpha = 99;
            }else{camButton_alpha = 50;}
        }
    }else{
        
        GRRLIB_DrawImg(SCREEN_W - (CamPanel.frames[CamPanel_f]->w), 60, CamPanel.frames[CamPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00+CamPanel_alpha);
        if(frame%2==0){
            if(CamPanel_f > 0){
                CamPanel_f-=1;
                camButton_alpha = 99;
            }else{camButton_alpha = 100; canPan = true; CamPanel_alpha = 0;}
        }
    }
}

void drawCamStatic(){
    if(camButton_alpha!=50) return;
    static u8 CamStatic_f = 0;
    GRRLIB_DrawImg(SCREEN_W - (CamStatic.frames[0]->w * 825.0f/1024.0f) - 20, 68, CamStatic.frames[CamStatic_f], 0, 825.0f/1021.2f, 650.0f/768.0f, 0xFFFFFF00+CamStatic_alpha);
    CamStatic_f = (CamStatic_f+1)%3;
    if(CamStatic_alpha>50 && frame%2==0){
        CamStatic_alpha--;
    }
}

void drawCamLayout(){
    if(camButton_alpha != 50) return;
    drawCamStatic();
    int y_pad = 0;
    int x_pad = -30;
    u16 x_l[10] = {391 + x_pad, 525 + x_pad, 577 + x_pad, 577 + x_pad, 475 + x_pad, 387 + x_pad, 387 + x_pad, 454 + x_pad, 495 + x_pad, 562 + x_pad};
    u16 y_l[10] = {407 + y_pad, 391 + y_pad, 364 + y_pad, 324 + y_pad, 333 + y_pad, 338 + y_pad, 297 + y_pad, 283 + y_pad, 257 + y_pad, 280 + y_pad};
    if(MapLayoutNormalMode == 0){
        GRRLIB_DrawImg(356+x_pad, 222+y_pad, CamLayout.frames[camLayoutSelection], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for the layout       
    }else{GRRLIB_DrawImg(356+x_pad, 222+y_pad, CamLayout.frames[camLayoutSelection + 1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);} // for the layout
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
    GRRLIB_DrawImg(298+x_pad, 332+y_pad, CamLayoutNmb.frames[17], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // for big buttons
    GRRLIB_DrawImg(298+x_pad, 381+y_pad, CamLayoutNmb.frames[17], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(298+x_pad + ((CamLayoutNmb.frames[17]->w-CamLayoutNmb.frames[18]->w)/2), 332+y_pad+((CamLayoutNmb.frames[17]->h-CamLayoutNmb.frames[18]->h)/2), CamLayoutNmb.frames[18], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // this is the right way to do it
    GRRLIB_DrawImg(298+x_pad + ((CamLayoutNmb.frames[17]->w-CamLayoutNmb.frames[19]->w)/2), 381+y_pad+((CamLayoutNmb.frames[17]->h-CamLayoutNmb.frames[19]->h)/2), CamLayoutNmb.frames[19], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF); // end big buttons
}

int gameSceneMain(){
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Render();
    InitAllImg();
    s8 x_pad = -30; // FOR CAM LAYOUT BUTTONS
    s8 y_pad = 0; // FOR CAM LAYOUT BUTTONS
    u16 x_l[10] = {391 + x_pad, 525 + x_pad, 577 + x_pad, 577 + x_pad, 475 + x_pad, 387 + x_pad, 387 + x_pad, 454 + x_pad, 495 + x_pad, 562 + x_pad}; // FOR CAM LAYOUT BUTTONS
    u16 y_l[10] = {407 + y_pad, 391 + y_pad, 364 + y_pad, 324 + y_pad, 333 + y_pad, 338 + y_pad, 297 + y_pad, 283 + y_pad, 257 + y_pad, 280 + y_pad}; // FOR CAM LAYOUT BUTTONS
    char buffer[100];
    u8 CamStaticRandom = (rand()%15) + 1;
    sceneTex = GRRLIB_CreateEmptyTexture(SCREEN_W, SCREEN_H);
    while (1)
    {
        WPAD_ScanPads();
        mote = WPAD_ButtonsDown(0);
        WPAD_IR(0, &ir);
        IR_Y = ir.y * (480.0f / 460.0f);
        drawOffice();
        drawFan();
        DrawWarpedScene(); //anything drew before this are getting warped
        drawPanelButton();
        drawControlPanel();
        drawCamPanel();
        drawCamLayout();
        drawCamButton();
        if(DEBUG){
            sprintf(buffer, "X: %d \n mX: %.1f, mY: %.1f", x, ir.x, IR_Y);
            GRRLIB_Printf(10, 10, font, 0xFFFFFFFF, .4, buffer);
        }
        if(mote & WPAD_BUTTON_A){
            if(canPanelButtonBeClicked){
                if(IsCursorWithinImg(panelClick, CONTROL_PANEL_X - (panelClick->w / 2), CONTROL_PANEL_Y - (panelClick->h))){isControlPanelOpen = 1;}
            }
            if(!PanelButtonVisible){
                if(panelSelect == 4){isControlPanelOpen = 2;}
            }
            if(camButton_alpha == 100){
                if(IsCursorWithinImg(camClick, 540, 73)){
                    isCamOpen = !isCamOpen;
                }
            }
            if(camButton_alpha == 50){
                if(IsCursorWithinImg(camClick, 540, 73)){
                    isCamOpen = !isCamOpen;
                }
                for(int i = 0; i < 10; i++){
                    if(IsCursorWithinImg(CamLayoutNmb.frames[0], x_l[i], y_l[i])){
                        selected_cam = i + 1;
                        CamStatic_alpha = 255;
                    }   
                }             
            }
        }
        drawPanelText();
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
            CamErrorTimer++;
            if(CamErrorTimer >= (60*12)){ // each 12 seconds, springtrap's AI level is deducted from the cam's health
                camHealth -= Springtrap_AI[NIGHT - 1];
                CamErrorTimer = 0;
            }
        }
        if(frame%60==0){ //global each second timer
            MapLayoutNormalMode = !MapLayoutNormalMode;
        }
        if(frame%60*CamStaticRandom==0){
            CamStatic_alpha = (rand()%50) + 51;
            CamStaticRandom = (rand()%15) + 1;
        }
        if(time_am == 6){break;}
        drawCursor();
        GRRLIB_Render();
        frame++;
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
    return 0;
}