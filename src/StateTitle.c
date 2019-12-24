#include "Banks/SetBank2.h"

#include <rand.h>
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "GameSound.h"
#include "Utils.h"
#include "PauseScreen.h"

#include "..\res\src\tiles.h"
#include "..\res\src\title.h"

#define NEXT_OPTION currentOption++; if(currentOption == Num){ currentOption = 0;}
#define PREV_OPTION if(currentOption == 0){ currentOption = Num - 1;} else {currentOption--;}

extern UINT8 *intro_mod_Data[];

typedef enum{
    Menu,
    Pause,
    TitleState_Num
} TitleState;

TitleState titleState = Menu;

void MenuUpdate();
void PauseInit();
void PauseUpdate();
void PauseExit();

State titleStates[TitleState_Num] = {
    {0, MenuUpdate, 0},
    {PauseInit, PauseUpdate, PauseExit}
};

typedef enum {
	Play,
    Options,
	Num
} Option;

Option currentOption = Play;

const UVector const cursorPosition[Num] = {{4,15}, {4,16} };
const UVector const *cp = 0;

void Start_StateTitle()
{
    NR52_REG = 0x80; //Enables sound, you should always setup this first
    NR51_REG = 0xFF; //Enables all channels (left and right)
    NR50_REG = 0x77; //Max volume

    SPRITES_8x16;
    HIDE_WIN;
    
    SpriteManagerLoad(0);
    SHOW_SPRITES;
    SpriteManagerAdd(SpritePlayer, 64, 64);

    InitScroll(&title, 0, 0);
    SHOW_BKG;
    InitScrollTiles(0, &tiles);
    PlaySong(intro_mod_Data, 3, 1);
    currentOption = Play;

    cp = &(cursorPosition[currentOption]);
    set_bkg_tiles(cp->x, cp->y, 1, 1, cursorTile);
    titleState = Menu;
}

void TitlePreviousOption(){
    cp = &(cursorPosition[currentOption]);
    set_bkg_tiles(cp->x, cp->y, 1, 1, blankTile);
    PREV_OPTION;
    cp =  &cursorPosition[currentOption];
    set_bkg_tiles(cp->x, cp->y, 1, 1, cursorTile);
}

void TitleNextOption(){
    cp =  &cursorPosition[currentOption];
    set_bkg_tiles(cp->x, cp->y, 1, 1, blankTile);
    NEXT_OPTION;
    cp =  &cursorPosition[currentOption];
    set_bkg_tiles(cp->x, cp->y, 1, 1, cursorTile);
}

void Title_SetState(TitleState newState){
    if(titleStates[titleState].exit != 0)
        titleStates[titleState].exit();

    titleState = newState;
    if(titleStates[titleState].init != 0)
        titleStates[titleState].init();
}

void Update_StateTitle()
{
    titleStates[titleState].update();
}

void MenuUpdate(){
    if(KEY_RELEASED(J_UP)){
        TitlePreviousOption();
    }
    else if(KEY_RELEASED(J_DOWN)){
        TitleNextOption();
    }
    else if (KEY_RELEASED(J_A)){
        if(currentOption == Play){
            SetState(StateTutorial);
            initrand(DIV_REG);
            CLEAR_KEYS();
        }
        else{
            Title_SetState(Pause);
        }
    }
}

void PauseInit(){
    InitPauseScreen();
}
void PauseUpdate(){
    UpdatePauseScreen();
    if(KEY_RELEASED(J_B)){
        Title_SetState(Menu);
        CLEAR_KEYS();
    }
}
void PauseExit(){
    ExitPauseScreen();
}