#include "Banks/SetBank2.h"

#include "..\res\src\tiles.h"
#include "..\res\src\map.h"
#include "..\res\src\font.h"
#include "..\res\src\jump.h"
#include "..\res\src\bear.h"
#include "..\res\src\lose.h"
#include "..\res\src\win.h"
#include "..\res\src\pressa.h"
#include "..\res\src\pressb.h"
#include "..\res\src\enemy.h"
#include "..\res\src\enemy_attack_mode.h"
#include "..\res\src\bear_attack_mode.h"
#include "..\res\src\bear_fail.h"
#include "..\res\src\enemy_attack.h"
#include "..\res\src\enemy_fail.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Print.h"
#include "PauseScreen.h"
#include "Utils.h"
#include "Sound.h"
#include "GameSound.h"
#include "Utils.h"
#include "Dialog.h"
#include "GamePlayCommon.h"
#include "VerticalBlankEffects.h"

struct Sprite *portraitSprite = 0;

typedef enum
{
    D0,
    D1,
    D2,
    D3,
    D4,
    D5,
    TutorialFinish,
    TutorialState_Num
} TutorialState;

const UINT8 tutorialHealthBarTiles[20] = {32, 31, 31, 31, 31, 31, 31, 31, 33, 0, 0, 32, 31, 31, 31, 31, 31, 31, 31, 33};

TutorialState currentTutorialState = D0;

DialogSequence dialogSequence;

const char d0[] =
    "Yo, you gotta trythis game!!!     Let me show you  how to play.▼";
//---------------//---------------//---------------//---------------//---------------//---------------
const char d1[] = "This is you.▼";
const char d2[] =
    "This is your     nemesis.▼";
//---------------//---------------//---------------//---------------//---------------//---------------
const char d3[] =
    "You'll take turnsattacking and    defending.▼";
//---------------//---------------//---------------//---------------//---------------//---------------

const char d4[] =
    "When an action   button appears,  press it quickly to attack or     defend.▼";
//---------------//---------------//---------------//---------------//---------------//---------------

const char d5[] =
    "These bars show  your health.     Beat your enemy  to progress to   the next round.▼";
//---------------//---------------//---------------//---------------//---------------//---------------

void D0_Init()
{
    dialogSequence.text = d0;
    Dialog_Start(&dialogSequence);
}

void D1_Init()
{
    dialogSequence.text = d1;
    Dialog_Start(&dialogSequence);
}

void D1_Exit()
{
    spritePlayer = SpriteManagerAdd(SpritePlayer, PLAYER_X, PLAYER_Y);
}

void D2_Init()
{
    dialogSequence.text = d2;
    Dialog_Start(&dialogSequence);
}

void D2_Exit()
{
    spriteEnemy = SpriteManagerAdd(SpriteEnemy, ENEMY_X, ENEMY_Y);
}

void D3_Init()
{
    dialogSequence.text = d3;
    Dialog_Start(&dialogSequence);
}

void D3_Exit() {}

void D4_Init()
{
    dialogSequence.text = d4;
    Dialog_Start(&dialogSequence);
    button = SpriteManagerAdd(SpriteButton, BUTTON_X, BUTTON_Y);
}

UINT8 tutorial_timer = 0;
void D4_Update()
{
    tutorial_timer = tutorial_timer > 32 ? 0 : tutorial_timer;
    if (tutorial_timer > 16)
	{
		SpriteManagerLoadTiles(button, pressa.data, 0);
	}
	else
	{
		SpriteManagerLoadTiles(button, pressb.data, 0);
	}

    ++tutorial_timer;
}

void D4_Exit()
{
    HIDE_SPRITE(button);
}

void D5_Init()
{
    dialogSequence.text = d5;
    Dialog_Start(&dialogSequence);

    set_bkg_tiles(0, 0, 20, 1, tutorialHealthBarTiles);
}

void D5_Exit()
{
}

void Exit_StateTutorial();

void Tutorial_Finish()
{
    Exit_StateTutorial();
    SetState(StateGame);
}

State tutorialStates[TutorialState_Num] = {
    {D0_Init, 0, 0},
    {D1_Init, 0, D1_Exit},
    {D2_Init, 0, D2_Exit},
    {D3_Init, 0, D3_Exit},
    {D4_Init, D4_Update, D4_Exit},
    {D5_Init, 0, D5_Exit},
    {Tutorial_Finish, 0, 0},
};

void Tutorial_SetState(TutorialState newState)
{
    if (tutorialStates[currentTutorialState].exit != 0)
        tutorialStates[currentTutorialState].exit();

    currentTutorialState = newState;
    if (tutorialStates[currentTutorialState].init != 0)
        tutorialStates[currentTutorialState].init();
}

void Start_StateTutorial()
{
    UINT8 i;
    UINT8 emptyTile[1] = {0};
    InGameEnableHbl();
    SHOW_SPRITES;

    for (i = 0; i != N_SPRITE_TYPES; ++i)
    {
        SpriteManagerLoad(i);
    }

    portraitSprite = SpriteManagerAdd(SpritePortrait, 1, 127);
    SetBkgTiles(&map);
    InitScrollTiles(0, &tiles);

    for (i = 0; i != 20; ++i)
    {
        set_bkg_tiles(i, 0, 1, 1, emptyTile);
    }

    currentTutorialState = D0;

    Dialog_Init();
    D0_Init();
}

void Update_StateTutorial()
{
    DialogState dialogState = Dialog_Update();

    if (KEY_TICKED(J_START))
    {
        Tutorial_SetState(TutorialFinish);
        return;
    }

    if (tutorialStates[currentTutorialState].update != 0)
        tutorialStates[currentTutorialState].update();

    if (dialogState == Dialog_State_Waiting)
    {
        if (KEY_TICKED(J_A))
        {
            Dialog_Continue();
        }
    }
    else if (dialogState == Dialog_State_Done)
    {
        if (KEY_TICKED(J_A))
        {
            if (currentTutorialState < TutorialState_Num - 1u)
                Tutorial_SetState(currentTutorialState + 1);
        }
    }
}

void Exit_StateTutorial()
{
    InGameDisableHbl();
}