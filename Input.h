#ifndef SHADOW_SPRINT_INPUT_H
#define SHADOW_SPRINT_INPUT_H

void iKeyboard(unsigned char key);
void iSpecialKeyboard(unsigned char key);
void iMouseMove(int mx, int my);
void iPassiveMouseMove(int mx, int my);
void iMouse(int button, int state, int mx, int my);

#endif
