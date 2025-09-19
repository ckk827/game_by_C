#pragma once
#ifndef _FIGHT_H
#define _FIGHT_H
void playerMove(int speed);		
void shoot(int gap);		//������
void bulletMove(int speed);
void enemy1Create(int gap);
void enemy1Move(int speed);
void enemy2Create(int gap);
void enemy2Move(int speed, int gap);
void e_bulletMove(int speed);		//�����ӵ��ƶ�
void hit(int harm);
void enemy4Create(int gap);
void enemy4Move(int speed,int gap);
void enemy3Create(int gap);
void enemy3Move(int speed, int gap);
void laserMove();		//�������
void missileMove(int speed);//�ɵ�����
void defenseCreate(int max);
void defenseMove();
void holeMove();
void itemTouch();
void itemCreate(int hp_gap,int energybag_gap);
void itemMove();
void difficultyCal();
extern int enemy1_num;		//�����ɵ�enemy1����
extern unsigned int score;
extern unsigned int distance;
extern int difficulty_num;
extern int energy;
extern int laser_timer[ENEMY4_NUM];	//���ⷢ����������
#endif