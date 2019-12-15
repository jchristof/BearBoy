#include "Banks/SetBank2.h"

#include <rand.h>
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "GameSound.h"

#include "..\res\src\tiles.h"
#include "..\res\src\title.h"

extern UINT8 *intro_mod_Data[];

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
}

void Update_StateTitle()
{
    if (ANY_KEY_PRESSED){
        SetState(StateGame);
        initrand(DIV_REG);
        CLEAR_KEYS();
    }
}