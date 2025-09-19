#undef UNICODE
#undef _UNICODE

#include "draw.h"
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include "fight.h"
#include "time.h"
#pragma comment( lib, "MSIMG32.LIB")

int enemy1_draw[ENEMY1_NUM] = { 0 };  //enemy1爆炸动画计数器
int enemy2_draw[ENEMY2_NUM] = { 0 };
int enemy3_draw = 0;
int enemy4_draw[ENEMY4_NUM] = { 0 };
int hole_draw[HOLE_NUM] = { 0 };
int missile_draw = 0;
int player_draw = 0;
bool end = false;
int record;		//最高记录

IMAGE bk;
IMAGE player_image[2];
IMAGE player_down[4];
IMAGE enemy_image[4];
IMAGE enemy1_down[4];
IMAGE enemy2_down[4];
IMAGE enemy4_down[3];
IMAGE enemy3_down[6];
IMAGE bullet_image[2];
IMAGE laser_image[3];
IMAGE missile_image;
IMAGE boom;
IMAGE defense_image;
IMAGE hp_image;
IMAGE energybag_image;

plane player;
plane enemy1[ENEMY1_NUM];
plane enemy2[ENEMY2_NUM];
plane enemy4[ENEMY4_NUM];
plane enemy3;
plane bullet[BULLET_NUM];
plane e_bullet[BULLET_NUM];
plane laser[LASER_NUM];
plane missile;
plane defense;
plane hole[HOLE_NUM];

item hp;
item energybag;

//代替putimage
static void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg) 
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	const int w = srcimg->getwidth();
	const int h = srcimg->getheight();
	const BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}
static void outText()
{
	char hpText[20];  
	sprintf_s(hpText, "HP: %d", player.hp);  
	outtextxy(100, 100, hpText);  
	char scoreText[20]; 
	sprintf_s(scoreText, "分数: %d", score);  
	outtextxy(100, 120, scoreText);  
	char distanceText[20];
	sprintf_s(distanceText, "飞行距离: % d m", distance);
	outtextxy(100, 140, distanceText);
	char difficultyText[20];
	sprintf_s(difficultyText, "难度系数: %d", difficulty_num);
	outtextxy(100, 160, difficultyText);
	if (record == -1)
	{
		outtextxy(1400, 100, "暂无记录");
	}
	else
	{
		char recordText[20];
		sprintf_s(recordText, "最好记录: %d", record);
		outtextxy(1400, 100, recordText);
	}
}
//计时器
bool timer(int ms, int id)                 //0号用于发射子弹 1号用于控制enemy1刷新 2-5号用于enemy2发射子弹 6-7用于enemy4移动 8-9用于enemy4发射激光
{											//10号用于enemy3移动,11号用于enemy3发射子弹 12号用于帧率控制 13号用于enemy4发射导弹 14号用于血包生成 15号用于能量球生成 16号用于enemy4产生黑洞

	static DWORD t[20] = { 0 };
	if (clock() - t[id] > ms)
	{
		t[id] = clock();   
		return true;
	}
	return false;
}         
static void loadimg()
{
	loadimage(&bk, "bk.png");
	loadimage(&player_image[0], "me1.png");
	loadimage(&player_image[1], "j20.png");
	loadimage(&bullet_image[0], "bullet2.png");
	loadimage(&bullet_image[1], "bullet1.png");
	loadimage(&enemy_image[0], "enemy1.png");
	loadimage(&enemy_image[1], "enemy2.png");
	loadimage(&enemy_image[2], "enemy3_n1.png");
	loadimage(&enemy_image[3], "enemy4.png");
	loadimage(&enemy1_down[0], "enemy1_down1.png");
	loadimage(&enemy1_down[1], "enemy1_down2.png");
	loadimage(&enemy1_down[2], "enemy1_down3.png");
	loadimage(&enemy1_down[3], "enemy1_down4.png");
	loadimage(&enemy2_down[0], "enemy2_down1.png");
	loadimage(&enemy2_down[1], "enemy2_down2.png");
	loadimage(&enemy2_down[2], "enemy2_down3.png");
	loadimage(&enemy2_down[3], "enemy2_down4.png");
	loadimage(&enemy4_down[0], "enemy4_down1.png");
	loadimage(&enemy4_down[1], "enemy4_down2.png");
	loadimage(&enemy4_down[2], "enemy4_down3.png");
	loadimage(&laser_image[0], "enemy4_shoot.png");
	loadimage(&laser_image[1], "laser1.png");
	loadimage(&laser_image[2], "laser2.png");
	loadimage(&player_down[0], "me_destroy_1.png");
	loadimage(&player_down[1], "me_destroy_2.png");
	loadimage(&player_down[2], "me_destroy_3.png");
	loadimage(&player_down[3], "me_destroy_4.png");
	loadimage(&enemy3_down[0], "enemy3_down1.png");
	loadimage(&enemy3_down[1], "enemy3_down2.png");
	loadimage(&enemy3_down[2], "enemy3_down3.png");
	loadimage(&enemy3_down[3], "enemy3_down4.png");
	loadimage(&enemy3_down[4], "enemy3_down5.png");
	loadimage(&enemy3_down[5], "enemy3_down6.png");
	loadimage(&missile_image, "missile.png");
	loadimage(&boom, "boom.png");
	loadimage(&defense_image, "defense.png");
	loadimage(&hp_image, "hp+.png");
	loadimage(&energybag_image, "energy+.png");
}
static void enemy1_damage()
{
	for (int i = 0; i < ENEMY1_NUM; i++)
	{
		if (enemy1[i].damage == true)
		{
			if (enemy1_draw[i] <= 50)
			{
				transparentimage3(NULL, enemy1[i].x, enemy1[i].y, &enemy1_down[0]);
				enemy1_draw[i]++;
			}
			else if (enemy1_draw[i] > 50 && enemy1_draw[i] <= 100)
			{
				transparentimage3(NULL, enemy1[i].x, enemy1[i].y, &enemy1_down[1]);
				enemy1_draw[i]++;
			}
			else if (enemy1_draw[i] > 100 && enemy1_draw[i] <= 150)
			{
				transparentimage3(NULL, enemy1[i].x, enemy1[i].y, &enemy1_down[2]);
				enemy1_draw[i]++;
			}
			else if (enemy1_draw[i] > 150 && enemy1_draw[i] <= 200)
			{
				transparentimage3(NULL, enemy1[i].x, enemy1[i].y, &enemy1_down[3]);
				enemy1_draw[i]++;
			}
			else
			{
				enemy1[i].damage = false; 
				enemy1_draw[i] = 0;       
			}
		}
	}
}
static void enemy2_damage()
{
	for (int i = 0; i < ENEMY2_NUM; i++)
	{
		if (enemy2[i].damage == true)
		{
			if (enemy2_draw[i] <= 60)
			{
				transparentimage3(NULL, enemy2[i].x, enemy2[i].y, &enemy2_down[0]);
				enemy2_draw[i]++;
			}
			else if (enemy2_draw[i] > 60 && enemy2_draw[i] <= 120)
			{
				transparentimage3(NULL, enemy2[i].x, enemy2[i].y, &enemy2_down[1]);
				enemy2_draw[i]++;
			}
			else if (enemy2_draw[i] > 120 && enemy2_draw[i] <= 180)
			{
				transparentimage3(NULL, enemy2[i].x, enemy2[i].y, &enemy2_down[2]);
				enemy2_draw[i]++;
			}
			else if (enemy2_draw[i] > 180 && enemy2_draw[i] <= 240)
			{
				transparentimage3(NULL, enemy2[i].x, enemy2[i].y, &enemy2_down[3]);
				enemy2_draw[i]++;
			}
			else
			{
				enemy2[i].damage = false;
				enemy2_draw[i] = 0;
			}
		}
	}
}
static void enemy4_damage()
{
	for (int i = 0; i < ENEMY4_NUM; i++)
	{
		if (enemy4[i].damage == true)
		{
			if (enemy4_draw[i] <= 60)
			{
				transparentimage3(NULL, enemy4[i].x, enemy4[i].y, &enemy4_down[0]);
				enemy4_draw[i]++;
			}
			else if (enemy4_draw[i] > 60 && enemy4_draw[i] <= 120)
			{
				transparentimage3(NULL, enemy4[i].x, enemy4[i].y, &enemy4_down[1]);
				enemy4_draw[i]++;
			}
			else if (enemy4_draw[i] > 120 && enemy4_draw[i] <= 200)
			{
				transparentimage3(NULL, enemy4[i].x, enemy4[i].y, &enemy4_down[2]);
				enemy4_draw[i]++;
			}
			else
			{
				enemy4[i].damage = false;
				enemy4_draw[i] = 0;
			}
		}
	}
}
static void enemy3_damage()
{
	if (enemy3.damage == true)
	{
		if (enemy3_draw <= 60)
		{
			transparentimage3(NULL, enemy3.x, enemy3.y, &enemy3_down[0]);
			enemy3_draw++;
		}
		else if (enemy3_draw > 60 && enemy3_draw <= 120)
		{
			transparentimage3(NULL, enemy3.x, enemy3.y, &enemy3_down[1]);
			enemy3_draw++;
		}
		else if (enemy3_draw > 120 && enemy3_draw <= 180)
		{
			transparentimage3(NULL, enemy3.x, enemy3.y, &enemy3_down[2]);
			enemy3_draw++;
		}
		else if (enemy3_draw > 180 && enemy3_draw <= 240)
		{
			transparentimage3(NULL, enemy3.x, enemy3.y, &enemy3_down[3]);
			enemy3_draw++;
		}
		else if (enemy3_draw > 240 && enemy3_draw <= 300)
		{
			transparentimage3(NULL, enemy3.x, enemy3.y, &enemy3_down[4]);
			enemy3_draw++;
		}
		else if (enemy3_draw > 300 && enemy3_draw <= 400)
		{
			transparentimage3(NULL, enemy3.x, enemy3.y, &enemy3_down[5]);
			enemy3_draw++;
		}
		else
		{
			enemy3.damage = false;
			enemy3_draw = 0;
		}
	}
}
static void player_damage()
{
	if (player.live == false)
	{
		if (player_draw <= 100)
		{
			transparentimage3(NULL, player.x, player.y, &player_down[0]);
			player_draw++;
		}
		else if (player_draw> 100 && player_draw <= 200)
		{
			transparentimage3(NULL, player.x, player.y, &player_down[1]);
			player_draw++;
		}
		else if (player_draw > 200 && player_draw <= 300)
		{
			transparentimage3(NULL, player.x, player.y, &player_down[2]);
			player_draw++;
		}
		else if (player_draw > 300 && player_draw <= 400)
		{
			transparentimage3(NULL, player.x, player.y, &player_down[3]);
			player_draw++;
		}
		else
		{
			
		}
	}
}
static void missile_damage()
{
	if (missile.damage == true)
	{
		if (missile_draw <= 150)
		{
			transparentimage3(NULL, missile.x+missile.width/2-40, missile.y+missile.height, &boom);
			missile_draw++;
		}
		else
		{
			missile_draw = 0;
			missile.damage = false;
		}
	}
}
static void bkRoll(bool defense_live)		//背景滚动
{
	static int init_bk_yPosition = -2334;
	putimage(0, init_bk_yPosition, &bk);
	if (defense_live==false)
		init_bk_yPosition++;
	else
		init_bk_yPosition+=2;
	if (init_bk_yPosition >= 0)
	{
		init_bk_yPosition = -2334;
	}
}
static void energyDraw()
{
	rectangle(100, 200, 150, 600);
	if (energy != MAX)
	{
		setfillcolor(BLUE);
		outtextxy(95, 610, "护盾能量");
	}
	else
	{
		setfillcolor(GREEN);
		outtextxy(95, 610, "按B使用");
	}
	fillrectangle(100, 200, 150, energy/21+200);
}
static void holeCreate()
{
	for (int i = 0; i < HOLE_NUM; i++)
	{
		if (hole[i].live == true)
		{
			setfillcolor(BLACK);
			if (0 <= hole_draw[i] && hole_draw[i] < 50)
				fillcircle(hole[i].x, hole[i].y, 5);
			if (50 <= hole_draw[i] && hole_draw[i] < 100)
				fillcircle(hole[i].x, hole[i].y, 10);
			if (100 <= hole_draw[i] && hole_draw[i] < 150)
				fillcircle(hole[i].x, hole[i].y, 15);
			if (150 <= hole_draw[i] && hole_draw[i] < 200)
				fillcircle(hole[i].x, hole[i].y, 20);
			if (200 <= hole_draw[i] && hole_draw[i] < 250)
				fillcircle(hole[i].x, hole[i].y, 25);
			if (250 <= hole_draw[i] && hole_draw[i] < 300)
				fillcircle(hole[i].x, hole[i].y, 30);
			if (300 <= hole_draw[i] && hole_draw[i] < 350)
				fillcircle(hole[i].x, hole[i].y, 35);
			if (350 <= hole_draw[i] && hole_draw[i] < 400)
				fillcircle(hole[i].x, hole[i].y, 40);
			if (400 <= hole_draw[i] && hole_draw[i] < 450)
				fillcircle(hole[i].x, hole[i].y, 45);
			if (450 <= hole_draw[i])
			{
				fillcircle(hole[i].x, hole[i].y, 50);
				hole[i].damage = true;
			}
			hole_draw[i]++;
		}
		else
		{
			hole_draw[i] = 0;
			hole[i].damage = false;
		}
	}
}
void playInit()
{   //玩家初始化
	player.width = 100;
	player.height = 100;
	player.x = BKWIDTH/2-player.width/2;
	player.y = BKHEIGHT-player.height-20;
	player.hp = 2000;
	player.live = true;
	//盾初始化
	defense.width = 150;
	defense.height = 150;
	defense.hp = 300;
	defense.x = 0;
	defense.y = 0;
	defense.live = false;
	//子弹初始化
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].live = false;
	}
	//敌机初始化
	for (int i = 0; i < ENEMY2_NUM; i++)
	{
		enemy2[i].width = 100;
		enemy2[i].height = 100;
		enemy2[i].x = 0-enemy2[i].width;
		enemy2[i].y = BKHEIGHT;
		enemy2[i].hp = 1000;
		enemy2[i].live = false;
		enemy2[i].damage = false;  
	}
	for (int i = 0; i < ENEMY1_NUM; i++)
	{
		enemy1[i].width = 57;
		enemy1[i].height = 43;
		enemy1[i].x = 0 - enemy1[i].width;
		enemy1[i].y = BKHEIGHT;
		enemy1[i].hp = 10;
		enemy1[i].live = false;
		enemy1[i].damage = false;
	}
	for (int i = 0; i < ENEMY4_NUM; i++)
	{
		enemy4[i].width = 100;
		enemy4[i].height = 100;
		enemy4[i].x = 0 - enemy4[i].width;
		enemy4[i].y = BKHEIGHT;
		enemy4[i].hp = 300;
		enemy4[i].live = false;
		enemy4[i].damage = false;
	}
	enemy3.width = 254;
	enemy3.height = 388;
	enemy3.x = 0 - enemy3.width;
	enemy3.y = BKHEIGHT;
	enemy3.hp = 40000;
	enemy3.live = false;
	enemy3.damage = false;
	
	//敌机子弹初始化
	for (int i = 0; i < BULLET_NUM; i++)
	{
		e_bullet[i].x = 0;
		e_bullet[i].y = 0;
		e_bullet[i].live = false;
	}
	//敌机激光初始化
	for (int i = 0; i < LASER_NUM; i++)
	{
		laser[i].x = 0;
		laser[i].y = 0;
		laser[i].live = false;
		laser[i].width = 35;
	}
	//敌机导弹初始化
	missile.x = 0;
	missile.y = 0;
	missile.live = false;
	missile.width = 56;
	missile.height = 252;
	missile.damage = false;
	//黑洞初始化
	for (int i = 0; i < HOLE_NUM; i++)
	{
		hole[i].live = false;
		hole[i].x = 0;
		hole[i].y = 0;
		hole[i].damage = false;  //此处用于标记是否生成完成
	}
	//血包初始化
	hp.live = false;
	hp.x = 0;
	hp.y = 0;
	hp.Xspeed = 0;
	hp.Yspeed = 0;
	hp.width = 60;
	hp.height = 60;
	//能量包初始化
	energybag.live = false;
	energybag.x = 0;
	energybag.y = 0;
	energybag.Xspeed = 0;
	energybag.Yspeed = 0;
	energybag.width = 60;
	energybag.height = 60;
}
void gamedraw(int air_choice)
{
	BeginBatchDraw();
	cleardevice();

	bkRoll(defense.live);

	if (hp.live == true)
		transparentimage3(NULL, hp.x, hp.y, &hp_image);
	if (energybag.live == true)
		transparentimage3(NULL, energybag.x, energybag.y, &energybag_image);

	outText();

	energyDraw();
	holeCreate();
	if (player.live==true)
	    transparentimage3(NULL, player.x, player.y, &player_image[air_choice==0 ? 0:1]);

	if (defense.live == true)
		transparentimage3(NULL, defense.x, defense.y, &defense_image);

	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].live == true)
		{
			transparentimage3(NULL, bullet[i].x, bullet[i].y, &bullet_image[0]);
		}
		if (e_bullet[i].live == true)
		{
			transparentimage3(NULL, e_bullet[i].x, e_bullet[i].y, &bullet_image[1]);
		}
	}

	for (int i = 0; i < ENEMY1_NUM; i++)
	{
		if (enemy1[i].live == true)
		{
			transparentimage3(NULL, enemy1[i].x, enemy1[i].y, &enemy_image[0]);
		}
	}
	for (int i = 0; i < ENEMY2_NUM; i++)
	{
		if (enemy2[i].live == true)
		{
			transparentimage3(NULL, enemy2[i].x, enemy2[i].y, &enemy_image[1]);
		}
	}
	for (int i = 0; i < ENEMY4_NUM; i++)
	{
		if (enemy4[i].live == true)
		{
			transparentimage3(NULL, enemy4[i].x, enemy4[i].y, &enemy_image[3]);
		}
	}

	if (enemy3.live == true)
	{
		transparentimage3(NULL, enemy3.x, enemy3.y, &enemy_image[2]);
	}
	for (int id = 0; id < LASER_NUM; id++)		//激光动画
	{
		if (laser[id].live == true)
		{
			if (laser_timer[id] >= 0 && laser_timer[id] < 400)
			{
				transparentimage3(NULL, enemy4[id].x + enemy4[id].width / 2 - 12, enemy4[id].y + enemy4[id].height, &laser_image[0]);
				laser_timer[id]++;
			}
			else if (laser_timer[id] >= 400 && laser_timer[id] < 550)
			{
				transparentimage3(NULL, enemy4[id].x + enemy4[id].width / 2 - 32, enemy4[id].y + enemy4[id].height, &laser_image[1]);
				laser_timer[id]++;
			}
			else if (laser_timer[id] >= 550 && laser_timer[id] < 1000)
			{
				transparentimage3(NULL, enemy4[id].x + enemy4[id].width / 2 - 50, enemy4[id].y + enemy4[id].height, &laser_image[2]);
				laser_timer[id]++;
			}
			else if (laser_timer[id] >= 1000 && laser_timer[id] <= 1150)
			{
				transparentimage3(NULL, enemy4[id].x + enemy4[id].width / 2 - 32, enemy4[id].y + enemy4[id].height, &laser_image[1]);
				laser_timer[id]++;
			}
		}
	}
	if (missile.live == true)
	{
		transparentimage3(NULL, missile.x, missile.y, &missile_image);
	}

	enemy1_damage();
	enemy2_damage();
	enemy4_damage();
	enemy3_damage();
	player_damage();
	missile_damage();
	FlushBatchDraw();
}
void gameInit()
{
	srand(time(NULL));
	initgraph(BKWIDTH, BKHEIGHT);
	playInit();
	loadimg();
	setbkmode(0);
	settextcolor(BLACK);
	setlinecolor(BLACK);
}
void game(int air_choice,int difficulty)
{

	playerMove(air_choice==0 ? 3:4);
	shoot(air_choice==0 ? 100:30);
	bulletMove(air_choice==0 ? 6:9);
	enemy1Create(difficulty==0 ? 100 - difficulty_num*2 :80 - difficulty_num*3);
	enemy1Move(difficulty == 0 ? 2 : 3);
	enemy2Create(difficulty == 0 ? 100 - difficulty_num*2 : 50 - difficulty_num*3);
	enemy2Move(1, 250);
	enemy4Create(difficulty == 0 ? 50-difficulty_num  : 30-difficulty_num );
	enemy4Move(2,100);
	enemy3Create(difficulty == 0 ? 800-difficulty_num*10 : 400-difficulty_num*15);
	enemy3Move(3, 100);
	e_bulletMove(difficulty == 0 ? 3 : 4);
	laserMove();
	missileMove(2);
	holeMove();
	hit(air_choice==0 ? 50:100);
	defenseCreate(MAX);
	defenseMove();
	itemCreate(20000,8000);
	itemMove();
	itemTouch();
	difficultyCal();
	gamedraw(air_choice);
	if (player_draw > 400)
	{
		MessageBox(NULL, "游戏结束", "GAME OVER", MB_SYSTEMMODAL);
	//	exit(0);
		end = true;
		return;
	}
	if (defense.live == false)
		distance++;
	else
		distance += 2;
}
