#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateTitle)\
_STATE(StateGame)\
STATE_DEF_END

#define SPRITES \
_SPRITE_DMG(SpritePlayer, bear)\
_SPRITE_DMG(SpriteEnemy, enemy)\
_SPRITE_DMG(SpriteHitEffect, slash)\
_SPRITE_DMG(SpriteButton, pressa)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif