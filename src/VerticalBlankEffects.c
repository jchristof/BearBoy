#include <gb/gb.h>

#include "main.h"

#define FRAME_SKIP 32

UINT8 bgOffset;

const UINT8 grassTable[16][4] = {
	{0,0,0,0}, {1,0,0,0}, {1,1,0,0}, {1,1,1,0},
	{1,1,1,1}, {2,1,1,1}, {2,2,1,1}, {2,2,2,1},
	{2,2,2,2}, {3,2,2,2}, {3,3,2,2}, {3,3,3,2},
	{3,3,3,3}, {4,3,3,3}, {4,4,3,3}, {4,4,4,3}
};

UINT8 val0;
UINT8 val1;
UINT8 val2;
UINT8 val3;

UINT8 grassBlowDir;
UINT8 grassBlowAmt;
UINT8 frameSkip;
UINT8 interruptsEnabled;

void InGameHbl(){

	switch (LYC_REG)
	{
	case  0x07:
		SCX_REG = bgOffset;
		LYC_REG = 0x40;
		break;
	
	case 0x40:
		SCX_REG = 0;
		LYC_REG = 81;
		break;

	case 81:
		SCX_REG = val0;
		LYC_REG = 82;
		break;

	case 82:
		SCX_REG = val1;
		LYC_REG = 83;
		break;

	case 83:
		SCX_REG = val2;
		LYC_REG = 84;
		break;

	case 84:
		SCX_REG = val3;
		LYC_REG = 85;
		break;
	
	case 85:
		SCX_REG = 0;
		LYC_REG = 0x07;
		break;
	}
}

void InGameHblUpdate(){
    if(frameSkip-- == 0){
		bgOffset++;
		if(grassBlowDir == 0){
			grassBlowAmt++;
			if(grassBlowAmt == 15)
				grassBlowDir = 1;
		}
		else{
			grassBlowAmt--;
			if(grassBlowAmt == 0)
				grassBlowDir = 0;
		}
		val0 = grassTable[grassBlowAmt][0];
		val1 = grassTable[grassBlowAmt][1];
		val2 = grassTable[grassBlowAmt][2];
		val3 = grassTable[grassBlowAmt][3];

		frameSkip = FRAME_SKIP;
	}
}

void InGameEnableHbl(){
    LYC_REG = 0x07;
    bgOffset = 0;
    grassBlowDir = 0;
    grassBlowAmt = 0;
    val0 = val1 = val2 = val3 = 0;
    frameSkip = FRAME_SKIP;

    interruptsEnabled = 1;
	disable_interrupts();
	add_LCD(InGameHbl);
    //add_VBL(InGameHblUpdate);
    extra_vbl = InGameHblUpdate;
    set_interrupts(VBL_IFLAG | TIM_IFLAG | LCD_IFLAG);
	enable_interrupts();
}

void InGameDisableHbl(){
    //UINT8 wait = 255;
    if(interruptsEnabled == 0)
        return;
    
    interruptsEnabled = 0;
    set_interrupts(VBL_IFLAG | TIM_IFLAG);
	disable_interrupts();
    //while(wait--)
    remove_LCD(InGameHbl);
    extra_vbl = 0;
    //remove_VBL(InGameHblUpdate);
    set_interrupts(VBL_IFLAG | TIM_IFLAG);
	enable_interrupts();

}