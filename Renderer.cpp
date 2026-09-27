#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#pragma warning(disable:4996)
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
#include "GameGlobals.h"
#include "Collision.h"
#include "Score.h"
#include "Audio.h"
#include "GameLogic.h"
#include "GraphicsAPI.h"
#include "Renderer.h"

void drawAimIndicator()
{
	if (gameOver || paused || playerDead) return;
	float startX = charX + playerW / 2.0f;
	float startY = charY + playerH / 2.0f;
	float dx = (float)mouseX - startX;
	float dy = (float)mouseY - startY;
	float length = sqrt(dx * dx + dy * dy);
	if (length < 1.0f) return;
	dx /= length; dy /= length;
	iSetColor(180, 220, 255);
	for (float d = 15.0f; d <= 150.0f; d += 12.0f)
	{
		float dotX = startX + dx * d;
		float dotY = startY + dy * d;
		if (dotX < 0 || dotX > SCREEN_WIDTH || dotY < 0 || dotY > SCREEN_HEIGHT) break;
		iFilledCircle((int)dotX, (int)dotY, 2);
	}
	float markX = startX + dx * 32.0f;
	float markY = startY + dy * 32.0f;
	iFilledCircle((int)markX, (int)markY, 4);
}

void introAnimation()
{
	if (introFinished)
		return;

	introFrame++;

	if (introFrame >= INTRO_FRAMES)
	{
		introFrame = INTRO_FRAMES - 1;
		introFinished = true;

		// Intro music
		mciSendString("stop introsong", NULL, 0, NULL);

		// Main menu music 
		playGameSound("play menusong repeat");
	}
}

void drawResultNumber(int x, int y, int value)
{
	char text[32];
	sprintf_s(text, sizeof(text), "%d", value);

	
	iSetColor(45, 25, 10);
	iText(x + 2, y - 2, text, GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(255, 220, 70);
	iText(x, y, text, GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawResultScreen()
{
	int resultImg = 0;
	if (mediumLevel)
		resultImg = gameComplete ? mediumLevelCompleteImg : mediumGameOverImg;
	else
		resultImg = gameComplete ? easyLevelCompleteImg : easyGameOverImg;

	
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, resultImg);

	
	if (!mediumLevel)
	{
		
		drawResultNumber(535, 250, score);
		drawResultNumber(535, 211, coinCount);
	}
	else if (!gameComplete)
	{
		
		drawResultNumber(560, 244, score);
		drawResultNumber(550, 205, coinCount);
	}
	else
	{
		drawResultNumber(420, 244, score);
		drawResultNumber(555, 214, coinCount);
	}
}

void iDraw()
{
	iClear();
	if (!introFinished)
	{
		iShowImage(
			0,
			0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			introImg[introFrame]
			);
		iSetColor(255, 255, 255);

		iText(
			300,
			200,
			"Shadow Sprint",
			GLUT_BITMAP_TIMES_ROMAN_24
			);


		return;
	}
	if (!gameStarted)
	{
		// GAME SETTINGS PAGE
		if (settingsPage)
		{
			iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, startImg);

			iSetColor(5, 20, 60);
			iFilledRectangle(150, 55, 500, 290);

			iSetColor(120, 220, 255);
			iText(315, 305, "GAME SETTINGS", GLUT_BITMAP_TIMES_ROMAN_24);

			iSetColor(255, 255, 255);
			iText(300, 245, "GAME SOUND", GLUT_BITMAP_HELVETICA_18);

			if (soundEnabled)
			{
				iSetColor(30, 180, 80);
				iFilledRectangle(300, 175, 200, 50);
				iSetColor(255, 255, 255);
				iText(370, 193, "ON", GLUT_BITMAP_HELVETICA_18);
			}
			else
			{
				iSetColor(190, 40, 40);
				iFilledRectangle(300, 175, 200, 50);
				iSetColor(255, 255, 255);
				iText(365, 193, "OFF", GLUT_BITMAP_HELVETICA_18);
			}

			iSetColor(100, 190, 255);
			iFilledRectangle(300, 95, 200, 50);
			iSetColor(255, 255, 255);
			iText(365, 113, "BACK", GLUT_BITMAP_HELVETICA_18);

			return;
		}

		// CREDITS PAGE
		if (creditsPage)
		{
			iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, startImg);

			iSetColor(5, 20, 60);
			iFilledRectangle(180, 55, 440, 290);

			iSetColor(120, 220, 255);
			iText(345, 305, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);

			iSetColor(255, 255, 255);
			iText(330, 245, "MD HAMIDUZZAMAN", GLUT_BITMAP_HELVETICA_18);
			iText(330, 200, "RAJKUMAR GOASH", GLUT_BITMAP_HELVETICA_18);
			iText(330, 155, "NOUSIN FERDUSI DINA", GLUT_BITMAP_HELVETICA_18);

			iSetColor(100, 190, 255);
			iFilledRectangle(320, 90, 160, 45);
			iSetColor(255, 255, 255);
			iText(365, 106, "BACK", GLUT_BITMAP_HELVETICA_18);

			return;
		}

		if (highScorePage)
		{
			iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, startImg);
			iSetColor(5, 20, 60); iFilledRectangle(120, 35, 560, 330);
			iSetColor(120, 220, 255); iText(305, 325, "HIGH SCORES", GLUT_BITMAP_TIMES_ROMAN_24);
			vector<int> scores = loadScores();
			int highest = getHighestScore();
			iSetColor(255, 220, 80);
			char highText[80]; sprintf_s(highText, sizeof(highText), "HIGHEST SCORE : %d", highest);
			iText(275, 290, highText, GLUT_BITMAP_HELVETICA_18);
			int totalPages = scores.empty() ? 1 : ((int)scores.size() + SCORES_PER_PAGE - 1) / SCORES_PER_PAGE;
			if (highScorePageNumber >= totalPages) highScorePageNumber = totalPages - 1;
			int startIndex = highScorePageNumber * SCORES_PER_PAGE;
			int endIndex = min(startIndex + SCORES_PER_PAGE, (int)scores.size());
			iSetColor(255, 255, 255);
			if (scores.empty()) iText(315, 220, "NO SCORES YET", GLUT_BITMAP_HELVETICA_18);
			else
			{
				int row = 245;
				for (int i = startIndex; i < endIndex; i++)
				{
					char line[80]; sprintf_s(line, sizeof(line), "GAME %d    SCORE : %d", i + 1, scores[i]);
					iText(285, row, line, GLUT_BITMAP_HELVETICA_12); row -= 20;
				}
			}
			iSetColor(100, 190, 255);
			iText(145, 55, "BACK", GLUT_BITMAP_HELVETICA_18);
			iText(340, 55, "< PREV", GLUT_BITMAP_HELVETICA_12);
			iText(510, 55, "NEXT >", GLUT_BITMAP_HELVETICA_12);
			//char pageText[50]; sprintf_s(pageText, sizeof(pageText), "PAGE %d / %d", highScorePageNumber + 1, totalPages);
			//iSetColor(255, 255, 255); iText(350, 80, pageText, GLUT_BITMAP_HELVETICA_12);
			return;
		}
		if (levelSelect) iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, levelSelectImg);
		else iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, startImg);
		return;
	}
	if (paused)
	{
		if (mediumLevel)
		{
			iShowImage(
				bgX,
				0,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				mediumBg1
				);

			iShowImage(
				bgX + SCREEN_WIDTH,
				0,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				mediumBg2
				);
		}
		else
		{
			iShowImage(
				bgX,
				0,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				bg1
				);

			iShowImage(
				bgX + SCREEN_WIDTH,
				0,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				bg2
				);
		}

		iSetColor(255, 255, 0);

		iText(
			350,
			280,
			"PAUSED",
			GLUT_BITMAP_TIMES_ROMAN_24
			);

		// RESUME BUTTON
		iSetColor(0, 150, 0);

		iFilledRectangle(
			300,
			210,
			200,
			50
			);

		iSetColor(255, 255, 255);

		iText(
			355,
			228,
			"RESUME",
			GLUT_BITMAP_HELVETICA_18
			);

		
		iShowImage(
			330,
			135,
			140,
			45,
			pauseExitImg
			);

		return;
	}

	
	if (gameOver)
	{
		drawResultScreen();
		return;
	}

	// RESTART BUTTON
	iSetColor(0, 150, 0);

	iFilledRectangle(
		280,
		120,
		240,
		50
		);

	iSetColor(255, 255, 255);

	iText(
		335,
		138,
		"RESTART",
		GLUT_BITMAP_HELVETICA_18
		);

	// BACKGROUND


	if (mediumLevel)
	{
		// MEDIUM LEVEL BACKGROUND
		iShowImage(
			bgX,
			0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			mediumBg1
			);

		iShowImage(
			bgX + SCREEN_WIDTH,
			0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			mediumBg2
			);
	}
	else
	{
		// EASY LEVEL BACKGROUND
		iShowImage(
			bgX,
			0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			bg1
			);

		iShowImage(
			bgX + SCREEN_WIDTH,
			0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			bg2
			);
	}

	if (bgX <= -SCREEN_WIDTH)
	{
		bgX = 0;
	}



	// Coins 

	for (int i = 0; i < MAX_COIN; i++)
	{
		if (coin[i].active)
		{
			iShowImage(coin[i].x,
				coin[i].y,
				30,
				30,
				coinImg);
		}
	}
	// LIFE ITEM 

	if (lifeItem.active)
	{
		iShowImage(
			lifeItem.x,
			lifeItem.y,
			30,
			30,
			lifeImg
			);
	}
	// Enemy 

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		iShowImage(enemy[i].x,
			enemy[i].y,
			enemy[i].w,
			enemy[i].h,
			enemyImg[enemyFrame]);
	}


	// FLYING ENEMY


	if (mediumLevel && flyingEnemy.active)
	{
		iShowImage(
			flyingEnemy.x,
			flyingEnemy.y,
			flyingEnemy.w,
			flyingEnemy.h,
			flyingEnemyImg[flyingEnemyFrame]
			);
	}



	// SPIKES / KATA

	if (mediumLevel){
		for (int i = 0; i < MAX_SPIKE; i++)
		{
			if (spike[i].active)
			{
				iShowImage(
					spike[i].x,
					spike[i].y,
					spike[i].w,
					spike[i].h,
					spikeImg
					);
			}
		}
	}
	// BOSS 

	if (bossActive)
	{
		iShowImage(
			bossX,
			bossY,
			bossW,
			bossH,
			bossImg
			);

		// Boss HP
		iSetColor(255, 0, 0);

		iFilledRectangle(
			bossX,
			bossY + bossH + 5,
			bossHP * 2,
			10
			);

		iSetColor(255, 255, 255);

		iText(
			bossX,
			bossY + bossH + 25,
			"BOSS HP",
			GLUT_BITMAP_HELVETICA_12
			);
	}

	// Enemy Bullet 

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (bullet[i].active)
		{
			iShowImage(bullet[i].x, bullet[i].y, 30, 20, bulletImg);
		}
	}
	// FLYING ENEMY BULLETS
	for (int i = 0; i < MAX_FLYING_BULLET; i++)
	{
		if (flyingBullet[i].active)
		{
			iShowImage(
				(int)flyingBullet[i].x,
				(int)flyingBullet[i].y,
				24,
				16,
				bulletImg
				);
		}
	}

	//  PLAYER DRAW 

	if (playerDead)
	{
		// Die animation
		iShowImage(
			charX,
			charY,
			playerW,
			playerH,
			dieImg[dieFrame]
			);
	}
	else if (playerHit)
	{
		// Hit animation
		iShowImage(
			charX,
			charY,
			playerW,
			playerH,
			hitImg[hitFrame]
			);
	}
	else if (playerJump)
	{
		// Jump image
		iShowImage(
			charX,
			charY,
			playerW,
			playerH,
			jumpImg
			);
	}
	else
	{
		// Normal Run animation
		iShowImage(
			charX,
			charY,
			playerW,
			playerH,
			charImg[idx]
			);
	}

	// Small dotted mouse-aim indicator
	drawAimIndicator();

	//hud
	char ammoText[50];

	sprintf_s(ammoText, sizeof(ammoText), "AMMO : %d / %d",
		ammo, maxAmmo);

	iSetColor(255, 255, 0);

	iText(
		20,
		280,
		ammoText,
		GLUT_BITMAP_HELVETICA_18
		);
	//ammo
	if (ammoActive)
	{
		iShowImage(
			ammoX,
			ammoY,
			40,
			30,
			bulletImg
			);
	}




	// PC GAME HUD 

	char txt[50];

	sprintf_s(txt, "SCORE : %d", score);

	iSetColor(255, 255, 255);
	iText(
		20,
		370,
		txt,
		GLUT_BITMAP_HELVETICA_18
		);
	// PAUSE BUTTON 

	if (!gameOver)
	{
		iSetColor(100, 100, 100);

		iFilledRectangle(
			700,
			340,
			80,
			40
			);

		iSetColor(255, 255, 255);

		iText(
			718,
			353,
			"PAUSE",
			GLUT_BITMAP_HELVETICA_12
			);
	}


	// LIFE 

	char txt2[50];

	sprintf_s(txt2, "LIFE : %d", playerLife);

	iSetColor(255, 255, 255);
	iText(
		20,
		340,
		txt2,
		GLUT_BITMAP_HELVETICA_18
		);


	// COIN 

	char txt3[50];

	sprintf_s(txt3, "COIN : %d", coinCount);

	iSetColor(255, 255, 0);
	iText(
		20,
		310,
		txt3,
		GLUT_BITMAP_HELVETICA_18
		);


	// HEALTH BAR 

	iSetColor(255, 0, 0);

	iFilledRectangle(
		120,
		335,
		playerLife * 25,
		12
		);

	iSetColor(255, 255, 255);

	iText(
		120,
		350,
		"HP",
		GLUT_BITMAP_HELVETICA_12
		);
	// GAME OVER 

	if (gameOver)
	{
		iSetColor(gameComplete ? 80 : 255, gameComplete ? 255 : 0, gameComplete ? 120 : 0);

		iText(
			gameComplete ? 275 : 300,
			250,
			gameComplete ? "GAME COMPLETE!" : "GAME OVER",
			GLUT_BITMAP_TIMES_ROMAN_24
			);

		iSetColor(255, 255, 255);

		char finalScore[50];

		sprintf_s(
			finalScore,
			"FINAL SCORE : %d",
			score
			);

		iText(
			310,
			210,
			finalScore,
			GLUT_BITMAP_HELVETICA_18
			);

		char finalCoin[50];

		sprintf_s(
			finalCoin,
			"COINS : %d",
			coinCount
			);

		iText(
			350,
			180,
			finalCoin,
			GLUT_BITMAP_HELVETICA_18
			);

		if (gameComplete)
		{
			iSetColor(0, 0, 0);
			iText(280, 150, "BOSS DEFEATED - YOU WIN!", GLUT_BITMAP_HELVETICA_12);
		}


	}

	// Player Bullet 

	for (int i = 0; i < MAX_PLAYER_BULLET; i++)
	{
		if (pBullet[i].active)
		{


			iShowImage(
				(int)pBullet[i].x,
				(int)pBullet[i].y,
				(int)pBullet[i].w,
				(int)pBullet[i].h,
				playerBulletImg);
		}
	}




}
