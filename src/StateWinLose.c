#include "ZGBMain.h"
#include "Banks/SetBank2.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"

#include "..\res\src\ascii.h"
#include "..\res\src\winlose.h"
#include "..\res\src\youdied.h"

struct Sprite *winLoseGraphic = 0;

#define YOU_WIN_MSG_LEN 7
#define YOU_DIED_MSG_LEN 8

const char youWinMessage[] = "You Win";
const char youDiedMessage[] = "You Died";
char charToDraw = ' ';
void Start_StateWinLose()
{
    UINT8 i;
    InitScroll(&winlose, 0, 0);
    InitScrollTiles(0, &ascii);

    winLoseGraphic = SpriteManagerAdd(SpriteWinLose, 60, 60);
    if(!lastGameWasWin){
        SpriteManagerLoadTiles(winLoseGraphic, youdied.data, 0);

        for(i = 0; i < YOU_DIED_MSG_LEN; ++i){
            charToDraw = youDiedMessage[i] - 32;
            set_bkg_tiles(6 + i, 2, 1, 1, &charToDraw);
        }
    }
    else{
        for(i = 0; i < YOU_WIN_MSG_LEN; ++i){
            charToDraw = youWinMessage[i] - 32;
            set_bkg_tiles(6 + i, 2, 1, 1, &charToDraw);
        }
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
}