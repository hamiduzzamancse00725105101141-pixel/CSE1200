#pragma warning(disable:4996)
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "GameGlobals.h"
#include "Collision.h"
#include "Score.h"
#include "Audio.h"
#include "GameLogic.h"
#include "GraphicsAPI.h"

void resetGameObjects()
{
	gameOver = false; gameComplete = false; scoreSaved = false; bossDefeated = false; paused = false;
	score = 0; coinCount = 0; playerLife = 5; ammo = maxAmmo;
	bgX = 0; charX = 80; charY = 35; idx = 0; bgSpeed = 5;
	playerJump = false; playerHit = false; playerDead = false; hitFrame = 0; dieFrame = 0;
	bossActive = false; bossHP = 80; bossX = SCREEN_WIDTH + 300; bossY = 30; bossFireTimer = 0;
	for (int i = 0; i < MAX_ENEMY; i++) { enemy[i].x = SCREEN_WIDTH + 200 + i * 300; enemy[i].y = 35; }
	for (int i = 0; i < MAX_BULLET; i++) bullet[i].active = false;
	for (int i = 0; i < MAX_PLAYER_BULLET; i++) pBullet[i].active = false;
	for (int i = 0; i < MAX_FLYING_BULLET; i++) flyingBullet[i].active = false;
	for (int i = 0; i < MAX_COIN; i++) { coin[i].active = true; coin[i].x = SCREEN_WIDTH + 100 + i * 180; coin[i].y = 40 + rand() % 50; }
	flyingEnemy.active = false; flyingEnemy.x = SCREEN_WIDTH + 500; flyingEnemy.y = 200; flyingEnemyFrame = 0;
	for (int i = 0; i < MAX_SPIKE; i++) spike[i].active = false;
	lifeItem.x = SCREEN_WIDTH + 1500; lifeItem.y = 40 + rand() % 60; lifeItem.active = true;
	ammoActive = false;
}

void finishGame(bool completed)
{
	if (gameOver) return;
	gameComplete = completed;
	gameOver = true;
	saveScoreToFile();
	mciSendString("stop mediumsong", NULL, 0, NULL);
	mciSendString("stop easySong", NULL, 0, NULL);
	playGameSound("play ggsong");
}

void autoMove()
{
	if (gameOver)
		return;
	if (!gameStarted)
		return;
	if (paused)
		return;
	// Background 
	bgX -= bgSpeed;
	// AMMO SPAWN 

	static int ammoSpawnTimer = 0;

	ammoSpawnTimer++;

	if (!ammoActive && ammoSpawnTimer >= 400)
	{
		ammoSpawnTimer = 0;

		ammoX = SCREEN_WIDTH + 500 + rand() % 500;
		ammoY = 50 + rand() % 100;

		ammoActive = true;
	}
	// AMMO MOVE 

	if (ammoActive)
	{
		ammoX -= 5;

		if (collision(
			charX,
			charY,
			playerW,
			playerH,
			ammoX,
			ammoY,
			40,
			30))
		{
			ammoActive = false;

			ammo += 10;

			if (ammo > maxAmmo)
				ammo = maxAmmo;
		}

		if (ammoX < -40)
			ammoActive = false;
	}

	// RANDOM LIFE ITEM SPAWN 

	static int lifeTimer = 0;

	lifeTimer++;

	if (!lifeItem.active && lifeTimer >= 500)
	{
		lifeTimer = 0;

		lifeItem.x = SCREEN_WIDTH + 500 + rand() % 500;
		lifeItem.y = 40 + rand() % 70;

		lifeItem.active = true;
	}
	//  FLYING ENEMY ANIMATION 

	static int flyingAnimTimer = 0;

	flyingAnimTimer++;

	if (flyingAnimTimer >= 5)
	{
		flyingEnemyFrame++;

		if (flyingEnemyFrame >= 2)
			flyingEnemyFrame = 0;

		flyingAnimTimer = 0;
	}
	// Player Animation 
	static int enemyTimer = 0;
	enemyTimer++;
	if (enemyTimer >= 5)
	{
		enemyFrame++;
		if (enemyFrame >= 3)
			enemyFrame = 0;
		enemyTimer = 0;
	}
	//  RUN ANIMATION 

	if (!playerJump && !playerHit && !playerDead)
	{
		static int runTimer = 0;

		runTimer++;

		if (runTimer >= 4)
		{
			idx++;

			if (idx >= RUN_FRAMES)
				idx = 0;

			runTimer = 0;
		}
	}



	// COIN MOVE 

	for (int i = 0; i < MAX_COIN; i++)
	{
		if (!coin[i].active)
			continue;

		coin[i].x -= 5;

		// Coin collect
		if (collision(
			charX, charY,
			playerW, playerH,
			coin[i].x, coin[i].y,
			30, 30))
		{
			score += 1;
			coinCount++;
			//new coin spawn
			coin[i].x = SCREEN_WIDTH + 700 + rand() % 2000;
			coin[i].y = 50 + rand() % 60;

		}

		// Coin out of screen
		if (coin[i].x < -30)
		{
			coin[i].x = SCREEN_WIDTH + 1000 + rand() % 4000;
			coin[i].y = 50 + rand() % 60;
		}
	}

	// LIFE ITEM MOVE 

	if (lifeItem.active)
	{
		lifeItem.x -= 5;

		if (collision(
			charX,
			charY,
			playerW,
			playerH,
			lifeItem.x,
			lifeItem.y,
			30,
			30))
		{
			lifeItem.active = false;

			if (playerLife < 7)
			{
				playerLife++;
			}
			score += 5;
		}

		if (lifeItem.x < -30)
		{
			lifeItem.active = false;
		}
	}







	// Enemy Fire 
	static int fireTimer = 0;

	fireTimer++;

	if (fireTimer >= 60)
	{
		fireTimer = 0;

		for (int i = 0; i < MAX_ENEMY; i++)
		{
			for (int j = 0; j < MAX_BULLET; j++)
			{
				if (!bullet[j].active)
				{
					bullet[j].active = true;

					bullet[j].x = enemy[i].x;
					bullet[j].y = enemy[i].y + enemy[i].h / 2;

					break;
				}
			}
		}
	}


	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (!bullet[i].active)
			continue;

		bullet[i].x -= 10;

		if (bullet[i].x < -30)
		{
			bullet[i].active = false;
			continue;
		}

		if (collision(
			charX,
			charY,
			playerW,
			playerH,
			bullet[i].x,
			bullet[i].y,
			30,
			20))
		{
			bullet[i].active = false;

			if (!shieldActive)
			{
				playerTakeHit();

				if (playerLife <= 0)
				{
					playerLife = 0;

				}
			}
		}
	}
	// ENEMY MOVE 

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		enemy[i].x -= bgSpeed;

		// Enemy hit player
		if (collision(
			charX, charY,
			playerW, playerH,
			enemy[i].x, enemy[i].y,
			enemy[i].w, enemy[i].h))
		{
			// Player life
			playerTakeHit();

			// Enemy repeat spawn 
			enemy[i].x = SCREEN_WIDTH + 200 + rand() % 400;
			enemy[i].y = 35;

			// if Life 0 Game Over
			if (playerLife <= 0)
			{
				playerLife = 0;
			}
		}

		// Enemy out of  screen spawn
		if (enemy[i].x < -enemy[i].w)
		{
			enemy[i].x = SCREEN_WIDTH + 200 + rand() % 400;
			enemy[i].y = 35;
		}
	}

	// BOSS SYSTEM - runs independently of player shooting
	if (score >= 50 && !bossActive && !bossDefeated)
	{
		bossActive = true;
		bossX = SCREEN_WIDTH + 300;
		bossY = 30;
		bossHP = 80;
		bossFireTimer = 0;
	}

	if (bossActive)
	{
		if (bossX > 600)
			bossX -= 3;

		bossFireTimer++;
		if (bossFireTimer >= 80)
		{
			bossFireTimer = 0;
			int fired = 0;

			for (int b = 0; b < MAX_BULLET; b++)
			{
				if (!bullet[b].active)
				{
					bullet[b].active = true;
					bullet[b].x = bossX;
					if (fired == 0)
					{
						bullet[b].y = bossY + 15;
					}
					else
					{
						bullet[b].y = bossY + 100;
					}
					fired++;
					if (fired == 2) break;
				}
			}
		}
	}

	// PLAYER BULLET SYSTEM 

	for (int i = 0; i < MAX_PLAYER_BULLET; i++)
	{
		if (!pBullet[i].active)
			continue;

		// Bullet move according to mouse direction
		pBullet[i].x += pBullet[i].dx * 8.0f;
		pBullet[i].y += pBullet[i].dy * 8.0f;


		// Bullet Outside Screen 

		if (pBullet[i].x > SCREEN_WIDTH ||
			pBullet[i].x + pBullet[i].w < 0 ||
			pBullet[i].y > SCREEN_HEIGHT ||
			pBullet[i].y + pBullet[i].h < 0)
		{
			pBullet[i].active = false;
			continue;
		}

		// Player Bullet vs Enemy 

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			if (collision(
				(int)pBullet[i].x,
				(int)pBullet[i].y,
				(int)pBullet[i].w,
				(int)pBullet[i].h,

				enemy[j].x,
				enemy[j].y,
				enemy[j].w,
				enemy[j].h))
			{
				// Bullet disappear
				pBullet[i].active = false;

				// Enemy respawn
				enemy[j].x = SCREEN_WIDTH + 200 + rand() % 400;
				enemy[j].y = 35;

				// Score
				score += 5;

				break;
			}
		}



		// PLAYER BULLET HITS BOSS
		if (bossActive && pBullet[i].active)
		{
			if (collision(
				(int)pBullet[i].x,
				(int)pBullet[i].y,
				(int)pBullet[i].w,
				(int)pBullet[i].h,
				bossX,
				bossY,
				bossW,
				bossH))
			{
				pBullet[i].active = false;
				bossHP -= 10;

				if (bossHP <= 0)
				{
					bossHP = 0;
					bossActive = false;
					bossDefeated = true;

					score += 100;
					combo += 5;
					missionKill++;

					// Boss defeat completes both Easy and Medium.
					finishGame(true);
				}
			}
		}


		// Player Bullet vs Enemy Bullet 

		if (!pBullet[i].active)
			continue;

		for (int j = 0; j < MAX_BULLET; j++)
		{
			if (!bullet[j].active)
				continue;

			if (collision(
				(int)pBullet[i].x,
				(int)pBullet[i].y,
				(int)pBullet[i].w,
				(int)pBullet[i].h,

				bullet[j].x,
				bullet[j].y,
				20,
				10))
			{
				// Both bullets disappear
				pBullet[i].active = false;
				bullet[j].active = false;

				break;
			}
		}
	}

	// MEDIUM LEVEL
	

	if (mediumLevel){
		static int flyingSpawnTimer = 0;

		flyingSpawnTimer++;


		// Flying Enemy Spawn

		if (!flyingEnemy.active && flyingSpawnTimer >= 300)
		{
			flyingSpawnTimer = 0;

			flyingEnemy.x = SCREEN_WIDTH + 300 + rand() % 500;

			flyingEnemy.y = 150 + rand() % 120;

			flyingEnemy.w = 70;
			flyingEnemy.h = 70;

			flyingEnemy.speed = 5;

			flyingEnemy.active = true;
		}


		// Flying Enemy Move

		if (flyingEnemy.active)
		{
			flyingEnemy.x -= flyingEnemy.speed;


			// Up and Down Movement

			static int flyMoveTimer = 0;

			flyMoveTimer++;

			if (flyMoveTimer % 20 < 10)
			{
				flyingEnemy.y += 2;
			}
			else
			{
				flyingEnemy.y -= 2;
			}


			// Flying Enemy hits Player

			if (collision(
				charX,
				charY,
				playerW,
				playerH,

				flyingEnemy.x,
				flyingEnemy.y,
				flyingEnemy.w,
				flyingEnemy.h))
			{
				if (!shieldActive)
				{
					playerTakeHit();
					combo = 0;
				}

				flyingEnemy.active = false;

				flyingEnemy.x = SCREEN_WIDTH + 500;
			}


			// Flying Enemy out of screen

			if (flyingEnemy.x < -flyingEnemy.w)
			{
				flyingEnemy.active = false;
			}
		}



		// FLYING ENEMY FIRE - shoots toward the player's current position
		static int flyingFireTimer = 0;

		if (flyingEnemy.active)
		{
			flyingFireTimer++;

			if (flyingFireTimer >= 70)
			{
				flyingFireTimer = 0;

				for (int i = 0; i < MAX_FLYING_BULLET; i++)
				{
					if (!flyingBullet[i].active)
					{
						flyingBullet[i].active = true;
						flyingBullet[i].x = flyingEnemy.x + flyingEnemy.w / 2.0f;
						flyingBullet[i].y = flyingEnemy.y + flyingEnemy.h / 2.0f;

						float targetX = charX + playerW / 2.0f;
						float targetY = charY + playerH / 2.0f;
						float dx = targetX - flyingBullet[i].x;
						float dy = targetY - flyingBullet[i].y;
						float length = sqrt(dx * dx + dy * dy);

						if (length > 0.001f)
						{
							flyingBullet[i].dx = dx / length;
							flyingBullet[i].dy = dy / length;
						}
						else
						{
							flyingBullet[i].dx = -1.0f;
							flyingBullet[i].dy = 0.0f;
						}

						break;
					}
				}
			}
		}
		else
		{
			flyingFireTimer = 0;
		}

		// FLYING ENEMY BULLET MOVE / PLAYER COLLISION
		for (int i = 0; i < MAX_FLYING_BULLET; i++)
		{
			if (!flyingBullet[i].active)
				continue;

			const float flyingBulletSpeed = 9.0f;
			flyingBullet[i].x += flyingBullet[i].dx * flyingBulletSpeed;
			flyingBullet[i].y += flyingBullet[i].dy * flyingBulletSpeed;

			if (flyingBullet[i].x < -40 ||
				flyingBullet[i].x > SCREEN_WIDTH + 40 ||
				flyingBullet[i].y < -40 ||
				flyingBullet[i].y > SCREEN_HEIGHT + 40)
			{
				flyingBullet[i].active = false;
				continue;
			}

			if (collision(
				charX, charY,
				playerW, playerH,
				(int)flyingBullet[i].x, (int)flyingBullet[i].y,
				24, 16))
			{
				flyingBullet[i].active = false;

				if (!shieldActive)
				{
					playerTakeHit();
					combo = 0;
				}
			}
		}

		// SPIKE 


		static int spikeTimer = 0;

		spikeTimer++;


		// Spike Spawn

		if (spikeTimer >= 180)
		{
			spikeTimer = 0;

			for (int i = 0; i < MAX_SPIKE; i++)
			{
				if (!spike[i].active)
				{
					spike[i].x =
						SCREEN_WIDTH + 300 + rand() % 500;

					spike[i].y = 20;

					spike[i].w = 40;
					spike[i].h = 66;

					spike[i].active = true;

					break;
				}
			}
		}


		// Spike Move

		for (int i = 0; i < MAX_SPIKE; i++)
		{
			if (!spike[i].active)
				continue;


			spike[i].x -= 5;


			// Spike hits Player

			if (collision(
				charX,
				charY,
				playerW,
				playerH,

				spike[i].x,
				spike[i].y,
				spike[i].w,
				spike[i].h))
			{
				if (!shieldActive)
				{
					playerTakeHit();
					combo = 0;
				}

				spike[i].active = false;
			}


			// Spike out of screen

			if (spike[i].x < -50)
			{
				spike[i].active = false;
			}
		}
	}
}

void playerTakeHit()
{
	if (playerDead)
		return;

	playerLife--;

	// LIFE 
	if (playerLife <= 0)
	{
		playerLife = 0;

		playerHit = false;
		playerJump = false;

		playerDead = true;

		dieFrame = 0;
		dieAnimTimer = 0;

		return;
	}

	// LIFE 
	playerHit = true;
	playerJump = false;

	hitFrame = 0;
	hitAnimTimer = 0;
}

void fixedUpdate()
{
	if (gameOver)
		return;

	if (!gameStarted)
		return;

	if (paused)
		return;


	// DEAD PLAYER

	if (playerDead)
	{
		dieAnimTimer++;

		if (dieAnimTimer >= 8)
		{
			dieAnimTimer = 0;

			dieFrame++;

			if (dieFrame >= DIE_FRAMES)
			{
				dieFrame = DIE_FRAMES - 1;

				// Die animation finished
				finishGame(false);
			}
		}

		return;
	}


	// HIT ANIMATION

	if (playerHit)
	{
		hitAnimTimer++;

		if (hitAnimTimer >= 6)
		{
			hitAnimTimer = 0;

			hitFrame++;

			if (hitFrame >= HIT_FRAMES)
			{
				hitFrame = 0;

				playerHit = false;
			}
		}

		return;
	}


	// PLAYER LEFT / RIGHT

	if (isKeyPressed('d') ||
		isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		charX += 6;
	}

	if (isKeyPressed('a') ||
		isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		charX -= 6;
	}


	// PLAYER JUMP

	if (isKeyPressed('w') ||
		isSpecialKeyPressed(GLUT_KEY_UP))
	{
		playerJump = true;

		charY = 120;
	}
	else
	{
		playerJump = false;

		charY = 30;
	}


	// X LIMIT

	if (charX < 0)
		charX = 0;

	if (charX > SCREEN_WIDTH - playerW)
		charX = SCREEN_WIDTH - playerW;
}
