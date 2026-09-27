#pragma warning(disable:4996)
#include "GameGlobals.h"
#include "GraphicsAPI.h"
#include "Renderer.h"
#include "GameLogic.h"
#include "Input.h"
#include "Init.h"

int main()
{
    iInitialize(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        "Shadow Sprint");

    initGame();
    iSetTimer(200, introAnimation);
    iSetTimer(20, autoMove);
    iSetTimer(20, fixedUpdate);

    iStart();

    return 0;
}
