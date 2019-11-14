#include "Banks/SetBank2.h"

#include "..\res\src\tiles.h"
#include "..\res\src\map.h"
//#include "..\res\src\wintiles.h"
#include "..\res\src\winmap.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"

#include "Print.h"
#include "../res/src/font.h"

UINT8 collision_tiles[] = {1, 0};

void Start_StateGame() {
	UINT8 i;

	SPRITES_8x16;
	for(i = 0; i != N_SPRITE_TYPES; ++ i) {
		SpriteManagerLoad(i);
	}
	SHOW_SPRITES;

    SpriteManagerAdd(SpriteHitEffect, 16,160);
    //scroll_target = comment this back in to make the character a scroll target
	SpriteManagerAdd(SpritePlayer, 20, 64);
	SpriteManagerAdd(SpriteEnemy, 112, 64);
	SpriteManagerAdd(SpriteActionCursor, 64, 124);
	
	InitScroll(&map, collision_tiles, 0);
	InitWindow(0,0,&winmap);
	move_win(8, 120);
	SHOW_BKG;
	SHOW_WIN;
	InitScrollTiles(0, &tiles);
	INIT_CONSOLE(font, 3, 2);
}

void Update_StateGame() {
}