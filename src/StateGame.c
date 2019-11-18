#include "Banks/SetBank2.h"

#include "BankManager.h"
#include "..\res\src\tiles.h"
#include "..\res\src\map.h"
#include "../res/src/font.h"
#include "..\res\src\jump.h"
#include "..\res\src\bear.h"
#include "..\res\src\lose.h"
#include "..\res\src\win.h"
#include "..\res\src\pressa.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Print.h"

#include "Sound.h"

extern UINT8* music_mod_Data[];

UINT8 collision_tiles[] = {1, 0};

void State_Init();
void State_Idle();
void State_Player_Input();
void State_Attack_Success();
void State_Attack_Failed();
void State_Attack_Failed_Idle();
void State_Attack_Pre();
void State_Attack();
void State_Attack_Post();
void State_Attack_Reset();

typedef enum
{
	Init,
	Idle,
	Player_Input,
	Attack_Success,
	Attack_Failed,
	Attack_Failed_Idle,
	Attack_Pre,
	Attack,
	Attack_Post,
	Attack_Reset,
	Attack_Num
} GameState;

GameState state = Init;

void (*fun_ptr_arr[Attack_Num])(void) = {
	State_Init, 
	State_Idle, 
	State_Player_Input, 
	State_Attack_Success, 
	State_Attack_Failed,
	State_Attack_Failed_Idle,
	State_Attack_Pre, 
	State_Attack, 
	State_Attack_Post,
	State_Attack_Reset
	};

struct Sprite *spriteHitEffect = 0;
struct Sprite *spritePlayer = 0;
struct Sprite *spriteEnemy = 0;
struct Sprite *button = 0;

const UINT8 anim_slash[] = {5, 0, 1, 2, 3, 4};

#define BLACK_OUT_BG \
	BGP_REG = (3 << 6) | (3 << 4) | (3 << 2) | 3

#define FLASH_BG \
	BGP_REG = (1 << 6) | (1 << 4) | (1 << 2) | 1

#define RESTORE_BG \
	BGP_REG = (3 << 6) | (2 << 4) | (1 << 2) | 0

#define OBJ1_ATTACK_COLOR \
	OBP0_REG = (2 << 6) | (2 << 4) | (3 << 2) | 3

#define OBJ1_RESTORE_COLOR \
	OBP0_REG = (3 << 6) | (2 << 4) | (1 << 2) | 0

#define HIDE_SPRITE(SPRITE) \
	SPRITE->x = 160; \
	SPRITE->y = 144;

#define SHOW_BUTTON(SPRITE) \
	SPRITE->x = 64; \
	SPRITE->y = 8;

void Start_StateGame()
{
	UINT8 i;

	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume

	SPRITES_8x16;

	for (i = 0; i != N_SPRITE_TYPES; ++i)
	{
		SpriteManagerLoad(i);
	}

	SHOW_SPRITES;

	spriteHitEffect = SpriteManagerAdd(SpriteHitEffect, 16, 160);
	SPRITE_SET_PALETTE(spriteHitEffect, 1);

	spritePlayer = SpriteManagerAdd(SpritePlayer, 20, 64);
	spriteEnemy = SpriteManagerAdd(SpriteEnemy, 112, 64);

	button = SpriteManagerAdd(SpriteButton, 64, 8);
	HIDE_SPRITE(button)

	SPRITE_SET_PALETTE(button, 1);

	InitScroll(&map, collision_tiles, 0);
	SHOW_BKG;

	InitScrollTiles(0, &tiles);
	//INIT_CONSOLE(font, 3, 2);

	PlayMusic(music_mod_Data, 3, 1);
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

	PlayFx(CHANNEL_4, 4, 0x0c, 0x41, 0x30, 0xc0);
	BLACK_OUT_BG;
	OBJ1_ATTACK_COLOR;
	
	SHOW_BUTTON(button)
	state = Player_Input;
	time = DELAY_TIME;
}

void State_Player_Input()
{
	if (KEY_PRESSED(J_A))
	{
		FLASH_BG;

		SpriteManagerLoadTiles(button, win.data, 0);
		state = Attack_Success;
		time = 0x10;
	}
	else
	{
		if (time--)
			return;

		state = Attack_Failed;
	}
}

void State_Attack_Success()
{
	if (time--)
		return;
	BLACK_OUT_BG;
	state = Attack_Pre;
	SpriteManagerLoadTiles(spritePlayer, jump.data, 0);
}

void State_Attack_Failed(){
	SpriteManagerLoadTiles(button, lose.data, 0);
	time = DELAY_TIME;
	state = Attack_Failed_Idle;
}

void State_Attack_Failed_Idle(){
	if(time-- == 0){
		state = Attack_Reset;
		spritePlayer->x = 20;
		return;
	}
	if(time %8== 0)
		spritePlayer->x = 160;
	else
		spritePlayer->x = 20;
}

void State_Attack_Pre()
{
	TranslateSprite(spritePlayer, 5, 0);

	if (spritePlayer->x < 160)
		return;

	state = Attack;

	spriteHitEffect->x = spriteEnemy->x;
	spriteHitEffect->y = spriteEnemy->y;
	SetSpriteAnim(spriteHitEffect, anim_slash, 15);
	PlayFx(CHANNEL_1, 10, 0x4f, 0xc7, 0xf3, 0x73, 0x86);
}

void State_Attack()
{
	if (spriteHitEffect->current_frame == 4)
	{
		state = Attack_Post;
		time = DELAY_TIME;
		spriteHitEffect->y = 160;
		spriteHitEffect->current_frame = 0;
		spriteHitEffect->anim_data = 0;
	}

	//DPRINT_POS(0, 0);
	//DPrintf("x:%d y:%d  ", enemySprite->x, enemySprite->y);
}

void State_Attack_Post()
{
	if(time-- == 0){
		state = Attack_Reset;
		spriteEnemy->x = 112;
		return;
	}
	if(time %8== 0)
		spriteEnemy->x = 160;
	else
		spriteEnemy->x = 112;
}

void State_Attack_Reset()
{
	state = Idle;
	RESTORE_BG;
	OBJ1_RESTORE_COLOR;

	SpriteManagerLoadTiles(button, pressa.data, 0);
	HIDE_SPRITE(button)
	SpriteManagerLoadTiles(spritePlayer, bear.data, 0);
	spritePlayer->x = 20;
	spritePlayer->y = 64;
}