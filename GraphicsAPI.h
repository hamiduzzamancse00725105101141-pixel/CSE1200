#ifndef SHADOW_SPRINT_GRAPHICS_API_H
#define SHADOW_SPRINT_GRAPHICS_API_H

#include "glut.h"

int isKeyPressed(unsigned char key);
int isSpecialKeyPressed(unsigned char key);

void iClear();
void iShowImage(int x, int y, int width, int height, unsigned int texture);
void iSetColor(double r, double g, double b);
void iFilledRectangle(double left, double bottom, double dx, double dy);
void iText(double x, double y, char *str, void *font = GLUT_BITMAP_8_BY_13);
void iFilledCircle(double x, double y, double r, int slices = 100);
unsigned int iLoadImage(char filename[]);
int iSetTimer(int msec, void (*f)(void));
void iPauseTimer(int index);
void iResumeTimer(int index);
void iInitialize(int width = 500, int height = 500, char *title = "iGraphics", int keyboardSamplingRate = 16);
void iStart();

#endif
