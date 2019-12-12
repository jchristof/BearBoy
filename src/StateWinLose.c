#include "ZGBMain.h"
#include "Banks/SetBank2.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"

#include "..\res\src\font.h"
#include "..\res\src\winlose.h"
#include "..\res\src\youdied.h"

struct Sprite *winLoseGraphic = 0;

UINT8 youWinMessage[] = {25, 15, 21, 0, 23, 9, 14, 37};
UINT8 youDiedMessage[] = {25, 15, 21, 0, 4, 9, 5, 4};
void Start_StateWinLose()
{

    InitScroll(&winlose, 0, 0);
    InitScrollTiles(0, &font);
    CLEAR_KEYS();

    winLoseGraphic = SpriteManagerAdd(SpriteWinLose, 60, 60);
    if(!lastGameWasWin){
        SpriteManagerLoadTiles(winLoseGraphic, youdied.data, 0);
        set_bkg_tiles(6, 2, 8, 1, youDiedMessage);
    }
    else{
        set_bkg_tiles(6, 2, 8, 1, youWinMessage);
    }

    SHOW_SPRITES;
}

void Update_StateWinLose()
{
    if (ANY_KEY_PRESSED)
    {
        if (lastGameWasWin)
        {
            lastGameWasWin = 0;
            consecutiveWins++;
            moveSpeedThrottle = moveSpeedThrottle > 0 ? moveSpeedThrottle - 1 : 0;
            SetState(StateGame);
        }
        else
        {
            moveSpeedThrottle = 4;
            consecutiveWins = 0;
            SetState(StateTitle);
        }
    }

    CLEAR_KEYS();
}