#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "draw.h"
//移动函数
unsigned int score = 0;     //游戏分数
unsigned int distance = 0;		//飞行距离
int difficulty_num = 0;		//难度系数
int enemy1_num = 0;     //enemy1生成总数计数
int enemy2_count = 0;        //防止enemy2一次生成多个计数
int enemy4_count = 0;
int enemy4_targetX[ENEMY4_NUM]={ 0 };    //enemy4的目标位置数组
int laser_timer[ENEMY4_NUM] = { 0 }; //enemy4发射激光计数数组
int laser_attack_count[LASER_NUM] = { 0 }; //激光撞击玩家计数数组，防止重复伤害
int energy = 0;		//护盾能量计数



void playerMove(int speed)
{
	if (GetAsyncKeyState('W')&&!GetAsyncKeyState('D') && !GetAsyncKeyState('A'))
	{
		if (player.y <= 0)
		{
			player.y += speed;
		}
		else
		{
			player.y -= speed;
		}
	}
	else if(GetAsyncKeyState('S') && !GetAsyncKeyState('A') && !GetAsyncKeyState('D'))
	{
		if (player.y >= BKHEIGHT-player.height)
		{
			player.y -= speed;
		}
		else
		{
			player.y += speed;
		}
	}
	else if (GetAsyncKeyState('A') && !GetAsyncKeyState('W') && !GetAsyncKeyState('S'))
	{
		if (player.x <= 0)
		{
			player.x += speed;
		}
		else
		{
			player.x -= speed;
		}
	}
	else if (GetAsyncKeyState('D')&&!GetAsyncKeyState('W') && !GetAsyncKeyState('S'))
	{
		if (player.x >= BKWIDTH-player.width)
		{
			player.x -= speed;
		}
		else
		{
			player.x += speed;
		}
	}
	//斜向移动
	else if (GetAsyncKeyState('A')&& GetAsyncKeyState('W'))
	{
		if (player.x <= 0)
		{
			player.x += speed;
		}
		else if (player.y <= 0)
		{
			player.y += speed;
		}
		else
		{
			player.x -= speed;
			player.y -= speed;
		}
	}
	else if (GetAsyncKeyState('D') && GetAsyncKeyState('W'))
	{
		if (player.x >= BKWIDTH - player.width)
		{
			player.x -= speed;
		}
		else if (player.y <= 0)
		{
			player.y += speed;
		}
		else
		{
			player.x += speed;
			player.y -= speed;
		}
	}
	else if (GetAsyncKeyState('D') && GetAsyncKeyState('S'))
	{
		if (player.x >= BKWIDTH - player.width)
		{
			player.x -= speed;
		}
		else if (player.y >= BKHEIGHT - player.height)
		{
			player.y -= speed;
		}
		else
		{
			player.x += speed;
			player.y += speed;
		}
	}
	else if (GetAsyncKeyState('A') && GetAsyncKeyState('S'))
	{
		if (player.x <= 0)
		{
			player.x += speed;
		}
		else if (player.y >= BKHEIGHT - player.height)
		{
			player.y -= speed;
		}
		else
		{
			player.x -= speed;
			player.y += speed;
		}
	}
}
void shoot(int gap)
{
	if (GetAsyncKeyState(VK_SPACE)&&timer(gap,0))
	{
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (bullet[i].live == false)
			{
				bullet[i].x = player.x + player.width / 2;
				bullet[i].y = player.y;
				bullet[i].live = true;
				break;
			}
		}
	}
}
void bulletMove(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].live == true)
		{
			bullet[i].y -= speed;
		}
		if (bullet[i].y <= 0)
		{
			bullet[i].live = false;
		}
	}
}
void enemy1Create(int gap)
{
	if (timer(gap, 1)==true)
	{
		for (int i = 0; i < ENEMY1_NUM; i++)
		{
			if (enemy1[i].live == false&&enemy1[i].damage==false)
			{
				enemy1[i].hp = 10;
				enemy1[i].x = rand() % (BKWIDTH - enemy1[i].width);
				enemy1[i].y = -100;
				enemy1[i].live = true;
				enemy1[i].damage = false;
				enemy1_num++;
				break;
			}
		}
	}
}
void enemy1Move(int speed)
{
	for (int i = 0; i < ENEMY1_NUM; i++)
	{
		if (enemy1[i].live == true)
		{
			enemy1[i].y += speed;
		}
		if (enemy1[i].y >= BKHEIGHT)
		{
			enemy1[i].live = false;
		}
	}
}
void hit(int harm)
{
	//子弹撞击敌人1
	for (int i = 0; i < ENEMY1_NUM; i++)
	{
		if (enemy1[i].live==false)
			continue;
		for (int j = 0; j < BULLET_NUM; j++)
		{
			if (bullet[j].live==false)
				continue;
			if (bullet[j].x > enemy1[i].x &&
				bullet[j].x < enemy1[i].x + enemy1[i].width &&
				bullet[j].y > enemy1[i].y &&
				bullet[j].y < enemy1[i].y + enemy1[i].height)
			{
				bullet[j].live = false;
				enemy1[i].hp-=harm;
			}
		}
		if (enemy1[i].hp <= 0)
		{
			enemy1[i].live = false;
			score += 10;
			enemy1[i].damage = true;
		}
	}
	//子弹撞击敌人2
	for (int i = 0; i < ENEMY2_NUM; i++)
	{
		if (enemy2[i].live == false)
			continue;
		for (int j = 0; j < BULLET_NUM; j++)
		{
			if (bullet[j].live == false)
				continue;
			if (bullet[j].x > enemy2[i].x &&
				bullet[j].x < enemy2[i].x + enemy2[i].width &&
				bullet[j].y > enemy2[i].y &&
				bullet[j].y < enemy2[i].y + enemy2[i].height)
			{
				bullet[j].live = false;
				enemy2[i].hp -= harm;
			}
		}
		if (enemy2[i].hp <= 0)
		{
			enemy2[i].live = false;
			score += 50;
			enemy2[i].damage = true;
		}
	}
	//子弹撞击敌人3
	if (enemy3.live == true)
	{
		for (int j = 0; j < BULLET_NUM; j++)
		{
			if (bullet[j].live == false)
				continue;
			if (bullet[j].x > enemy3.x &&
				bullet[j].x < enemy3.x + enemy3.width &&
				bullet[j].y > enemy3.y &&
				bullet[j].y < enemy3.y + enemy3.height)
			{
				bullet[j].live = false;
				enemy3.hp -= harm;
			}
		}
		if (enemy3.hp <= 0)
		{
			enemy3.live = false;
			score += 200;
			enemy3.damage = true;
		}
	}
	//子弹撞击敌人4
	for (int i = 0; i < ENEMY4_NUM; i++)
	{
		if (enemy4[i].live == false)
			continue;
		for (int j = 0; j < BULLET_NUM; j++)
		{
			if (bullet[j].live == false)
				continue;
			if (bullet[j].x > enemy4[i].x &&
				bullet[j].x < enemy4[i].x + enemy4[i].width &&
				bullet[j].y > enemy4[i].y &&
				bullet[j].y < enemy4[i].y + enemy4[i].height)
			{
				bullet[j].live = false;
				enemy4[i].hp -= harm;
			}
		}
		if (enemy4[i].hp <= 0)
		{
			enemy4[i].live = false;
			score += 50;
			enemy4[i].damage = true;
		}
	}
	//护盾受击
	if (defense.live == true)
	{
		//敌人1撞击护盾
		for (int i = 0; i < ENEMY1_NUM; i++)
		{
			if (defense.x + defense.width >= enemy1[i].x &&
				defense.x <= enemy1[i].x + enemy1[i].width &&
				defense.y <= enemy1[i].y + enemy1[i].height &&
				defense.y + defense.height >= enemy1[i].y &&
				enemy1[i].live == true)
			{
				enemy1[i].live = false;
				enemy1[i].damage = true;
				energy -= 200;
			}
		}
		//子弹撞击护盾
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (e_bullet[i].live == false)
				continue;
			else
			{
				if (e_bullet[i].x >= defense.x &&
					e_bullet[i].x < defense.x + defense.width &&
					e_bullet[i].y >= defense.y &&
					e_bullet[i].y < defense.y + defense.height &&
					e_bullet[i].live == true)
				{
					e_bullet[i].live = false;
					energy -= 100;
				}
			}
		}
		//激光撞击护盾
		for (int i = 0; i < LASER_NUM; i++)
		{
			if (laser[i].live == true && laser_timer[i] > 400 && laser_timer[i] < 1150)
			{
				if (defense.x + defense.width - laser[i].x > 0 &&
					defense.x - laser[i].x - laser[i].width < 0 &&
					laser_attack_count[i] == 0 &&
					laser[i].live == true)
				{
					energy -= 2000;
					laser_attack_count[i]++;
				}
			}
		}
		//导弹撞击护盾
		if (missile.live == true)
		{
			if (missile.x<defense.x + defense.width &&
				missile.x + missile.width>defense.x &&
				missile.y + missile.height > defense.y &&
				missile.y + 60 < defense.y + defense.height)
			{
				missile.live = false;
				energy -= 1000;
				missile.damage = true;
			}
		}
		if (energy <= 0)
		{
			defense.live = false;
		}
	}

	//敌人1撞击玩家
	for (int i = 0; i < ENEMY1_NUM; i++)
	{
		if (player.x + player.width >= enemy1[i].x &&
			player.x <= enemy1[i].x + enemy1[i].width &&
			player.y <= enemy1[i].y + enemy1[i].height &&
			player.y + player.height >= enemy1[i].y&&
			enemy1[i].live==true)
		{
			enemy1[i].live = false;
			enemy1[i].damage = true;
			player.hp-=20;
		}
	}
	//子弹撞击玩家
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (e_bullet[i].live == false)
			continue;
		else
		{
			if (e_bullet[i].x >= player.x &&
				e_bullet[i].x < player.x + player.width &&
				e_bullet[i].y >= player.y &&
				e_bullet[i].y < player.y + player.height&&
				e_bullet[i].live==true)
			{
				e_bullet[i].live = false;
				player.hp -= 20;
			}
		}
	}
	//激光撞击玩家
	for (int i = 0; i < LASER_NUM; i++)
	{
		if (laser[i].live == true&&laser_timer[i]>400&&laser_timer[i]<1150)
		{
			if (player.x+player.width - laser[i].x>0 &&
				player.x - laser[i].x - laser[i].width<0&&
				laser_attack_count[i] == 0&&
				laser[i].live==true)
			{
				player.hp -= 200;
				laser_attack_count[i]++;
			}
		}
	}
	//导弹撞击玩家
	if (missile.live == true)
	{
		if (missile.x<player.x + player.width &&
			missile.x + missile.width>player.x &&
			missile.y + missile.height > player.y &&
			missile.y + 60 < player.y + player.height)
		{
			missile.live = false;
			player.hp -= 150;
			missile.damage = true;
		}
	}
	//黑洞攻击玩家
	for (int i = 0; i < HOLE_NUM; i++)
	{
		if (hole[i].live == true&&hole[i].damage==true)
		{
			if (player.x + player.width / 2 - hole[i].x<10 && player.y + player.height / 2 - hole[i].y<10&& player.x + player.width / 2 - hole[i].x > -10 && player.y + player.height / 2 - hole[i].y > -10)
				player.hp -= 9999;
		}
	}
	//检查护盾死亡
	 if (energy<=0)
	{
		defense.live = false;
	}
	//检查玩家死亡
	if (player.hp<=0)
	{
		player.live = false;
	}
}
void enemy2Create(int gap) //此gap为enemy1的数量
{
	if (enemy1_num % gap == 0&&enemy1_num!=0)
	{
		if (enemy2_count == 0)
		{
			for (int i = 0; i < ENEMY2_NUM; i++)
			{
				if (enemy2[i].live == false&&enemy2[i].damage==false)
				{
					enemy2[i].x = -400;
					enemy2[i].y = 50;
					enemy2[i].hp = 1000;
					enemy2[i].live = true;
					enemy2[i].damage = false;
					enemy2_count++;
					break;
				}
			}
		}
	}
	else
	{
		enemy2_count = 0;
	}
}
void e_bulletMove(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (e_bullet[i].live == true)
		{
			e_bullet[i].y += speed;

		}
		if (e_bullet[i].y >= BKHEIGHT)
		{
			e_bullet[i].live = false;
		}
	}
}
void enemy2Move(int speed,int gap) //enemy2的行为逻辑
{
	for (int i = 0; i < ENEMY2_NUM; i++)
	{
		if (enemy2[i].live == true)
		{
			if (enemy2[i].x <= player.x)
			{
				enemy2[i].x += speed;
			}
			else
			{
				enemy2[i].x -= speed;
			}
			if (timer(gap, i+2) == true)
			{
				for (int j = 0; j < BULLET_NUM; j++)
				{
					if (e_bullet[j].live == false)
					{
						e_bullet[j].x = enemy2[i].x + enemy2[i].width / 2;
						e_bullet[j].y = enemy2[i].y + enemy2[i].height;
						e_bullet[j].live = true;
						break;
					}
				}
			}

		}
	}
}
void enemy3Create(int gap)
{
	{
		if (enemy1_num % gap == 0 && enemy1_num != 0)
		{
			if (enemy3.live == false && enemy3.damage == false)
			{
				enemy3.x = BKWIDTH/2-enemy3.width/2;
				enemy3.y = -400;
				enemy3.hp = 40000;
				enemy3.live = true;
				enemy3.damage = false;
			}
		}

	}
}
void enemy3Move(int speed, int gap)
{
	if (enemy3.live == true)
	{
		if (enemy3.y < 50)		//入场
		{
			enemy3.y += speed;
		}


		else
		{
			if (timer(20, 10) == true)		//enemy3移动逻辑，一端面对玩家
			{
				if (enemy3.x < player.x + 60)
				{
					enemy3.x += speed;
				}
				else if (enemy3.x > player.x + 60)
				{
					enemy3.x -= speed;
				}
			}
			

			if (timer(200, 11) == true)		//发射子弹
			{
				for (int j = 0; j < BULLET_NUM; j++)
				{
					if (e_bullet[j].live == false)
					{
						e_bullet[j].x = enemy3.x - 50;
						e_bullet[j].y = enemy3.y + enemy3.height;
						e_bullet[j].live = true;
						for (int i = j; i < BULLET_NUM; i++)
						{
							if (e_bullet[i].live == false)
							{
								e_bullet[i].x = enemy3.x + enemy3.width + 50;
								e_bullet[i].y = enemy3.y + enemy3.height;
								e_bullet[i].live = true;
								break;

							}
						}
						break;
					}
				}
			}
			if (timer(1000, 13) == true)		//发射飞弹
			{
				if (missile.live == false)
				{
					missile.x = enemy3.x + enemy3.width / 2;
					missile.y = enemy3.y+60;
					missile.live = true;
				}
			}
			if (timer(15000, 16) == true) //生成黑洞

			{
				for (int i = 0; i < HOLE_NUM; i++)
				{
					if (hole[i].live == false)
					{
						hole[i].live = true;
						hole[i].hp = 7000;
						hole[i].x = rand() % 1500 + 50;
						hole[i].y = 750;
						break;
					}
				}
			}
		}
	}
}
void enemy4Create(int gap) //此gap为enemy1的数量
{
	if (enemy1_num % gap == 0 && enemy1_num != 0)
	{
		if (enemy4_count == 0)
		{
			for (int i = 0; i < ENEMY4_NUM; i++)
			{
				if (enemy4[i].live == false && enemy4[i].damage == false)
				{
					enemy4[i].x = BKWIDTH+400;
					enemy4[i].y = 50;
					enemy4[i].hp = 5000;
					enemy4[i].live = true;
					enemy4[i].damage = false;
					enemy4_count++;
					break;
				}
			}
		}
	}
	else
	{
		enemy4_count = 0;
	}
}
void enemy4Move(int speed,int gap)   //enemy4的行为逻辑
{
	for (int i = 0; i < ENEMY4_NUM; i++)   
	{ 
		if (enemy4[i].live == true)
		{
			if (timer(8000, i + 6))    //随机移动
			{
				enemy4_targetX[i] = rand() % (BKWIDTH - enemy4[i].width);
			}

			if (timer(10000, i + 6 + ENEMY4_NUM))   //发射一轮激光
			{
				laser[i].x = enemy4[i].x;
				laser[i].y = enemy4[i].y;
				laser[i].live = true;
			}

			if (enemy4[i].x < enemy4_targetX[i])		//移动逻辑
			{
				enemy4[i].x += speed;
			}
			else if (enemy4[i].x > enemy4_targetX[i])
			{
				enemy4[i].x -= speed;
			}
		}
	}
}
void laserMove()		//激光逻辑
{
	for (int i = 0; i < LASER_NUM; i++)
	{
		if (laser[i].live == true)
		{
			laser[i].x = enemy4[i].x;
			laser_timer[i]++;
			if (laser_timer[i] > 1150)
			{
				laser[i].live = false;
				laser_timer[i] = 0;
				laser_attack_count[i] = 0;
			}
		}
		
	}
}
void missileMove(int speed)
{
	if (missile.live == true)
	{
		missile.y += speed;
	}
	if (missile.y > BKWIDTH)
	{
		missile.live = false;
	}
}
void defenseCreate(int max)
{
	if (GetAsyncKeyState('B')&&energy >= max)
	{
		defense.live = true;
	}
	if (defense.live == false&&energy<max)
	{
		energy++;
	}
	if (defense.live == true)
	{
		energy--;
	}
	if (energy > max)
	{
		energy = max;
	}
	if (energy < 0) 
	{
		energy = 0;
	}
}
void defenseMove()
{
	if (defense.live == true)
	{
		defense.x = player.x - 15;
		defense.y = player.y - 15;
	}
}
void itemCreate(int hp_gap,int energybag_gap)
{
	if (timer(hp_gap, 14)&&hp.live==false)
	{
		hp.live = true;
		hp.x = rand() % (1600 - hp.width);
		hp.y = rand() % (900 - hp.height);
		hp.Xspeed = rand() % 5 - 2;
		hp.Yspeed = rand() % 5 - 2;
	}
	if (timer(energybag_gap, 15) && energybag.live == false)
	{
		energybag.live = true;
	}
}
void itemMove()
{
	//血包的移动逻辑
	if (hp.live == true)
	{
		hp.x += hp.Xspeed;
		hp.y += hp.Yspeed;
		if (hp.x <= 0 || hp.x + hp.width >= BKWIDTH)
		{
			hp.Xspeed = 0 - hp.Xspeed;
		}
		if (hp.y <= 0 || hp.y + hp.height >= BKHEIGHT)
		{
			hp.Yspeed = 0 - hp.Yspeed;
		}
	}
	//能量球的逻辑
	static int carrier_index;
	static bool target_get = false;
	static bool target_die = false;
	if (target_get == false&&energybag.live==true)
	{
		for (int i = 0; i < ENEMY1_NUM; i++)
		{
			if (enemy1[i].y < 0 && enemy1[i].live == true)
			{
				carrier_index = i;
				target_get = true;
				break;
			}
		}
	}
	if (target_get == true && energybag.live == true)
	{
		if (enemy1[carrier_index].live == false)
		{
			target_die = true;
		}
		if (target_die == false&&target_get==true)
		{
			energybag.x = enemy1[carrier_index].x;
			energybag.y = enemy1[carrier_index].y + enemy1[carrier_index].width-30;
		}
	}
	if (energybag.live == false)
	{
		target_get = false;
		target_die = false;
	}
}
void itemTouch()
{
	if (hp.live == true)
	{
		if (hp.x<player.x + player.width &&
			hp.x + hp.width>player.x &&
			hp.y + hp.height > player.y &&
			hp.y < player.y + player.height)
		{
			player.hp += 200;
			hp.live = false;
		}
	}

	if (energybag.y + energybag.height >= BKHEIGHT && energybag.live == true)
		energybag.live = false;
	if (energybag.live == true)
	{
		if (energybag.x<player.x + player.width &&
			energybag.x + energybag.width>player.x &&
			energybag.y + energybag.height > player.y &&
			energybag.y < player.y + player.height)
		{
			energy += 2000;
			energybag.live = false;
		}
	}
}
void holeMove()
{
	for (int i = 0; i < HOLE_NUM; i++)
	{
		if (hole[i].live == true&&hole[i].damage==true)
		{
			int R = (player.x - hole[i].x) * (player.x - hole[i].x) + (player.y - hole[i].y) * (player.y - hole[i].y);
			if (R < 400 * 400&&R>=200 * 200)
			{
				if (player.x+player.width /2< hole[i].x)
					player.x++;
				if (player.x + player.width /2> hole[i].x)
					player.x--;
				if (player.y +player.height/2> hole[i].y)
					player.y--;
				if (player.y + player.height /2< hole[i].y)
					player.y++;
			}
			if (R < 200 * 200 && R>=50 * 50)
			{
				if (player.x + player.width /2< hole[i].x)
					player.x+=2;
				if (player.x + player.width /2> hole[i].x)
					player.x-=2;
				if (player.y + player.height /2> hole[i].y)
					player.y-=2;
				if (player.y + player.height /2< hole[i].y)
					player.y+=2;
			}
			if (R<50 * 50&&R>=25*25)
			{
				if (player.x + player.width / 2 < hole[i].x)
					player.x += 3;
				if (player.x + player.width / 2 > hole[i].x)
					player.x -= 3;
				if (player.y + player.height /2> hole[i].y)
					player.y -= 3;
				if (player.y + player.height /2< hole[i].y)
					player.y += 3;
			}
			if (R < 25*25)
			{
				if (player.x + player.width / 2 < hole[i].x)
					player.x += 5;
				if (player.x + player.width / 2 > hole[i].x)
					player.x -= 5;
				if (player.y + player.height / 2 > hole[i].y)
					player.y -= 5;
				if (player.y + player.height / 2 < hole[i].y)
					player.y += 5;
			}
			hole[i].hp--;
			if (hole[i].hp <= 0)
				hole[i].live = false;
		}
	}
}
void difficultyCal()
{
	difficulty_num = distance / 5000;
	if (difficulty_num > 20)
		difficulty_num = 20;
}