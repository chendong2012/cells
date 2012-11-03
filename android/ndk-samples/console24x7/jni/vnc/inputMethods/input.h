#ifndef KEYMANIP_H
#define KEYMANIP_H

#include "common.h"
#include "gui.h"

#include "rfb/rfb.h"
#include "suinput.h"

#define BUS_VIRTUAL 0x06

void initInput();
int keysym2scancode(rfbBool down, rfbKeySym c, rfbClientPtr cl, int *sh, int *alt);
void transformTouchCoordinates(int *x, int *y,int,int);
void ptrEvent(int buttonMask, int x, int y, rfbClientPtr cl);
void keyEvent(rfbBool down, rfbKeySym key, rfbClientPtr cl);
void cleanupInput();

#endif
