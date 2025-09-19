#pragma once
#ifndef _FIGHT_H
#define _FIGHT_H
void playerMove(int speed);		
void shoot(int gap);		//玩家射击
void bulletMove(int speed);
void enemy1Create(int gap);
void enemy1Move(int speed);
void enemy2Create(int gap);
void enemy2Move(int speed, int gap);
void e_bulletMove(int speed);		//敌人子弹移动
void hit(int harm);
void enemy4Create(int gap);
void enemy4Move(int speed,int gap);
void enemy3Create(int gap);
void enemy3Move(int speed, int gap);
void laserMove();		//激光控制
void missileMove(int speed);//飞弹控制
void defenseCreate(int max);
void defenseMove();
void holeMove();
void itemTouch();
void itemCreate(int hp_gap,int energybag_gap);
void itemMove();
void difficultyCal();
extern int enemy1_num;		//已生成的enemy1数量
extern unsigned int score;
extern unsigned int distance;
extern int difficulty_num;
extern int energy;
extern int laser_timer[ENEMY4_NUM];	//激光发射间隔控制器
#endif