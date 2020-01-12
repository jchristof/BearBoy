#include "PauseScreen.h"

#include "..\res\src\pause.h"
#include "..\res\src\ascii.h"

#include "ZGBMain.h"
#include "Utils.h"
#include "Scroll.h"
#include "Keys.h"
#include "GameSound.h"
#include "Utils.h"
#include "BankManager.h"

#define NEXT_OPTION currentPauseOption++; if(currentPauseOption == PauseOptionsNum){ currentPauseOption = 0;}
#define PREV_OPTION if(currentPauseOption == 0){ currentPauseOption = PauseOptionsNum - 1;} else {currentPauseOption--;}

const UINT8 onPhrase[] = { 79, 78, 0 };
const UINT8 offPhrse[] = { 79, 70, 70};
const UINT8 pauseCursor[1] = {95};

void (*pauseOnQuit)(void);

typedef enum {
	Music,
	SoundFx,
	Exit,
	PauseOptionsNum
} PauseOption;

const UVector pauseCursorPosition[PauseOptionsNum] = {{3,3}, {3,4}, {3,5} };
PauseOption currentPauseOption = Music;

void InitPauseScreen(){
    InitWindow(0,0,&pause);
    InitScrollTiles(0, &ascii);

    wait_vbl_done();
    WY_REG = 0;
    SHOW_WIN;
    SHOW_BKG;
    HIDE_SPRITES;
    currentPauseOption = Music;

    if(IsMusicMuted())
        set_win_tiles(13, 3, 3, 1, offPhrse);
    else
        set_win_tiles(13, 3, 3, 1, onPhrase);

    if(IsSfxMuted())
        set_win_tiles(13, 4, 3, 1, offPhrse);
    else
        set_win_tiles(13, 4, 3, 1, onPhrase);

}

void PreviousOption(){
    UVector const *cursorPosition =  &pauseCursorPosition[currentPauseOption];
    set_win_tiles(cursorPosition->x, cursorPosition->y, 1, 1, blankTile);
    PREV_OPTION;
    cursorPosition =  &pauseCursorPosition[currentPauseOption];
    set_win_tiles(cursorPosition->x, cursorPosition->y, 1, 1, pauseCursor);
}

void NextOption(){
    UVector const *cursorPosition =  &pauseCursorPosition[currentPauseOption];
    set_win_tiles(cursorPosition->x, cursorPosition->y, 1, 1, blankTile);
    NEXT_OPTION;
    cursorPosition =  &pauseCursorPosition[currentPauseOption];
    set_win_tiles(cursorPosition->x, cursorPosition->y, 1, 1, pauseCursor);
}

void UpdatePauseScreen(){
    
    if(KEY_TICKED(J_UP)){
        PreviousOption();
    }
    else if(KEY_TICKED(J_DOWN)){
        NextOption();
    }
    else if(KEY_TICKED(J_A)){
        if(currentPauseOption == Music){
            if(IsMusicMuted()){
                UnMuteMusic();
                set_win_tiles(13, 3, 3, 1, onPhrase);
            }
            else{
                MuteMusic();
                set_win_tiles(13, 3, 3, 1, offPhrse);
            }
        }
        else if(currentPauseOption == SoundFx){
            if(IsSfxMuted()){
                UnMuteSfx();
                set_win_tiles(13, 4, 3, 1, onPhrase);
            }
            else{
                MuteSfx();
                set_win_tiles(13, 4, 3, 1, offPhrse);
            }
        }
        else if(currentPauseOption == Exit){
            if(pauseOnQuit){
                pauseOnQuit();
                pauseOnQuit = NULL;
            }
            else
                SetState(StateTitle);
        }
    }
}

void ExitPauseScreen(){
    HIDE_WIN;
    HIDE_BKG;
    SHOW_SPRITES;
    wait_vbl_done();
}