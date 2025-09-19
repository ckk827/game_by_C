#undef UNICODE
#undef _UNICODE

#include "draw.h"
#include <graphics.h>
IMAGE title;
IMAGE air_title;
IMAGE game_start;
IMAGE game_exit;
IMAGE bk2;
IMAGE bk3;
IMAGE air1;
IMAGE air2;
IMAGE skull;
IMAGE eazy;
IMAGE hard;
IMAGE difficulty_title;
IMAGE pause;

void mainMenuLoad()
{
	loadimage(&title, "title.png");
	loadimage(&game_start, "gamebegin.png");
	loadimage(&game_exit, "gameover.png");
	loadimage(&bk2, "background0.png");
}
void airMenuLoad()
{
	loadimage(&air1, "me2.png");
	loadimage(&air2, "j20_menu.png");
	loadimage(&air_title, "airtitle.png");
	loadimage(&bk3, "background1.png");
}
void difficultyMenuLoad()
{
	loadimage(&eazy, "easy.png");
	loadimage(&hard, "hard.png");
	loadimage(&skull, "skull.jpg");
	loadimage(&difficulty_title, "difficultytitle.png");
}
void pauseMenuLoad()
{
	loadimage(&pause, "resume_pressed.png");
}
void mainMenu()
{
	while (1)
	{
		BeginBatchDraw();
		cleardevice();
		putimage(0,0,&bk2);
		putimage(450, 100, &title);
		putimage(500, 400, &game_start);
		putimage(500, 600, &game_exit);
		EndBatchDraw();
		MOUSEMSG msg;
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			if (500 < msg.x&&msg.x < 500 + 607 && 400 < msg.y && msg.y< 400 + 80)
				break;
			else if (500 < msg.x && msg.x< 500 + 607 && 600 < msg.y && msg.y< 600 + 80)
				exit(0);
		}
	}
}
int airMenu()
{
	setlinecolor(BLACK);
	while (1)
	{
		BeginBatchDraw();
		cleardevice();
		putimage(0, 0, &bk3);
		putimage(200, 280, &air1);		//飞机1
		putimage(1000, 280, &air2);		//飞机2
		putimage(450, 0, &air_title);
		MOUSEMSG msg;
		msg = GetMouseMsg();
		if (200 < msg.x && msg.x < 200 + 500 && 280 < msg.y && msg.y < 280 + 618)		//黑框显示
		{
			rectangle(200, 280, 200 + 500, 280 + 618);
		}
		if (900 < msg.x && msg.x < 900 + 500 && 280 < msg.y && msg.y < 280 + 618)
		{
			rectangle(900, 280, 900 + 500, 280 + 618);
		}
		EndBatchDraw();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			if (200 < msg.x && msg.x < 200 + 500 && 280 < msg.y && msg.y < 280 + 618)
				return 0;
			else if (900 < msg.x && msg.x < 900 + 500 && 280 < msg.y && msg.y < 280 + 618)
				return 1;
		}
	}
	

}
int difficultyMenu()		
{
	while (1)
	{
		BeginBatchDraw();
		cleardevice();
		putimage(0, 0, &bk2);
		putimage(450, 100, &difficulty_title);
		putimage(500, 400, &eazy);
		putimage(500, 600, &hard);
		putimage(400, 600, &skull);
		outtextxy(530, 700, " 更高的敌人生成速度，更多的精英敌人,更快的敌人移动速度和子弹飞行速度");
		EndBatchDraw();
		MOUSEMSG msg;
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			if (500 < msg.x && msg.x < 500 + 607 && 400 < msg.y && msg.y < 400 + 80)
				return 0;		//eazy
			else if (500 < msg.x && msg.x < 500 + 607 && 600 < msg.y && msg.y < 600 + 80)
				return 1;		//hard
		}
	}
}
void pauseMenu()
{
	BeginBatchDraw();
	putimage(650, 260, &pause);
	EndBatchDraw();
}