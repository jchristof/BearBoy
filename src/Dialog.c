#include "Dialog.h"
#include "Utils.h"
#include "BankManager.h"

#include "..\res\src\ascii.h"

const UINT8 dialogFrameLeft[] = {42, 45, 46, 43, 0, 47, 44, 49, 48};
const UINT8 textBoxArea[] = {96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,
                            113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129};

#define CLEAR_DIALOG(dialog) 
#define DIALOG_OPEN_POSITION 126
#define DIALOG_CLOSED_POSITION 144

#define TYPE_TIME 8
UINT8 timer = TYPE_TIME;
DialogSequence *sequence = 0;
UINT8 currentLine = 0;
UINT8 currentCharIndex = 0;
UINT8 currentTile = 96;
UINT8 dialogActive = 0;
const char* currentChar = 0;

void Dialog_Init()
{
    ClearWindow();
    SHOW_WIN;
    WY_REG = DIALOG_OPEN_POSITION;

    set_win_tiles(0, 0, 3, 3, dialogFrameLeft);
    PUSH_BANK(2);
    set_bkg_data(96, 32, ascii.data->data);
    POP_BANK;
    set_win_tiles(3, 0, 17, 2, textBoxArea);
}

void ClearTextArea(){
    UINT8 i = 0;
    for(i = 0; i < 34; ++i)
        set_bkg_data(96 + i, 1, &(ascii.data->data[0]));
}

void Dialog_Reset(){
    timer = TYPE_TIME;
    sequence = 0;
    currentLine = 0;
    currentCharIndex = 0;
    currentTile = 96;
    dialogActive = 0;
    currentChar = 0;
}

void Dialog_Start(DialogSequence *dialog)
{
    Dialog_Reset();
    ClearTextArea();
    sequence = dialog;
    dialogActive = 1;
    currentChar = sequence->text;
}

void Dialog_Continue(){
    ClearTextArea();
    currentTile = 96;
    timer = TYPE_TIME;
}

DialogState Dialog_Update()
{
    if(sequence == 0)
        return Dialog_State_Done;
        
    if((*currentChar) < ' '){
        sequence = 0;
        return Dialog_State_Done;
    }

    if(currentTile == 130)
        return Dialog_State_Waiting;
    
    if(timer--)
        return Dialog_State_Running;
    
    timer = TYPE_TIME;

    PUSH_BANK(2);
    set_bkg_data(currentTile, 1, &(ascii.data->data[((*currentChar)-32)*16]));
    POP_BANK;
    currentTile++;
    currentChar++;

    return Dialog_State_Running;
}