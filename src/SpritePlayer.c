#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "Print.h"
#include "ZGBMain.h"

const UINT8 anim_idle[] = {1, 0}; //The first number indicates the number of frames
const UINT8 anim_walk[] = {2, 1, 2};
const UINT8 anim_slash[] = {5, 0,1,2,3,4};
UINT8 state = 0;

void findEnemyPosition();
void State_Idle();
void State_Attack_Setup();
void State_Attack();

struct Sprite* enemySprite = 0;
struct Sprite* hitEffectSprite = 0;

void (*fun_ptr_arr[])(void) = {State_Idle, State_Attack_Setup, State_Attack};

void Start_SpritePlayer() {
//    THIS->coll_x = 2;
//	THIS->coll_y = 0;
//	THIS->coll_w = 12;
//	THIS->coll_h = 16;
}

void Update_SpritePlayer() {
    (*fun_ptr_arr[state])(); 
}

void Destroy_SpritePlayer() {
}

void findEnemyPosition(){
    UINT8 i;
	struct Sprite* spr = 0;

	SPRITEMANAGER_ITERATE(i, spr) {
		if(spr->type == SpriteEnemy) 
		    enemySprite =  spr;
		if(spr->type == SpriteHitEffect)
		    hitEffectSprite = spr;
	}
}

void State_Idle(){
    if(KEY_PRESSED(J_A))
        state = 1;
    
    findEnemyPosition();
}

void State_Attack_Setup(){
   TranslateSprite(THIS, 5, 0);
   if(CheckCollision(THIS, enemySprite)){
   TranslateSprite(THIS, 10, -32);
        state = 2;
        }
        
   
}

void State_Attack(){

    hitEffectSprite->x = enemySprite->x;
    hitEffectSprite->y = enemySprite->y;
    SetSpriteAnim(hitEffectSprite, anim_slash, 15);
   
    if(hitEffectSprite->current_frame == 4){
        state = 0;
        THIS->x = 20;
        THIS->y = 64;
        hitEffectSprite->y = 160;
        }
        
    DPRINT_POS(0, 0);
    DPrintf("x:%d y:%d  ", enemySprite->x, enemySprite->y);
    
}

//void x(){
//    if(KEY_PRESSED(J_UP)){
//        TranslateSprite(THIS, 0, -1);
//        SetSpriteAnim(THIS, anim_walk, 15);
//    }
//    else if(KEY_PRESSED(J_DOWN)){
//        TranslateSprite(THIS, 0, 1);
//        SetSpriteAnim(THIS, anim_walk, 15);
//    }
//    else if(KEY_PRESSED(J_LEFT)){
//        TranslateSprite(THIS, -1, 0);
//        SetSpriteAnim(THIS, anim_walk, 15);
//     }
//    else if(KEY_PRESSED(J_RIGHT)){
//        TranslateSprite(THIS, 1, 0);
//        SetSpriteAnim(THIS, anim_walk, 15);
//     }
//    if(keys == 0){
//        SetSpriteAnim(THIS, anim_idle, 15);
//    }    
//    
//    findEnemyPosition();
//    DPRINT_POS(0, 0);
//    DPrintf("x:%d y:%d  ", enemySprite->x, enemySprite->y);
//}
