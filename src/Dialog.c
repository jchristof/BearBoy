#include "Dialog.h"
#include "Utils.h"

const UINT8 dialogFrameLeft[] = {42, 45, 46, 43, 0, 47, 44, 49, 48};

#define DIALOG_LINE_LENGTH 20
UINT8 dialogLine1[DIALOG_LINE_LENGTH] = {0};
UINT8 dialogLine2[DIALOG_LINE_LENGTH] = {0};

#define CLEAR_DIALOG(dialog) 
#define DIALOG_OPEN_POSITION 126
#define DIALOG_CLOSED_POSITION 144

void Dialog_Init()
{
    ClearWindow();
    SHOW_WIN;
    WY_REG = DIALOG_CLOSED_POSITION;

    set_win_tiles(0, 0, 3, 3, dialogFrameLeft);
}


void Dialog_RenderText(char* text){
    set_win_tiles(3, 0, 3, 3, dialogFrameLeft);
}

void Dialog_Start(DialogSequence *dialogSequence)
{

}

void Dialog_Update()
{
}