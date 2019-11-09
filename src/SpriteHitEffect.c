#include "Banks/SetBank2.h"
#include "SpriteManager.h"

const UINT8 anim_slash[] = {5, 0,1,2,3,4};

void Start_SpriteHitEffect() {
}

void Update_SpriteHitEffect() {
SetSpriteAnim(THIS, anim_slash, 1);
}

void Destroy_SpriteHitEffect() {
}