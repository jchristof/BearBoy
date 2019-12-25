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

typedef enum{
    Start,
    Pause,
    TutorialState_Num
} TutorialState;

TutorialState currentState = Start;

State tutorialStates[TutorialState_Num] = {
    {0, 0, 0},
    {0, 0, 0}
};

DialogSequence dialogSequence;

struct Sprite *portraitSprite = 0;

const char dialog1[] = {'L','i','s','t','e','n',' ','u','p','.','.','.','\0'};

void Start_StateTutorial() {
    UINT8 i;
    SHOW_SPRITES;

	for (i = 0; i != N_SPRITE_TYPES; ++i)
	{
		SpriteManagerLoad(i);
	}

    portraitSprite = SpriteManagerAdd(SpritePortrait, 1, 127);
    SetBkgTiles(&map);
    InitScrollTiles(0, &tiles);

    currentState = Start;

    Dialog_Init();
    
}

void Update_StateTutorial() {
    if(tutorialStates[currentState].update != 0)
        tutorialStates[currentState].update();

    if(KEY_TICKED(J_A)){
        dialogSequence.text[0] = dialog1;
        Dialog_Start(&dialogSequence);
    }

    Dialog_Update();
}

void Destroy_StateTutorial() {}