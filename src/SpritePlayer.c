#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "Print.h"
#include "ZGBMain.h"

const UINT8 anim_idle[] = {1, 0}; //The first number indicates the number of frames
const UINT8 anim_walk[] = {2, 1, 2};

void findEnemyPosition();
void State_Idle();
void State_Attack();

struct Sprite* enemySprite = 0;
void (*fun_ptr_arr[])(void) = {State_Idle, State_Idle, State_Idle};

void Start_SpritePlayer() {
    THIS->coll_x = 2;
	THIS->coll_y = 0;
	THIS->coll_w = 12;
	THIS->coll_h = 16;
}

void Update_SpritePlayer() {
    (*fun_ptr_arr[0])(); 
}

void Destroy_SpritePlayer() {
}

void findEnemyPosition(){
    UINT8 i;
	struct Sprite* spr = 0;

	SPRITEMANAGER_ITERATE(i, spr) {
		if(spr->type == SpriteEnemy) {
		    enemySprite =  spr;
		    return;
		}
	}
}

void State_Idle(){
    if(KEY_PRESSED(J_UP)){
        TranslateSprite(THIS, 0, -1);
        SetSpriteAnim(THIS, anim_walk, 15);
    }
    else if(KEY_PRESSED(J_DOWN)){
        TranslateSprite(THIS, 0, 1);
        SetSpriteAnim(THIS, anim_walk, 15);
    }
    else if(KEY_PRESSED(J_LEFT)){
        TranslateSprite(THIS, -1, 0);
        SetSpriteAnim(THIS, anim_walk, 15);
     }
    else if(KEY_PRESSED(J_RIGHT)){
        TranslateSprite(THIS, 1, 0);
        SetSpriteAnim(THIS, anim_walk, 15);
     }
    if(keys == 0){
        SetSpriteAnim(THIS, anim_idle, 15);
    }    
    
    findEnemyPosition();
    DPRINT_POS(0, 0);
    DPrintf("x:%d y:%d  ", enemySprite->x, enemySprite->y);
}

void State_Attack(){
}
