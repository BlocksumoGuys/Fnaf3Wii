#include "main.h"
#define L_PAN_W 214
#define H_PAN_W 130
#define PAN_SPEED 8
#define HALF_REGION 624
#define SCREEN_W 640
#define SCREEN_H 480
#define CONTROL_PANEL_X 266
#define CONTROL_PANEL_Y 453
float x = 0;
float tx = 0.0f;
ir_t ir;
u8 isControlPanelOpen = 0;
bool PanelButtonVisible = true;
bool canPanelButtonBeClicked = true;
bool canPan = true;
bool isCamOpen = false;
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
u8 CamPanel_f = 0;
u8 CamPanel_alpha = 255;
GRRLIB_texImg *cursor = NULL;
GRRLIB_texImg *font = NULL;
GRRLIB_texImg *panelClick = NULL;
GRRLIB_texImg *camClick = NULL;
u8 camButton_alpha = 100;
bool IsCursorWithinImg(GRRLIB_texImg *img, int x, int y){
    if(ir.x >= x && ir.x <= x + img->w && IR_Y >= y && IR_Y <= y + img->h){return true;}
    return false;
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
    CamPanel.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/74.png");
    CamPanel.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/72.png");
    CamPanel.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/71.png");
    CamPanel.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/69.png");
    CamPanel.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/68.png");
    CamPanel.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/63.png");
    CamPanel.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/62.png");
    CamPanel.frames[7] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/61.png");
    CamPanel.frames[8] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/60.png");
    CamPanel.frames[9] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/59.png");    
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
    GRRLIB_DrawImg(x+ (office.frames[1]->w), 0, office.frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    if(DEBUG){
        GRRLIB_Rectangle(0, 0, L_PAN_W, 480, 0xFF00FFFF, true); // light left pan
        GRRLIB_Rectangle(640 - L_PAN_W, 0, L_PAN_W, 480, 0xFF00FFFF, true); // ligth right pan
        GRRLIB_Rectangle(0, 0, H_PAN_W, 480, 0x680068FF, true); // heavy left pan
        GRRLIB_Rectangle(640 - H_PAN_W, 0, H_PAN_W, 480, 0x680068FF, true); // heavy right pan
    }
    // size of pans: 160x480
    if(!canPan) return;
    if(tx < 0 && ir.x <= L_PAN_W){
        tx+=PAN_SPEED;
    }
    if(tx >= -HALF_REGION && ir.x > (SCREEN_W - L_PAN_W)){
        tx-=PAN_SPEED;
    }
    if(tx < 0 && ir.x <= H_PAN_W){
        tx+=PAN_SPEED;
    }
    if(tx >= -HALF_REGION && ir.x > (SCREEN_W - H_PAN_W)){
        tx-=PAN_SPEED;
    }

    if(tx <= -HALF_REGION){
        tx = -HALF_REGION;
    }
    if(tx >= 0){
        tx = 0;
    }
    x = lerp(x, tx, .25f);
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
    GRRLIB_DrawImg(540, 67, camClick, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + camButton_alpha);
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

int gameSceneMain(){
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Render();
    InitAllImg();
    char buffer[100];
    while (1)
    {
        WPAD_ScanPads();
        mote = WPAD_ButtonsDown(0);
        WPAD_IR(0, &ir);
        IR_Y = ir.y * (480.0f / 460.0f);
        drawOffice();
        drawFan();   
        drawPanelButton();
        drawControlPanel();
        drawCamPanel();
        drawCamButton();
        sprintf(buffer, "%lld", frame);
        //GRRLIB_Printf(10, 10, font, 0xFFFFFFFF, .4, buffer);
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
            if(camButton_alpha == 100 || camButton_alpha == 50){
                if(IsCursorWithinImg(camClick, 540, 67)){
                    isCamOpen = !isCamOpen;
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
        if(time_am == 6){break;}
        drawCursor();
        GRRLIB_Render();
        frame++;
    }
    GRRLIB_FreeTexture(panelClick);
    GRRLIB_FreeTexture(camClick);
    FreeImgAnim(&office);
    FreeImgAnim(&fan);
    FreeImgAnim(&controlPanel);
    FreeImgAnim(&PanelCommands);
    FreeImgAnim(&NightTimeFont);
    FreeImgAnim(&CamPanel);
    return 0;
}