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

const char dialog1[] = 
"Yo, you gotta try this game!!!    Let me tell you   how it works.    This is you.     This is your enemy.▼";
 //---------------//---------------//---------------//---------------//---------------//---------------

 void Dialog1_Init(){}
 void Dialog1_Update(){}
 void Dialog1_Exit(){}
 
void Tutorial_SetState(TutorialState newState){
    if(tutorialStates[currentState].exit != 0)
        tutorialStates[currentState].exit();

    currentState = newState;
    if(tutorialStates[currentState].init != 0)
        tutorialStates[currentState].init();
}

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

    dialogSequence.text = dia;
    Dialog_Start(&dialogSequence);
}

void Update_StateTutorial() {
    DialogState dialogState = Dialog_Update();

    if(tutorialStates[currentState].update != 0)
        tutorialStates[currentState].update();

    
    if(dialogState == Dialog_State_Waiting){
        if(KEY_TICKED(J_A)){
            Dialog_Continue();
        }
    }
    else if (dialogState == Dialog_State_Done){
    //     if(KEY_TICKED(J_A)){
    //         //next state
    //     }
     }

    
}

void Destroy_StateTutorial() {}