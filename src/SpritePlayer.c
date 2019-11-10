#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "Print.h"
#include "ZGBMain.h"

const UINT8 anim_idle[] = {1, 0}; //The first number indicates the number of frames
const UINT8 anim_walk[] = {2, 1, 2};
const UINT8 anim_slash[] = {5, 0,1,2,3,4};


void findEnemyPosition();

void State_Init();
void State_Idle();
void State_Attack_Pre();
void State_Attack();
void State_Attack_Post();
void Print();

struct Sprite* enemySprite = 0;
struct Sprite* hitEffectSprite = 0;

typedef enum {
    Init,
    Idle,
    Attack_Pre,
    Attack,
    Attack_Post,
    Attack_Num
}PlayerState;

PlayerState state = Init;

void (*fun_ptr_arr[Attack_Num])(void) = {State_Init, State_Idle, State_Attack_Pre, State_Attack, State_Attack_Post};

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

void State_Init(){
    Print();
    findEnemyPosition();
    state = Idle;
}

void State_Idle(){
    if(KEY_PRESSED(J_A))
        state = Attack_Pre;
}

void State_Attack_Pre(){
   TranslateSprite(THIS, 5, 0);
   if(CheckCollision(THIS, enemySprite)){
       TranslateSprite(THIS, 10, -32);
       state = Attack;
       
       hitEffectSprite->x = enemySprite->x;
       hitEffectSprite->y = enemySprite->y;
       SetSpriteAnim(hitEffectSprite, anim_slash, 15);
   }
}

void State_Attack(){
    if(hitEffectSprite->current_frame == 4){
        state = Attack_Post;
        THIS->x = 20;
        THIS->y = 64;
        hitEffectSprite->y = 160;
        hitEffectSprite->current_frame = 0;
        hitEffectSprite->anim_data = 0;
    }
        
    //DPRINT_POS(0, 0);
    //DPrintf("x:%d y:%d  ", enemySprite->x, enemySprite->y);
}

void State_Attack_Post(){
    state = Idle;
}

void Print(){
//ld d,d; jr @end; dw $6464; dw $0001; dw address; dw bank; @end
//__asm
//	ld d,d
//	jr @end
//	dw $6464
//	dw $0000
//	db "message"
//	@end
//__endasm;
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
