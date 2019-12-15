#include "Utils.h"

const UINT8 blankTile[1] = {0};

void ClearWindow()
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

void ClearWindowLine(UINT8 line){
	UINT8 x = 0;
	for (x = 0; x < 20; ++x)
	{
		set_win_tiles(x, line, 1, 1, blankTile);
	}
}