#include "Banks/SetBank2.h"

#include "..\res\src\tiles.h"
#include "..\res\src\map.h"
#include "..\res\src\winmap.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Print.h"
#include "../res/src/font.h"

UINT8 collision_tiles[] = {1, 0};

void State_Init();
void State_Idle();
void State_Player_Input();
void State_Attack_Success();
void State_Attack_Pre();
void State_Attack();
void State_Attack_Post();

typedef enum
{
	Init,
	Idle,
	Player_Input,
	Attack_Success,
	Attack_Pre,
	Attack,
	Attack_Post,
	Attack_Num
} PlayerState;

PlayerState state = Init;

#define BLACK_OUT_BG \
	BGP_REG = (3 << 6) | (3 << 4) | (3 << 2) | 3

#define FLASH_BG \
	BGP_REG = (2 << 6) | (2 << 4) | (2 << 2) | 2

#define RESTORE_BG \
	BGP_REG = (3 << 6) | (2 << 4) | (1 << 2) | 0

#define OBJ1_ATTACK_COLOR \
	OBP0_REG = (2 << 6) | (2 << 4) | (3 << 2) | 3

#define OBJ1_RESTORE_COLOR \
	OBP0_REG = (3 << 6) | (2 << 4) | (1 << 2) | 0

void (*fun_ptr_arr[Attack_Num])(void) = {State_Init, State_Idle, State_Player_Input, State_Attack_Success, State_Attack_Pre, State_Attack, State_Attack_Post};

struct Sprite *spriteHitEffect = 0;
struct Sprite *spritePlayer = 0;
struct Sprite *spriteEnemy = 0;
struct Sprite *button = 0;

const UINT8 anim_slash[] = {5, 0, 1, 2, 3, 4};

void Start_StateGame()
{
	UINT8 i;

	SPRITES_8x16;
	for (i = 0; i != N_SPRITE_TYPES; ++i)
	{
		SpriteManagerLoad(i);
	}
	SHOW_SPRITES;

	spriteHitEffect = SpriteManagerAdd(SpriteHitEffect, 16, 160);
	//scroll_target = comment this back in to make the character a scroll target
	spritePlayer = SpriteManagerAdd(SpritePlayer, 20, 64);
	spriteEnemy = SpriteManagerAdd(SpriteEnemy, 112, 64);
	//SpriteManagerAdd(SpriteActionCursor, 64, 124);

	InitScroll(&map, collision_tiles, 0);
	InitWindow(0, 0, &winmap);
	//move_win(8, 120);
	SHOW_BKG;
	//SHOW_WIN;
	InitScrollTiles(0, &tiles);
	INIT_CONSOLE(font, 3, 2);
}

void Update_StateGame()
{
	(*fun_ptr_arr[state])();
}

#define DELAY_TIME 0x20
UINT8 time = DELAY_TIME;

void State_Init()
{
	state = Idle;
	time = DELAY_TIME;
}

void State_Idle()
{
	if (time--)
		return;
	//if(KEY_PRESSED(J_A)){
	//state = Attack_Pre;
	BLACK_OUT_BG;
	OBJ1_ATTACK_COLOR;
	button = SpriteManagerAdd(SpriteButton, 64, 8);
	SPRITE_SET_PALETTE(button, 1);
	state = Player_Input;
	time = DELAY_TIME;
	// }
}

void State_Player_Input()
{
	if (KEY_PRESSED(J_A))
	{
		FLASH_BG;
		SpriteManagerRemoveSprite(button);
		button = SpriteManagerAdd(SpriteWin, 64, 8);
		SPRITE_SET_PALETTE(button, 1);
		state = Attack_Success;
		time = 0x10;
	}
	else
	{
		if (time--)
			return;
		//state fail
	}
}

void State_Attack_Success()
{
	if (time--)
		return;
	BLACK_OUT_BG;
	state = Attack_Pre;
	SpriteManagerRemoveSprite(spritePlayer);
	spritePlayer = SpriteManagerAdd(SpritePlayerAttack, 20, 64);
}

void State_Attack_Pre()
{
	
	TranslateSprite(spritePlayer, 5, 0);
	//if(CheckCollision(spritePlayer, spriteEnemy)){
	//TranslateSprite(spritePlayer, 10, -32);

	if (spritePlayer->x < 160)
		return;

	state = Attack;

	spriteHitEffect->x = spriteEnemy->x;
	spriteHitEffect->y = spriteEnemy->y;
	SetSpriteAnim(spriteHitEffect, anim_slash, 15);
	//}
}

void State_Attack()
{
	if (spriteHitEffect->current_frame == 4)
	{
		state = Attack_Post;
		spritePlayer->x = 20;
		spritePlayer->y = 64;
		spriteHitEffect->y = 160;
		spriteHitEffect->current_frame = 0;
		spriteHitEffect->anim_data = 0;
	}

	//DPRINT_POS(0, 0);
	//DPrintf("x:%d y:%d  ", enemySprite->x, enemySprite->y);
}

void State_Attack_Post()
{
	state = Idle;
	RESTORE_BG;
	OBJ1_RESTORE_COLOR;
	SpriteManagerRemoveSprite(button);
	SpriteManagerRemoveSprite(spritePlayer);
	spritePlayer = SpriteManagerAdd(SpritePlayer, 20, 64);
}