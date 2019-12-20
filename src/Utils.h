#ifndef UTILS_H
#define UTILS_H

#include "ZGBMain.h"
typedef struct {
	UINT8 x;
	UINT8 y;
} UVector;

typedef struct {
	void (*init)(void);
	void (*update)(void);
	void (*exit)(void);
} State;

extern const UINT8 cursorTile[];
extern const UINT8 blankTile[];

void ClearWindow();
void ClearWindowLine(UINT8 line);

#endif