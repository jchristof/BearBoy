#ifndef GAMEPLAY_COMMON_H
#define GAMEPLAY_COMMON_H

#define HIDE_SPRITE(SPRITE) \
	SPRITE->x = 160;        \
	SPRITE->y = 144;

#define BUTTON_X 64
#define BUTTON_Y 8

#define SHOW_BUTTON(SPRITE) \
	SPRITE->x = BUTTON_X;   \
	SPRITE->y = BUTTON_Y;

#define PLAYER_X 8
#define PLAYER_Y 64

#define ENEMY_X 112
#define ENEMY_Y 64

#define RESET_PLAYER_POSITION(SPRITE) \
	SPRITE->x = PLAYER_X;             \
	SPRITE->y = PLAYER_Y;

#define RESET_ENEMY_POSITION(SPRITE) \
	SPRITE->x = ENEMY_X;             \
	SPRITE->y = ENEMY_Y;

struct Sprite *spritePlayer = 0;
struct Sprite *spriteEnemy = 0;
struct Sprite *button = 0;

#endif