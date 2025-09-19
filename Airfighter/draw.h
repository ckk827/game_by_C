#pragma once
#ifndef _SOURCE_H
#define _SOURCE_H
#define BKWIDTH 1600
#define BKHEIGHT 900
#define ENEMY1_NUM 30 //������Ļ�������15��
#define ENEMY2_NUM 3
#define ENEMY4_NUM 3
#define ENEMY3_NUM 1
#define BULLET_NUM 30
#define LASER_NUM 3
#define MAX 8400
#define HOLE_NUM 2
//��Ļ�������30���ӵ�

struct plane		//ʵ��Ľṹ��
{
	int x;
	int y;
	bool live;
	int width;
	int height;
	bool damage;		//�Ƿ񲥷Ŷ���
	int hp;
};

struct item		//���ߵĽṹ��
{
	int Xspeed;
	int Yspeed;
	int x;
	int y;
	int width;
	int height;
	bool live;
};
extern plane hole[];
extern plane player;
extern plane enemy1[];
extern plane enemy2[];
extern plane enemy4[];
extern plane enemy3;
extern plane bullet[];
extern plane e_bullet[];
extern plane laser[];
extern plane missile;
extern plane defense;

extern item hp;
extern item energybag;

extern bool end;		//��Ϸ�����ж�
extern int record;

void gameInit();		//��Ϸ��ʼ��
void game(int air_choice,int difficulty);		//��Ϸ��ʼ

bool timer(int ms, int id);
#endif