#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateTitle)\
_STATE(StateGame)\
_STATE(StateWinLose)\
_STATE(StateTutorial)\
STATE_DEF_END

#define SPRITES \
_SPRITE_DMG(SpritePlayer, bear)\
_SPRITE_DMG(SpriteEnemy, enemy)\
_SPRITE_DMG(SpriteHitEffect, slash)\
_SPRITE_DMG(SpriteButton, pressa)\
_SPRITE_DMG(SpriteWinLose, youwin)\
_SPRITE_DMG(SpritePortrait, bearportrait)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

extern UINT8 moveSpeedThrottle; 
extern UINT8 consecutiveWins;
extern UINT8 lastGameWasWin;
extern UINT8 musicOn;
extern UINT8 sfxOn;

#endif