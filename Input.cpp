#pragma warning(disable:4996)
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "glut.h"
#include "GameGlobals.h"
#include "Score.h"
#include "Audio.h"
#include "GameLogic.h"
#include "Input.h"

void iKeyboard(unsigned char key)
{
	if (!gameStarted)
	{
		if (key == 13 && !levelSelect && !highScorePage)
		{
			levelSelect = true;
			return;
		}
		if (key == 27) exit(0);
		return;
	}

	if (key == 'p' || key == 'P')
	{
		paused = !paused;
		return;
	}

	if (key == 'r' || key == 'R')
	{
		resetGameObjects();
		gameStarted = true;
		mciSendString("stop ggsong", NULL, 0, NULL);
		if (mediumLevel) playGameSound("play mediumsong repeat");
		else playGameSound("play easySong repeat");
		return;
	}
}

void iSpecialKeyboard(unsigned char key)
{

}

void iMouseMove(int mx, int my)
{
	mouseX = mx;
	mouseY = SCREEN_HEIGHT - my;
}

void iPassiveMouseMove(int mx, int my)
{
	mouseX = mx;
	mouseY = SCREEN_HEIGHT - my;
}

void iMouse(int button, int state, int mx, int my)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;


	int x = mx;
	int y = my;



	// MAIN MENU / LEVEL SELECT

	if (!gameStarted)
	{
		// SETTINGS PAGE
		if (settingsPage)
		{
			// SOUND ON/OFF
			if (x >= 300 && x <= 500 && y >= 175 && y <= 225)
			{
				soundEnabled = !soundEnabled;

				if (!soundEnabled)
				{
					stopAllSounds();
				}
				else
				{
					playGameSound("play menusong repeat");
				}
				return;
			}

			// BACK
			if (x >= 300 && x <= 500 && y >= 95 && y <= 145)
			{
				settingsPage = false;
				return;
			}

			return;
		}

		// CREDITS PAGE
		if (creditsPage)
		{
			if (x >= 320 && x <= 480 && y >= 90 && y <= 135)
			{
				creditsPage = false;
				return;
			}
			return;
		}

		if (highScorePage)
		{
			vector<int> scores = loadScores();
			int totalPages = scores.empty() ? 1 : ((int)scores.size() + SCORES_PER_PAGE - 1) / SCORES_PER_PAGE;

			if (x >= 130 && x <= 230 && y >= 35 && y <= 80)
			{
				highScorePage = false;
				return;
			}
			if (x >= 330 && x <= 430 && y >= 35 && y <= 80)
			{
				if (highScorePageNumber > 0) highScorePageNumber--;
				return;
			}
			if (x >= 500 && x <= 600 && y >= 35 && y <= 80)
			{
				if (highScorePageNumber + 1 < totalPages) highScorePageNumber++;
				return;
			}
			return;
		}

		if (!levelSelect)
		{
			// START GAME
			// start.png -> START GAME
			if (x >= 300 && x <= 500 &&
				y >= 180 && y <= 220)
			{
				levelSelect = true;
				return;
			}

			// HIGH SCORE
			if (x >= 300 && x <= 500 && y >= 140 && y <= 180)
			{
				highScorePage = true;
				highScorePageNumber = 0;
				return;
			}

			// GAME SETTINGS
			// Keep this button separate from CREDITS.
			if (x >= 300 && x <= 500 &&
				y >= 118 && y <= 148)
			{
				settingsPage = true;
				return;
			}

			// CREDITS
			// Separate range so a Credits click cannot open Settings.
			if (x >= 300 && x <= 500 &&
				y >= 92 && y <= 120)
			{
				creditsPage = true;
				return;
			}

			// QUIT
			// Separate range so a Quit click cannot open Credits.
			if (x >= 300 && x <= 500 &&
				y >= 66 && y <= 94)
			{
				exit(0);
			}

			return;
		}

		// LEVEL SELECT PAGE

		// EASY
		if (x >= 280 && x <= 520 &&
			y >= 175 && y <= 220)
		{
			mediumLevel = false;
			mciSendString("stop menusong", NULL, 0, NULL);
			playGameSound("play easySong repeat");
			resetGameObjects();
			gameStarted = true;
			levelSelect = false;
			return;
		}

		// MEDIUM
		if (x >= 280 && x <= 520 &&
			y >= 125 && y <= 170)
		{
			mediumLevel = true;
			mciSendString("stop menusong", NULL, 0, NULL);
			playGameSound("play mediumsong repeat");
			resetGameObjects();
			gameStarted = true;
			levelSelect = false;
			return;
		}

		// BACK
		if (x >= 280 && x <= 520 &&
			y >= 70 && y <= 115)
		{
			levelSelect = false;
			return;
		}

		return;
	}


	// PAUSE SCREEN


	if (paused)
	{
		// RESUME BUTTON
		if (x >= 295 && x <= 505 &&
			y >= 205 && y <= 265)
		{
			paused = false;

			return;
		}

		// MAIN MENU IMAGE BUTTON
		// Must match the exact position of the image drawn above.
		if (x >= 325 && x <= 475 &&
			y >= 125 && y <= 185)
		{
			// Stop the current level music.
			mciSendString("stop easySong", NULL, 0, NULL);
			mciSendString("stop mediumsong", NULL, 0, NULL);
			mciSendString("stop ggsong", NULL, 0, NULL);

			// Return to the main menu instead of closing the game.
			gameStarted = false;
			paused = false;
			gameOver = false;
			gameComplete = false;
			levelSelect = false;
			settingsPage = false;
			creditsPage = false;
			highScorePage = false;
			highScorePageNumber = 0;

			// Start menu music again if sound is ON.
			playGameSound("play menusong repeat");
			return;
		}

		return;
	}



	
	if (gameOver)
	{
		
		if (!mediumLevel && !gameComplete)
		{
			
			if (x >= 220 && x <= 400 && y >= 50 && y <= 120)
			{
				resetGameObjects();
				gameStarted = true;
				mciSendString("stop ggsong", NULL, 0, NULL);
				playGameSound("play easySong repeat");
				return;
			}

			// QUIT:
			if (x >= 410 && x <= 592 && y >= 50 && y <= 120)
			{
				mciSendString("stop easySong", NULL, 0, NULL);
				mciSendString("stop mediumsong", NULL, 0, NULL);
				mciSendString("stop ggsong", NULL, 0, NULL);
				exit(0);
			}
			return;
		}

		
		if (!mediumLevel && gameComplete)
		{
			// RETRY
			if (x >= 226 && x <= 405 && y >= 50 && y <= 120)
			{
				resetGameObjects();
				gameStarted = true;
				mciSendString("stop ggsong", NULL, 0, NULL);
				playGameSound("play easySong repeat");
				return;
			}

			// RETURN TO CAMP
			if (x >= 417 && x <= 596 && y >= 50 && y <= 120)
			{
				mciSendString("stop easySong", NULL, 0, NULL);
				mciSendString("stop mediumsong", NULL, 0, NULL);
				mciSendString("stop ggsong", NULL, 0, NULL);
				gameStarted = false;
				paused = false;
				gameOver = false;
				gameComplete = false;
				levelSelect = false;
				settingsPage = false;
				creditsPage = false;
				highScorePage = false;
				highScorePageNumber = 0;
				playGameSound("play menusong repeat");
				return;
			}
			return;
		}

		
		if (mediumLevel && !gameComplete)
		{
			// HOME
			if (x >= 296 && x <= 388 && y >= 0 && y <= 72)
			{
				mciSendString("stop easySong", NULL, 0, NULL);
				mciSendString("stop mediumsong", NULL, 0, NULL);
				mciSendString("stop ggsong", NULL, 0, NULL);
				gameStarted = false;
				paused = false;
				gameOver = false;
				gameComplete = false;
				levelSelect = false;
				settingsPage = false;
				creditsPage = false;
				highScorePage = false;
				highScorePageNumber = 0;
				playGameSound("play menusong repeat");
				return;
			}

			// RETRY:
			if (x >= 416 && x <= 506 && y >= 0 && y <= 72)
			{
				resetGameObjects();
				gameStarted = true;
				mciSendString("stop ggsong", NULL, 0, NULL);
				playGameSound("play mediumsong repeat");
				return;
			}
			return;
		}

		
		if (mediumLevel && gameComplete)
		{
			// HOME:
			if (x >= 296 && x <= 388 && y >= 0 && y <= 72)
			{
				mciSendString("stop easySong", NULL, 0, NULL);
				mciSendString("stop mediumsong", NULL, 0, NULL);
				mciSendString("stop ggsong", NULL, 0, NULL);
				gameStarted = false;
				paused = false;
				gameOver = false;
				gameComplete = false;
				levelSelect = false;
				settingsPage = false;
				creditsPage = false;
				highScorePage = false;
				highScorePageNumber = 0;
				playGameSound("play menusong repeat");
				return;
			}

			// RETRY LEVEL:
			if (x >= 416 && x <= 507 && y >= 0 && y <= 72)
			{
				resetGameObjects();
				gameStarted = true;
				mciSendString("stop ggsong", NULL, 0, NULL);
				playGameSound("play mediumsong repeat");
				return;
			}
			return;
		}
	}

	//  PAUSE button top on screen

	if (x >= 700 && x <= 780 &&
		y >= 340 && y <= 380)
	{
		paused = true;

		return;
	}






	// click  korle bullet fire

	float targetX = (float)mx;
	float targetY = (float)(SCREEN_HEIGHT - my);

	for (int i = 0; i < MAX_PLAYER_BULLET; i++)
	{
		if (ammo <= 0){
			return;
		}
		int shots = 1;

		if (doubleBulletActive)
			shots = 2;
		if (!pBullet[i].active)
		{
			pBullet[i].active = true;

			pBullet[i].x = charX + playerW;
			pBullet[i].y = charY + playerH / 2.0f;

			pBullet[i].w = 45;
			pBullet[i].h = 10;

			if (targetY < pBullet[i].y)
				targetY = pBullet[i].y;

			float dx = targetX - pBullet[i].x;
			float dy = targetY - pBullet[i].y;

			float length = sqrt(dx * dx + dy * dy);

			if (length > 0.001f)
			{
				pBullet[i].dx = dx / length;
				pBullet[i].dy = dy / length;
			}
			else
			{
				pBullet[i].dx = 1.0f;
				pBullet[i].dy = 0.0f;
			}
			ammo--;
			break;
		}
	}
}
