#include "ZGBMain.h"
#include "Math.h"

UINT8 moveSpeedThrottle = 4;
UINT8 consecutiveWins = 0;
UINT8 lastGameWasWin = 0;
UINT8 musicOn = 1;
UINT8 sfxOn = 1;

UINT8 next_state = StateTitle;

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if(current_state == StateGame) {
		if(U_LESS_THAN(255 - (UINT16)*tile_ptr, N_SPRITE_TYPES)) {
			*tile = 0;
			return 255 - (UINT16)*tile_ptr;
		}

		*tile = *tile_ptr;
	}

	return 255u;
}