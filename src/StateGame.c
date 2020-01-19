#include "Banks/SetBank2.h"
#include <rand.h>
#include "..\res\src\tiles.h"
#include "..\res\src\map.h"
#include "..\res\src\font.h"
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
#include "PauseScreen.h"
#include "Utils.h"
#include "Sound.h"
#include "GameSound.h"
#include "GamePlayCommon.h"
#include "VerticalBlankEffects.h"

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
	State_Paused
};

struct Sprite *spriteHitEffect = 0;

const UINT8 anim_slash[] = {5, 0, 1, 2, 3, 4};

#define BLACK_OUT_BG \
	BGP_REG = (3 << 6) | (3 << 4) | (3 << 2) | 3;

#define FLASH_BG \
	BGP_REG = (1 << 6) | (1 << 4) | (1 << 2) | 1;

#define RESTORE_BG \
	BGP_REG = (3 << 6) | (2 << 4) | (1 << 2) | 0;

void UpdateBearHP();
void UpdateEnemyHP();
void DamageEnemy(UINT8 damage);
void ShowRound();

void QuitFromPause();

UINT8 playerMoveSpeed = 1;
UINT8 enemyMoveSpeed = 1;

UINT8 bearHP = 56;
UINT8 enemyHP = 56;

//nine bg tiles representing the healthbar
const UINT8 healthBarTiles[9] = {23, 24, 25, 26, 27, 28, 29, 30, 31};
const UINT8 trophyTile[1] = {34};
const UINT8 roundPhrase[] = {65, 66, 67, 68, 69};
const UINT8 numberTiles[] = {80, 81, 82, 83, 84, 85, 86, 87, 88, 89};
const UINT8 readyPhrase[] = {65, 70, 71, 69, 72, 79};
const UINT8 fightPhrase[] = {73, 74, 75, 76, 77, 78};

void State_Paused() {
	UpdatePauseScreen();
}

void Start_StateGame()
{
	UINT8 i;
	InGameEnableHbl();

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
	InitScrollTiles(0, &tiles);
	SHOW_BKG;

	for (i = 0; i < consecutiveWins; ++i)
	{
		set_bkg_tiles(i, 17, 1, 1, &(trophyTile[0]));
	}

	UpdateBearHP();
	UpdateEnemyHP();

	ShowRound();

	state = Init;
	lastState = Init;

	pauseOnQuit = QuitFromPause;
}

//show the two digit round count
void ShowRound(){
	
	UINT8 tens = (consecutiveWins + 1) / 10;
	UINT8 ones = (consecutiveWins + 1) % 10;
	UINT8 position = 12;

	ClearWindow();
	SHOW_WIN;
	WY_REG = 120;

	if(tens){
		set_win_tiles(position, 1, 1, 1, &numberTiles[tens]);
		position++;
	}

	set_win_tiles(position, 1, 1, 1, &numberTiles[ones]);

	set_win_tiles(6, 1, 5, 1, roundPhrase);
}

unsigned char lastPalette = 0;
void Update_StateGame()
{
	if (KEY_TICKED(J_START))
	{
		if (state != Paused)
		{
			HIDE_WIN;
    		HIDE_BKG;
			wait_vbl_done();
			InitPauseScreen();
			lastState = state;
			state = Paused;
			lastPalette = BGP_REG;
			RESTORE_BG;
		}
		else
		{
			ExitPauseScreen();
			SetBkgTiles(&map);
			InitScrollTiles(0, &tiles);
			state = lastState;
			SHOW_BKG;
			HIDE_WIN;
			SHOW_SPRITES;
			BGP_REG = lastPalette;
			UpdateBearHP();
			UpdateEnemyHP();
		}
	}
	else
		(*fun_ptr_arr[state])();
}

void Exit_GameState(){
	InGameDisableHbl();
}

void QuitFromPause(){
	lastGameWasWin = 0;
	consecutiveWins = 0;
	Exit_GameState();
}

#define DELAY_TIME 0x20

UINT8 time = DELAY_TIME;
void State_Init()
{
	if(spritePlayer->x < PLAYER_X)
		spritePlayer->x++;
	
	if(spriteEnemy->x > ENEMY_X)
		spriteEnemy->x--;

	if(ANY_KEY_PRESSED){
		ClearWindowLine(1);
		set_win_tiles(7, 1, 6, 1, readyPhrase);
		time = DELAY_TIME;
		state = Intro;
		RESET_PLAYER_POSITION(spritePlayer);
		RESET_ENEMY_POSITION(spriteEnemy);
	}
}

void State_Intro(){
	if(time-- == 0){
		ClearWindowLine(1);
		set_win_tiles(7, 1, 6, 1, fightPhrase);
		state = Fight;
		PlaySong(music_mod_Data, 3, 1);
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

		Exit_GameState();
		SetState(StateWinLose);
		return;
	}
	if (time--)
	{
		if (KEY_TICKED(J_A) || KEY_TICKED(J_B))
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

	SHOW_BUTTON(button)
	time = DELAY_TIME;
}

void State_Player_Defend()
{
	++enemyMoveSpeed;
	
	if (spriteEnemy->x > spritePlayer->x)
	{
		if (KEY_TICKED(J_B)){
			state = Player_Defend_Success_Init;
			time = DELAY_TIME;
			FLASH_BG;
		}
		else if (KEY_TICKED(J_A))
			state = Player_Defend_Fail;
		

		spriteEnemy->x -= (enemyMoveSpeed >> moveSpeedThrottle);
	}
	else
		state = Player_Defend_Fail;
}

void State_Player_Defend_Success_Init()
{
	while(time--)
		return;

	SpriteManagerLoadTiles(spriteEnemy, enemy_fail.data, 0);
	SpriteManagerLoadTiles(spritePlayer, jump.data, 0);
	BLACK_OUT_BG;
	SpriteManagerLoadTiles(button, win.data, 0);
	SHOW_BUTTON(button)
	DamageEnemy(8);

	time = DELAY_TIME;
	state = Player_Defend_Success;
}

void State_Player_Defend_Success()
{
	if (time-- == 0)
	{
		HIDE_SPRITE(button);
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

	SPRITE_SET_VMIRROR(spriteHitEffect);
	SetSpriteAnim(spriteHitEffect, anim_slash, 15);
	SpriteManagerLoadTiles(spritePlayer, bear_fail.data, 0);
	PlayFx(CHANNEL_1, 4, 0x4f, 0xc7, 0xf3, 0x73, 0x86);

	state = Attack_Failed;
}

void State_Player_Input_Attack()
{
	++playerMoveSpeed;
	spritePlayer->x += (playerMoveSpeed >> moveSpeedThrottle);

	if (KEY_TICKED(J_A))
	{
		FLASH_BG;
		SpriteManagerLoadTiles(button, win.data, 0);
		SHOW_BUTTON(button)
		state = Attack_Success;
		time = 0x10;
	}
	else if (spritePlayer->x >= 112 || KEY_TICKED(J_B)){
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

void DamageEnemy(UINT8 damage)
{
	if (damage < enemyHP)
		enemyHP -= damage;
	else
		enemyHP = 0;

	UpdateEnemyHP();
}

void State_Attack_Failed()
{
	DamagePlayer(8);
	
	SpriteManagerLoadTiles(button, lose.data, 0);
	SHOW_BUTTON(button)
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
	SPRITE_UNSET_VMIRROR(spriteHitEffect);
	SetSpriteAnim(spriteHitEffect, anim_slash, 15);
	SpriteManagerLoadTiles(spriteEnemy, enemy_fail.data, 0);
	PlayFx(CHANNEL_1, 4, 0x4f, 0xc7, 0xf3, 0x73, 0x86);
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

	HIDE_SPRITE(button)
	//SpriteManagerLoadTiles(button, pressa.data, 0);
	
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
