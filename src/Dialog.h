#ifndef DIALOG_H
#define DIALOG_H

#include "Sprite.h"

typedef struct{
    struct Sprite* portrait;
    const struct TilesInfoInternal* tileInfo;
    const char* text;
    UINT8 portraitSide;

} DialogSequence;

void Dialog_Init();
void Dialog_Start(DialogSequence *dialogSequence);
void Dialog_Update();

#endif