#include "Banks/SetBank2.h"

#include "Keys.h"
#include "SpriteManager.h"
#include "Print.h"
#include "ZGBMain.h"

INT8 move = 1;

void Start_SpriteActionCursor() {
}

void Update_SpriteActionCursor() {
    if(THIS->x > 136)
        move = -1;
    else if(THIS->x < 16)
        move = 1;
        
    THIS->x += move;
}

void Destroy_SpriteActionCursor() {
}