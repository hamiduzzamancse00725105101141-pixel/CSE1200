#ifndef SHADOW_SPRINT_GLOBALS_H
#define SHADOW_SPRINT_GLOBALS_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define MAX_ENEMY 1
#define MAX_BULLET 20
#define MAX_COIN 20
#define MAX_PLAYER_BULLET 30
#define RUN_FRAMES 5
#define HIT_FRAMES 3
#define DIE_FRAMES 2
#define INTRO_FRAMES 30
#define MAX_FLYING_BULLET 10
#define MAX_SPIKE 5

extern int introImg[INTRO_FRAMES];
extern int introFrame;
extern bool introFinished;
extern int bg1, bg2;
extern int mediumBg1, mediumBg2;
extern int bgX;
extern int startImg;
extern int levelSelectImg;
extern int pauseExitImg;
extern int easyGameOverImg, easyLevelCompleteImg;
extern int mediumGameOverImg, mediumLevelCompleteImg;
extern bool levelSelect;
extern bool highScorePage;
extern int highScorePageNumber;
extern const int SCORES_PER_PAGE;
extern bool settingsPage;
extern bool creditsPage;
extern bool soundEnabled;
extern bool gameComplete;
extern bool scoreSaved;
extern bool bossDefeated;
extern int enemyImg[4];
extern int enemyFrame;
extern int bulletImg;
extern int lifeImg;
extern int coinImg;
extern int playerBulletImg;
extern int mouseX, mouseY;
extern bool gameOver;
extern bool gameStarted;
extern bool mediumLevel;
extern bool paused;
extern int coinCount;
extern int playerLife;
extern int charImg[RUN_FRAMES];
extern int jumpImg;
extern int hitImg[HIT_FRAMES];
extern int dieImg[DIE_FRAMES];
extern int charX, charY;
extern int playerW, playerH;
extern int idx;
extern bool playerJump, playerHit, playerDead;
extern int hitFrame, dieFrame;
extern int hitAnimTimer, dieAnimTimer;
extern int bgSpeed;
extern int score;

extern int bossImg;
extern int bossX, bossY, bossW, bossH;
extern int bossHP;
extern bool bossActive;
extern int bossFireTimer;

extern int ammo, maxAmmo;
extern int ammoX, ammoY;
extern bool ammoActive;

extern int powerImg;
extern int powerX, powerY;
extern bool powerActive;
extern int powerType;
extern bool shieldActive, doubleBulletActive, speedBoostActive;
extern int powerTimer;

extern int combo, comboTimer;
extern int missionKill, missionTarget;
extern bool nightMode;

struct Enemy {
    int x;
    int y;
    int w;
    int h;
};

extern Enemy enemy[3];

struct Bullet {
    int x;
    int y;
    bool active;
};

extern Bullet bullet[MAX_BULLET];

struct LifeItem {
    int x;
    int y;
    bool active;
};

extern LifeItem lifeItem;

struct Coin {
    int x;
    int y;
    bool active;
};

extern Coin coin[MAX_COIN];

struct PlayerBullet {
    float x;
    float y;
    float w;
    float h;
    float dx;
    float dy;
    bool active;
};

extern PlayerBullet pBullet[MAX_PLAYER_BULLET];

struct FlyingEnemy {
    int x;
    int y;
    int w;
    int h;
    int speed;
    bool active;
};

extern FlyingEnemy flyingEnemy;
extern int flyingEnemyImg[2];
extern int flyingEnemyFrame;

struct FlyingBullet {
    float x;
    float y;
    float dx;
    float dy;
    bool active;
};

extern FlyingBullet flyingBullet[MAX_FLYING_BULLET];

struct Spike {
    int x;
    int y;
    int w;
    int h;
    bool active;
};

extern Spike spike[MAX_SPIKE];
extern int spikeImg;

#endif
