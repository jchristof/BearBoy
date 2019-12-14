#include "Banks/SetBank2.h"
#include <rand.h>

#include "..\res\src\tiles.h"
#include "..\res\src\map.h"
#include "..\res\src\font.h"
#include "..\res\src\pause.h"
#include "..\res\src\jump.h"
#include "..\res\src\bear.h"
#include "..\res\src\lose.h"
#include "..\res\src\win.h"
#include "..\res\src\pressa.h"
#include "..\res\src\pressb.h"
#include "..\res\src\enemy.h"
#include "..\res\src\enemy_attack_mode.h"
#include "..\res\src\bear_attack_mode.h"
#include "..\res\src\bear_fail.h"
#include "..\res\src\enemy_attack.h"
#include "..\res\src\enemy_fail.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Print.h"

#include "Sound.h"

extern UINT8 *music_mod_Data[];

void State_Init();
void State_Intro();
void State_Fight();
void State_Idle();
void State_Player_Input_Attack();
void State_Player_Defend();
void State_Player_Defend_Success_Init();
void State_Player_Defend_Success();
void State_Player_Defend_Fail();
void State_Attack_Success();
void State_Attack_Failed();
void State_Attack_Failed_Idle();
void State_Attack_Pre();
void State_Attack();
void State_Attack_Post();
void State_Attack_Reset();
void State_Paused();

typedef enum
{
	Init,
	Intro,
	Fight,
	Idle,
	Player_Input_Attack,
	Player_Defend,
	Player_Defend_Success_Init,
	Player_Defend_Success,
	Player_Defend_Fail,
	Attack_Success,
	Attack_Failed,
	Attack_Failed_Idle,
	Attack_Pre,
	Attack,
	Attack_Post,
	Attack_Reset,
	Paused,
	State_Num
} GameState;

GameState state = Init;
GameState lastState = Init;

void (*fun_ptr_arr[State_Num])(void) = {
	State_Init,
	State_Intro,
	State_Fight,
	State_Idle,
	State_Player_Input_Attack,
	State_Player_Defend,
	State_Player_Defend_Success_Init,
	State_Player_Defend_Success,
	State_Player_Defend_Fail,
	State_Attack_Success,
	State_Attack_Failed,
	State_Attack_Failed_Idle,
	State_Attack_Pre,
	State_Attack,
	State_Attack_Post,
	State_Attack_Reset,
	State_Paused};

struct Sprite *spriteHitEffect = 0;
struct Sprite *spritePlayer = 0;
struct Sprite *spriteEnemy = 0;
struct Sprite *button = 0;

const UINT8 anim_slash[] = {5, 0, 1, 2, 3, 4};

#define BLACK_OUT_BG \
	BGP_REG = (3 << 6) | (3 << 4) | (3 << 2) | 3;

#define FLASH_BG \
	BGP_REG = (1 << 6) | (1 << 4) | (1 << 2) | 1;

#define RESTORE_BG \
	BGP_REG = (3 << 6) | (2 << 4) | (1 << 2) | 0;

// #define OBJ1_ATTACK_COLOR \
// 	OBP0_REG = (1 << 6) | (1 << 4) | (2 << 2) | 2

// #define OBJ1_RESTORE_COLOR \
// 	OBP0_REG = (3 << 6) | (2 << 4) | (1 << 2) | 0

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

void UpdateBearHP();
void UpdateEnemyHP();
void clearWin();

UINT8 playerMoveSpeed = 1;
UINT8 enemyMoveSpeed = 1;

UINT8 bearHP = 56;
UINT8 enemyHP = 56;

//nine bg tiles representing the healthbar
UINT8 healthBarTiles[9] = {23, 24, 25, 26, 27, 28, 29, 30, 31};
UINT8 trophyTile[1] = {34};
UINT8 blankTile[1] = {0};
const UINT8 roundPhrase[] = {0x52, 0x4f, 0x55, 0x4e, 0x44};
UINT8 numberTiles[] = {0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64};
UINT8 readyPhrase[] = {0x52, 0x45, 0x41, 0x44, 0x59, 0x6c};
UINT8 fightPhrase[] = {0x46, 0x49, 0x47, 0x48, 0x54, 0x65};

void State_Paused() {}
void Start_StateGame()
{
	UINT8 i;

	for (i = 0; i != N_SPRITE_TYPES; ++i)
	{
		SpriteManagerLoad(i);
	}

	playerMoveSpeed = 1;
	enemyMoveSpeed = 1;

	bearHP = 56;
	enemyHP = 56;

	SHOW_SPRITES;

	spriteHitEffect = SpriteManagerAdd(SpriteHitEffect, 16, 160);
	SPRITE_SET_PALETTE(spriteHitEffect, 1);

	spritePlayer = SpriteManagerAdd(SpritePlayer, PLAYER_X, PLAYER_Y);
	spritePlayer->x = 0;

	spriteEnemy = SpriteManagerAdd(SpriteEnemy, ENEMY_X, ENEMY_Y);
	spriteEnemy->x = ENEMY_X + 16;

	button = SpriteManagerAdd(SpriteButton, BUTTON_X, BUTTON_Y);
	HIDE_SPRITE(button);

	SetBkgTiles(&map);
	SHOW_BKG;

	InitScrollTiles(0, &tiles);

	for (i = 0; i < consecutiveWins; ++i)
	{
		set_bkg_tiles(i, 17, 1, 1, &(trophyTile[0]));
	}

	PlayMusic(music_mod_Data, 3, 1);

	UpdateBearHP();
	UpdateEnemyHP();

	clearWin();
	SHOW_WIN;
	WY_REG = 120;
	set_win_tiles(6, 1, 5, 1, roundPhrase);
	set_win_tiles(12, 1, 1, 1, &numberTiles[consecutiveWins + 1]);

	state = Init;
	lastState = Init;
}

void clearWin()
{
	UINT8 x = 0;
	UINT8 y = 0;
	for (y = 0; y < 18; ++y)
	{
		for (x = 0; x < 20; ++x)
		{
			set_win_tiles(x, y, 1, 1, blankTile);
		}
	}
}

void clearWinLine(UINT8 line){
	UINT8 x = 0;
	for (x = 0; x < 20; ++x)
	{
		set_win_tiles(x, line, 1, 1, blankTile);
	}
}

unsigned char lastPalette = 0;
void Update_StateGame()
{
	unsigned char map[1] = {1};
	if (KEY_RELEASED(J_START))
	{
		if (state != Paused)
		{
			lastState = state;
			state = Paused;

			clearWin();
			InitWindow(0,0,&pause);
			WY_REG = 0;
			SHOW_WIN;
			HIDE_SPRITES;
			lastPalette = BGP_REG;
			RESTORE_BG;
		}
		else
		{
			state = lastState;
			SHOW_BKG;
			HIDE_WIN;
			SHOW_SPRITES;
			BGP_REG = lastPalette;
		}
	}
	else
		(*fun_ptr_arr[state])();
}

#define DELAY_TIME 0x20

UINT8 time = DELAY_TIME;

UINT8 count = 0;
UINT8 blinkFreq = 0;
void State_Init()
{
	if(spritePlayer->x < PLAYER_X)
		spritePlayer->x++;
	
	if(spriteEnemy->x > ENEMY_X)
		spriteEnemy->x--;

	if(ANY_KEY_PRESSED){
		clearWinLine(1);
		set_win_tiles(7, 1, 6, 1, readyPhrase);
		time = DELAY_TIME;
		state = Intro;
		RESET_PLAYER_POSITION(spritePlayer);
		RESET_ENEMY_POSITION(spriteEnemy);
	}
}

void State_Intro(){
	if(time-- == 0){
		clearWinLine(1);
		set_win_tiles(7, 1, 6, 1, fightPhrase);
		state = Fight;
		time = DELAY_TIME;
	}
}

void State_Fight(){
	if(time-- == 0){
		HIDE_WIN;
		state = Idle;
	}
}

void State_Idle()
{
	if (bearHP == 0 || enemyHP == 0)
	{
		if (enemyHP == 0)
			lastGameWasWin = 1;

		SetState(StateWinLose);
		return;
	}
	if (time--)
	{
		if (KEY_PRESSED(J_A) || KEY_PRESSED(J_B))
		{
			time = DELAY_TIME;
			state = Attack_Failed;
			PlayFx(CHANNEL_1, 4, 0x4f, 0x96, 0xB7, 0xBB, 0x85);
		}
		return;
	}

	PlayFx(CHANNEL_4, 4, 0x0c, 0x41, 0x30, 0xc0);
	BLACK_OUT_BG;
	SpriteManagerLoadTiles(spriteEnemy, enemy_attack_mode.data, 0);
	SpriteManagerLoadTiles(spritePlayer, bear_attack_mode.data, 0);
	SHOW_BUTTON(button)
	enemyMoveSpeed = 1;
	playerMoveSpeed = 1;

	if (rand() % 2)
	{
		SpriteManagerLoadTiles(button, pressa.data, 0);
		SpriteManagerLoadTiles(spritePlayer, jump.data, 0);
		state = Player_Input_Attack;
	}
	else
	{
		SpriteManagerLoadTiles(button, pressb.data, 0);
		SpriteManagerLoadTiles(spriteEnemy, enemy_attack.data, 0);
		state = Player_Defend;
	}

	time = DELAY_TIME;
}

void State_Player_Defend()
{
	++enemyMoveSpeed;
	if (spriteEnemy->x > spritePlayer->x)
	{
		if (KEY_PRESSED(J_B))
			state = Player_Defend_Success_Init;

		spriteEnemy->x -= (enemyMoveSpeed >> moveSpeedThrottle);
	}
	else
		state = Player_Defend_Fail;
}

void State_Player_Defend_Success_Init()
{
	SpriteManagerLoadTiles(spriteEnemy, enemy_fail.data, 0);
	SpriteManagerLoadTiles(spritePlayer, jump.data, 0);
	HIDE_SPRITE(button);
	enemyHP--;
	UpdateEnemyHP();
	time = DELAY_TIME;
	state = Player_Defend_Success;
}

void State_Player_Defend_Success()
{
	if (time-- == 0)
	{
		state = Attack_Reset;
	}
	if (time % 8 == 0)
	{
		HIDE_SPRITE(spriteEnemy)
	}
	else
	{
		RESET_ENEMY_POSITION(spriteEnemy)
	}
}

void State_Player_Defend_Fail()
{
	if (spriteEnemy->x > enemyMoveSpeed)
	{
		TranslateSprite(spriteEnemy, -enemyMoveSpeed, 0);
		return;
	}

	HIDE_SPRITE(spriteEnemy);
	spriteHitEffect->x = spritePlayer->x;
	spriteHitEffect->y = spritePlayer->y;
	SetSpriteAnim(spriteHitEffect, anim_slash, 15);
	SpriteManagerLoadTiles(spritePlayer, bear_fail.data, 0);
	PlayFx(CHANNEL_1, 4, 0x4f, 0xc7, 0xf3, 0x73, 0x86);

	state = Attack_Failed;
}

void State_Player_Input_Attack()
{
	++playerMoveSpeed;
	spritePlayer->x += (playerMoveSpeed >> moveSpeedThrottle);

	if (KEY_PRESSED(J_A))
	{
		FLASH_BG;
		SHOW_BUTTON(button)
		SpriteManagerLoadTiles(button, win.data, 0);
		state = Attack_Success;
		time = 0x10;
	}
	else
	{
		if (spritePlayer->x < 112)
			return;

		SpriteManagerLoadTiles(spritePlayer, bear_fail.data, 0);
		SpriteManagerLoadTiles(spriteEnemy, enemy_attack.data, 0);
		PlayFx(CHANNEL_1, 4, 0x4f, 0xc7, 0xf3, 0x73, 0x86);
		state = Attack_Failed;
	}
}

void State_Attack_Success()
{
	if (time--)
		return;
	BLACK_OUT_BG;
	state = Attack_Pre;
}

void DamagePlayer(UINT8 damage)
{
	if (damage < bearHP)
		bearHP -= damage;
	else
		bearHP = 0;

	UpdateBearHP();
}

void State_Attack_Failed()
{
	DamagePlayer(8);
	SHOW_BUTTON(button)
	SpriteManagerLoadTiles(button, lose.data, 0);
	SpriteManagerLoadTiles(spritePlayer, bear_fail.data, 0);
	time = DELAY_TIME;
	state = Attack_Failed_Idle;
}

void State_Attack_Failed_Idle()
{
	if (time-- == 0)
	{
		state = Attack_Reset;
		spritePlayer->x = 20;
		return;
	}
	if (time % 8 == 0)
	{
		HIDE_SPRITE(spritePlayer);
	}
	else
	{
		RESET_PLAYER_POSITION(spritePlayer);
	}
}

void State_Attack_Pre()
{
	TranslateSprite(spritePlayer, playerMoveSpeed, 0);
	if (spritePlayer->x + playerMoveSpeed < 160)
	{
		spritePlayer->x += playerMoveSpeed;
		return;
	}
	else
	{
		spritePlayer->x = 160;
	}

	state = Attack;

	spriteHitEffect->x = spriteEnemy->x;
	spriteHitEffect->y = spriteEnemy->y;
	SetSpriteAnim(spriteHitEffect, anim_slash, 15);
	SpriteManagerLoadTiles(spriteEnemy, enemy_fail.data, 0);
	PlayFx(CHANNEL_1, 4, 0x4f, 0xc7, 0xf3, 0x73, 0x86);
}

void DamageEnemy(UINT8 damage)
{
	if (damage < enemyHP)
		enemyHP -= damage;
	else
		enemyHP = 0;

	UpdateEnemyHP();
}

void State_Attack()
{
	if (spriteHitEffect->current_frame == 4)
	{
		state = Attack_Post;
		time = DELAY_TIME;
		DamageEnemy(8);
	}
}

void State_Attack_Post()
{
	if (time-- == 0)
	{
		state = Attack_Reset;
		spriteEnemy->x = 112;
		return;
	}
	if (time % 8 == 0)
	{
		HIDE_SPRITE(spriteEnemy)
	}
	else
	{
		RESET_ENEMY_POSITION(spriteEnemy)
	}
}

void State_Attack_Reset()
{
	state = Idle;
	RESTORE_BG;
	//OBJ1_RESTORE_COLOR;

	SpriteManagerLoadTiles(button, pressa.data, 0);
	HIDE_SPRITE(button)
	SpriteManagerLoadTiles(spritePlayer, bear.data, 0);
	SpriteManagerLoadTiles(spriteEnemy, enemy.data, 0);

	RESET_PLAYER_POSITION(spritePlayer)

	RESET_ENEMY_POSITION(spriteEnemy)
}

void UpdateBearHP()
{
	UINT8 wholeTiles = bearHP / 8;
	UINT8 partialTileValue = bearHP % 8;
	UINT8 index = 0;

	for (index = 0; index < wholeTiles; index++)
	{
		set_bkg_tiles(1 + index, 0, 1, 1, &healthBarTiles[8]);
	}

	for (; index < 7; index++)
	{
		set_bkg_tiles(1 + index, 0, 1, 1, &healthBarTiles[partialTileValue]);

		break;
	}

	for (; index < 7; index++)
	{
		set_bkg_tiles(1 + index, 0, 1, 1, &healthBarTiles[0]);
		break;
	}
}

void UpdateEnemyHP()
{
	UINT8 wholeTiles = enemyHP / 8;
	UINT8 partialTileValue = enemyHP % 8;
	UINT8 index = 0;

	for (index = 0; index < wholeTiles; index++)
	{
		set_bkg_tiles(12 + index, 0, 1, 1, &healthBarTiles[8]);
	}

	for (; index < 7;)
	{
		set_bkg_tiles(12 + index, 0, 1, 1, &healthBarTiles[partialTileValue]);
		index++;
		break;
	}

	for (; index < 7; index++)
	{
		set_bkg_tiles(12 + index, 0, 1, 1, &healthBarTiles[0]);
		break;
	}
}
