#ifndef DIALOG_H
#define DIALOG_H

#include "Sprite.h"

typedef struct{
    struct Sprite* portrait;
    const struct TilesInfoInternal* tileInfo;
    const char* text;
    UINT8 portraitSide;

} DialogSequence;

typedef enum {
    Dialog_State_Done,
    Dialog_State_Waiting,
    Dialog_State_Running,
    
} DialogState;

void Dialog_Init();
void Dialog_Start(DialogSequence *dialogSequence);
DialogState Dialog_Update();
void Dialog_Continue();

#endif