#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include "draw.h"
#include "menu.h"
#include "fight.h"
#include<windows.h>
#include<mmsystem.h>
#include "stdio.h"
#include "conio.h"
#pragma comment(lib,"Winmm.lib")
void updateData(int recordScore) 
{
//	printf("1\n");
	const char* filename = "record.txt";
	FILE* file = fopen(filename, "r+");

	if (file == NULL) 
	{
		//printf("2\n");
		file = fopen(filename, "w+");
		if (file == NULL) 
		{
	//		printf("无法打开文件\n");
			return;
		}
		fprintf(file, "%d", recordScore);
	//	printf("写入新记录: %d\n", recordScore);
	}
	else 
	{
		int currentData;
		fscanf(file, "%d", &currentData);  
		printf("当前记录为: %d\n", currentData);
		if (currentData < recordScore) 
		{
			//printf("3\n");
			rewind(file);  
			fprintf(file, "%d", recordScore);  
		}
	}
	fclose(file); 
}
int readData() 
{
	const char* filename = "record.txt";
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		return -1;
	}
	else
	{
		int currentData;
		if (fscanf(file, "%d", &currentData) !=  0)
		{
			return currentData;
		}
		else
		{
			return -1;
		}
		fclose(file);
	}
}
bool pause()
{
   	static int isPaused = 0;
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		isPaused = 1;
		Sleep(100);
	}
	if (isPaused==1)
	{
		pauseMenu();
		while (isPaused==1)
		{
			pauseMenu();
			if (GetAsyncKeyState(VK_ESCAPE))
			{
				isPaused = 0;
				Sleep(100);
			}
		}

	}
	return 0;
}
int main()
{
	while (1)
	{
		record = readData();
		gameInit();
		mainMenuLoad();
		airMenuLoad();
		difficultyMenuLoad();
		pauseMenuLoad();
		mainMenu();
		const int air_choice = airMenu();
		const int difficulty = difficultyMenu();
		PlaySound(TEXT("BKmusic.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		while (1)
		{
			if (timer(2, 12))
			{
				if (pause())
				{
					break;
				}
				game(air_choice, difficulty);
				if (end)
				{
					break;
				}
			}
		}
		updateData(score);
		break;
	}
}