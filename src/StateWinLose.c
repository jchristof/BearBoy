#include "ZGBMain.h"
#include "Banks/SetBank2.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"

#include "..\res\src\font.h"
#include "..\res\src\winlose.h"

void Start_StateWinLose()
{
    InitScroll(&winlose, 0, 0);
    InitScrollTiles(0, &font);
    CLEAR_KEYS();
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