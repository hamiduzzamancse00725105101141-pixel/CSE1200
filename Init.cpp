#pragma warning(disable:4996)
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "GameGlobals.h"
#include "Audio.h"
#include "GraphicsAPI.h"
#include "Init.h"

void initGame()
{
	srand((unsigned)time(0));
	settingsPage = false;
	creditsPage = false;
	soundEnabled = true;
	// MAIN MENU MUSIC
	mciSendString("open \"Audios//intro.mp3\" alias introsong", NULL, 0, NULL);
	mciSendString("open \"Audios//medium.mp3\" alias mediumsong", NULL, 0, NULL);
	mciSendString("open \"Audios//easy.mp3\" alias easySong", NULL, 0, NULL);
	mciSendString("open \"Audios//menu.mp3\" alias menusong", NULL, 0, NULL);
	mciSendString("open \"Audios//gg.mp3\" alias ggsong", NULL, 0, NULL);
	playGameSound("play introsong");

	bg1 = iLoadImage("Images//bg1.png");
	bg2 = iLoadImage("Images//bg1.png");
	startImg = iLoadImage("Images//start.png");
	levelSelectImg = iLoadImage("Images//levelselect.png");

	
	pauseExitImg = iLoadImage("Images//pause_exit.png");

	// RESULT SCREENS
	easyGameOverImg = iLoadImage("Images//easy1.jpg");
	easyLevelCompleteImg = iLoadImage("Images//easy2.jpg");
	mediumGameOverImg = iLoadImage("Images//medium1.jpg");
	mediumLevelCompleteImg = iLoadImage("Images//medium2.jpg");
	powerImg = iLoadImage("Images//power.png");
	bossImg = iLoadImage("Images//boss.png");
	spikeImg = iLoadImage("Images//spike.png");
	mediumBg1 = iLoadImage("Images//mediumBg1.png");
	mediumBg2 = iLoadImage("Images//mediumBg2.png");
	bulletImg = iLoadImage("Images//bullet.png");
	coinImg = iLoadImage("Images//coin.png");
	lifeImg = iLoadImage("Images//life.png");
	playerBulletImg = iLoadImage("Images//playerBullet.png");

	// INTRO ANIMATION
	for (int i = 0; i < INTRO_FRAMES; i++)
	{
		char file[100];
		sprintf_s(file, sizeof(file), "Images//intro_%02d.png", i + 1);

		introImg[i] = iLoadImage(file);
	}
	//  LOAD PLAYER RUN IMAGES 

	for (int i = 0; i < 5; i++)
	{
		char file[100];

		sprintf_s(file, sizeof(file), "Images//c%d.png", i + 1);

		charImg[i] = iLoadImage(file);
	}


	//  LOAD JUMP IMAGE 

	jumpImg = iLoadImage("Images//jump.png");


	//  LOAD HIT IMAGES 

	for (int i = 0; i < 3; i++)
	{
		char file[100];

		sprintf_s(file, sizeof(file), "Images//hit%d.png", i + 1);

		hitImg[i] = iLoadImage(file);
	}


	//  LOAD DIE IMAGES 

	for (int i = 0; i < 2; i++)
	{
		char file[100];

		sprintf_s(file, sizeof(file), "Images//die%d.png", i + 1);

		dieImg[i] = iLoadImage(file);
	}
	for (int i = 0; i < 4; i++)
	{
		char file[100];
		sprintf_s(file, sizeof(file), "Images//enemy%d.png", i + 1);
		enemyImg[i] = iLoadImage(file);
	}
	// FLYING ENEMY
	for (int i = 0; i < 2; i++)
	{
		char file[100];
		sprintf_s(file, sizeof(file), "Images//fly%d.png", i + 1);
		flyingEnemyImg[i] = iLoadImage(file);
	}


	// Enemy
	for (int i = 0; i<MAX_ENEMY; i++)
	{
		enemy[i].x = 900 + i * 250;
		enemy[i].y = 35;
		enemy[i].w = 60;
		enemy[i].h = 100;
	}

	// Bullet
	for (int i = 0; i<MAX_BULLET; i++)
	{
		bullet[i].active = false;
	}

	// Coin


	for (int i = 0; i < MAX_COIN; i++)
	{
		coin[i].x = SCREEN_WIDTH + 100 + i * 180;
		coin[i].y = 40 + rand() % 50;
		coin[i].active = true;
	}
	// LIFE ITEM 

	lifeItem.x = SCREEN_WIDTH + 1500;
	lifeItem.y = 40 + rand() % 60;
	lifeItem.active = true;
	// Player Bullet
	for (int i = 0; i < MAX_PLAYER_BULLET; i++)
	{
		pBullet[i].active = false;

		pBullet[i].x = 0;
		pBullet[i].y = 0;

		pBullet[i].w = 45;
		pBullet[i].h = 10;

		pBullet[i].dx = 0;
		pBullet[i].dy = 0;
	}
	//  FLYING ENEMY INITIALIZE 

	for (int i = 0; i < MAX_FLYING_BULLET; i++)
	{
		flyingBullet[i].x = 0;
		flyingBullet[i].y = 0;
		flyingBullet[i].dx = 0;
		flyingBullet[i].dy = 0;
		flyingBullet[i].active = false;
	}

	flyingEnemy.x = SCREEN_WIDTH + 500;
	flyingEnemy.y = 200;

	flyingEnemy.w = 70;
	flyingEnemy.h = 70;

	flyingEnemy.speed = 5;

	flyingEnemy.active = false;

	flyingEnemyFrame = 0;


	//  SPIKE INITIALIZE 

	for (int i = 0; i < MAX_SPIKE; i++)
	{
		spike[i].x = SCREEN_WIDTH + 300 + i * 500;

		spike[i].y = 20;

		spike[i].w = 40;
		spike[i].h = 40;

		spike[i].active = false;
	}
}
