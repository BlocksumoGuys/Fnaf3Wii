#include "main.h"
#define L_PAN_W 214
#define H_PAN_W 130
#define PAN_SPEED 8
#define HALF_REGION 624
#define SCREEN_W 640
#define SCREEN_H 480
#define CONTROL_PANEL_X 266
#define CONTROL_PANEL_Y 453
int x = 0;
ir_t ir;
u8 isControlPanelOpen = 0;
bool PanelButtonVisible = true;
bool canPanelButtonBeClicked = true;
bool canPan = true;
u8 panelSelect = 0;
float IR_Y;
u8 time_am = 12;

bool IsCursorWithinImg(GRRLIB_texImg *img, int x, int y){
    if(ir.x >= x && ir.x <= x + img->w && IR_Y >= y && IR_Y <= y + img->h){return true;}
    return false;
}

void drawNightInf(IMG_Anim *img){
    GRRLIB_DrawImg(570, 20, img->frames[7], 0, IMG_SCALE + .4, IMG_SCALE + .4, 0xFFFFFFFF);
    GRRLIB_DrawImg(570+50, 20-2.5, img->frames[NIGHT], 0, .6, .6, 0xFFFFFFFF);
    u16 tx = 590;
    u16 ty = 35;
    GRRLIB_DrawImg(612, ty + 5, img->frames[8], 0, IMG_SCALE + .4, IMG_SCALE + .4, 0xFFFFFFFF);
    if(time_am!=12){
        GRRLIB_DrawImg(tx, ty, img->frames[time_am], 0, .6, .6, 0xFFFFFFFF);
    }
    else{
        GRRLIB_DrawImg(tx, ty, img->frames[1], 0, .6, .6, 0xFFFFFFFF);
        GRRLIB_DrawImg(tx + (img->frames[1]->w/2), ty, img->frames[2], 0, .6, .6, 0xFFFFFFFF);        
    }
}

void drawCursor(GRRLIB_texImg *img){
    float y = ir.y * (480.0f / 460.0f);
    GRRLIB_DrawImg(ir.x, y, img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
}

void drawFan(IMG_Anim* fan, u8 *fan_f, u64 *frame){
    GRRLIB_DrawImg(x+HALF_REGION + 100, 180, fan->frames[*fan_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    if(*frame%2==0){
        *fan_f = (*fan_f + 1) % 3;
    }
}

void drawControlPanel(IMG_Anim *img, u8 *controlPanel_f, u64 *frame){
    if(isControlPanelOpen == 1){
        canPan = false;
        GRRLIB_DrawImg(10, SCREEN_H - (img->frames[*controlPanel_f]->h) + 5, img->frames[*controlPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        if(*frame%2==0){
            if(*controlPanel_f!= 10){
                *controlPanel_f+=1;
                canPanelButtonBeClicked = false;           
            }
            else{PanelButtonVisible = false;}
        }
    }
    else if(isControlPanelOpen == 2){
        GRRLIB_DrawImg(10, SCREEN_H - (img->frames[*controlPanel_f]->h) + 5, img->frames[*controlPanel_f], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        PanelButtonVisible = true;
        canPanelButtonBeClicked = false;
        if(*frame%2==0){
            if(*controlPanel_f!=0){
                *controlPanel_f-=1;
            }
            else{canPanelButtonBeClicked = true; canPan = true; isControlPanelOpen = 0;}
        }
    }
}

void drawPanelButton(GRRLIB_texImg *img){
    if(x == 0){
        if(PanelButtonVisible){
            GRRLIB_DrawImg(CONTROL_PANEL_X - (img->w / 2), CONTROL_PANEL_Y - (img->h), img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00 + 127);
        }else{
            GRRLIB_DrawImg(CONTROL_PANEL_X - (img->w / 2), CONTROL_PANEL_Y - (img->h), img, 0, IMG_SCALE, IMG_SCALE, 0xFFFFFF00);
        }
    }
}

void drawOffice(IMG_Anim* office, u32 *mote){
    GRRLIB_DrawImg(x, 0, office->frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(x+ (office->frames[1]->w), 0, office->frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    if(DEBUG){
        GRRLIB_Rectangle(0, 0, L_PAN_W, 480, 0xFF00FFFF, true); // light left pan
        GRRLIB_Rectangle(640 - L_PAN_W, 0, L_PAN_W, 480, 0xFF00FFFF, true); // ligth right pan
        GRRLIB_Rectangle(0, 0, H_PAN_W, 480, 0x680068FF, true); // heavy left pan
        GRRLIB_Rectangle(640 - H_PAN_W, 0, H_PAN_W, 480, 0x680068FF, true); // heavy right pan
    }
    // size of pans: 160x480
    if(!canPan) return;
    if(x < 0 && ir.x <= L_PAN_W){
        x+=PAN_SPEED;
    }
    if(x >= -HALF_REGION && ir.x > (SCREEN_W - L_PAN_W)){
        x-=PAN_SPEED;
    }
    if(x < 0 && ir.x <= H_PAN_W){
        x+=PAN_SPEED;
    }
    if(x >= -HALF_REGION && ir.x > (SCREEN_W - H_PAN_W)){
        x-=PAN_SPEED;
    }

    if(x <= -HALF_REGION){
        x = -HALF_REGION;
    }
    if(x >= 0){
        x = 0;
    }
}

void drawPanelText(IMG_Anim *img){
    if(PanelButtonVisible) return;
    GRRLIB_DrawImg(42, 162, img->frames[0], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(85, 235, img->frames[1], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(85, 273, img->frames[2], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(85, 306, img->frames[3], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(85, 373, img->frames[4], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    GRRLIB_DrawImg(85, 404, img->frames[5], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
    u8 y_pad = 4;
    u8 x_pad = 10;
    if(IsCursorWithinImg(img->frames[1], 85, 235)) {panelSelect = 0;}
    if(IsCursorWithinImg(img->frames[2], 85, 273)) {panelSelect = 1;}
    if(IsCursorWithinImg(img->frames[3], 85, 306)) {panelSelect = 2;}
    if(IsCursorWithinImg(img->frames[4], 85, 373)) {panelSelect = 3;}
    if(IsCursorWithinImg(img->frames[5], 85, 404)) {panelSelect = 4;}
    switch (panelSelect)
    {
    case 0:
        GRRLIB_DrawImg(66-img->frames[6]->w + x_pad, 235 + y_pad, img->frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    case 1:
        GRRLIB_DrawImg(66-img->frames[6]->w + x_pad, 273 + y_pad, img->frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    case 2:
        GRRLIB_DrawImg(66-img->frames[6]->w + x_pad, 306 + y_pad, img->frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    case 3:
        GRRLIB_DrawImg(66-img->frames[6]->w + x_pad, 373 + y_pad, img->frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    case 4:
        GRRLIB_DrawImg(66-img->frames[6]->w + x_pad, 404 + y_pad, img->frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    default:
        GRRLIB_DrawImg(66-img->frames[6]->w + x_pad, 235 + y_pad, img->frames[6], 0, IMG_SCALE, IMG_SCALE, 0xFFFFFFFF);
        break;
    }
}

int gameSceneMain(){
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    GRRLIB_FillScreen(0x000000FF);
    GRRLIB_Render();
    u64 frame = 0;
    IMG_Anim office = {0};
    office.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/203(1).png"); // normal 1
    office.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/203(2).png"); // normal 2
    office.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/204(1).png"); // alarm 1
    office.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/204(2).png"); // alarm 2
    IMG_Anim fan = {0};
    fan.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/18.png");
    fan.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/19.png");
    fan.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/20.png");
    u8 fan_f = 0;
    IMG_Anim controlPanel = {0}; // the physical panel
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
    u8 controlPanel_f = 0;
    IMG_Anim PanelCommands = {0}; // the commands
    PanelCommands.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/599.png"); // "system restart menu"
    PanelCommands.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/544.png"); // "audio devices"
    PanelCommands.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/545.png"); // "camera system"
    PanelCommands.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/555.png"); // "ventilation"
    PanelCommands.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/594.png"); // "reboot all"
    PanelCommands.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/600.png"); // "exit"
    PanelCommands.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/596.png"); // green select cursor
    IMG_Anim NightTimeFont = {0};
    NightTimeFont.frames[0] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/746.png"); // 0
    NightTimeFont.frames[1] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/747.png"); // 1
    NightTimeFont.frames[2] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/748.png"); // 2
    NightTimeFont.frames[3] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/749.png"); // 3
    NightTimeFont.frames[4] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/750.png"); // 4
    NightTimeFont.frames[5] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/751.png"); // 5
    NightTimeFont.frames[6] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/752.png"); // 6
    NightTimeFont.frames[7] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/245.png"); // "night" text
    NightTimeFont.frames[8] = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/246.png"); // "am" text
    
    GRRLIB_texImg *cursor = GRRLIB_LoadTexture(pointer_png);
    GRRLIB_texImg *font = GRRLIB_LoadTexture(font_png);
    GRRLIB_texImg *panelClick = GRRLIB_LoadTextureFromFile("sd:/fnaf3/assets/images/77.png");
    GRRLIB_InitTileSet(cursor, 32, 32, 32);
    GRRLIB_InitTileSet(font, 32, 36, 32);
    char buffer[100];
    while (1)
    {
        WPAD_ScanPads();
        u32 mote = WPAD_ButtonsDown(0);
        WPAD_IR(0, &ir);
        IR_Y = ir.y * (480.0f / 460.0f);
        drawOffice(&office, &mote);
        drawFan(&fan, &fan_f, &frame);   
        drawPanelButton(panelClick);
        drawControlPanel(&controlPanel, &controlPanel_f, &frame);
        sprintf(buffer, "%lld", frame);
        GRRLIB_Printf(10, 10, font, 0xFFFFFFFF, .4, buffer);
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
        }
        drawPanelText(&PanelCommands);
        drawNightInf(&NightTimeFont);
        if(NIGHT == 1){
            if(frame%(60*40)==0){
                if(time_am == 12){
                    time_am = 1;
                }else{time_am += 1;}
            }
        }
        else{
            if(frame%(60*60)==0){
                if(time_am == 12){
                    time_am = 1;
                }else{time_am += 1;}
            }
        }
        if(time_am == 6){break;}
        drawCursor(cursor);
        GRRLIB_Render();
        frame++;
    }
    GRRLIB_FreeTexture(panelClick);
    FreeImgAnim(&office);
    FreeImgAnim(&fan);
    FreeImgAnim(&controlPanel);
    FreeImgAnim(&PanelCommands);
    FreeImgAnim(&NightTimeFont);
    return 0;
}