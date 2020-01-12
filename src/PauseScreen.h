#ifndef PAUSE_SCREEN_H
#define PAUSE_SCREEN_H

extern void (*pauseOnQuit)(void);
void InitPauseScreen();
void UpdatePauseScreen();
void ExitPauseScreen();
#endif