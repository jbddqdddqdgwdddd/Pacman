//#include "StdAfx.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <time.h>
#include <math.h>
#pragma warning(disable: 4996)

//枚举命令 方向 吃豆人的张嘴 怪物的触须
enum CMD { CMD_QUIT = 1, CMD_UP = 2, CMD_DOWN = 4, CMD_LEFT = 8, CMD_RIGHT = 16, CMD_MARKRED = 32, CMD_MARKGREEN = 64, CMD_MARKYELLOW = 128, CMD_CLEARMARK = 256 };
enum direc { up = 0, right = 1, down = 2, left = 3 };
enum mouth { shut = 0, open = 1 , openb = 2, openbb = 3};
enum stat { Exist = 1, Eaten = 0};

struct coord {
	int x, y;
};
struct Pacman
{
	coord pos;
	int direc;
	IMAGE imga[4][4];
	mouth amouth;
	coord spos;
};
struct Pac
{
	//coord pos;
	IMAGE imga;
	int stat;
};
struct Wall {
	IMAGE imga[6];
	int width, height;
	coord pos;
};
struct Monster {
	coord pos;
	int direc;
	IMAGE imga[2][4];
	coord spos;
	int stat;
	
};
/****************************************************/
// 函数声明
/****************************************************/

void	OnUp(Pacman* a);								// 向上移动
void	OnLeft(Pacman* a);							// 向左移动
void	OnRight(Pacman* a);							// 向右移动
void	OnDown(Pacman* a);							// 向下移动
int		GetCmd();							// 获取用户输入的命令
void	DispatchCmd(Pacman* a, int cmd, int map[21][19]);				// 处理用户输入的命令
void drawPacman(Pacman* a);//画吃豆人
void movePacman(Pacman* a, direc pdirec);//移动吃豆人
void drawMap(int map[21][19], Wall* wall_1, Pac* pac);//画地图
void checkPac(int map[21][19], Pacman* a, Pac* pac);//判断豆子是否被吃
void openMouth(Pacman* a);//改变吃豆人张嘴的幅度
void checkEdge(Pacman* a);//判断吃豆人撞墙
int checkClear(int map[21][19], Monster* mon1, Monster* mon2, Monster* mon3, Monster* mon4, Pacman* a);//判断游戏进行状态
void moveMon(Monster* mon, int map[21][19], int direc);//移动怪物
void checkMonsterWall(int map[21][19], Pacman* a, Monster* mon);//判断怪物撞墙
void drawMonster(Monster mon);//画怪物
void monsterWalk(Monster* mon);//怪物触须变换
//////////////////////////////////////////////////////

int map[21][19] = {
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,7,7,7,7,7,7,7,7,1,7,7,7,7,7,7,7,7,2},
	{2,7,1,7,7,1,7,1,7,1,7,1,7,1,7,7,1,7,2},
	{2,7,1,1,7,1,1,1,7,1,7,1,1,1,7,1,1,7,2},
	{2,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,2},
	{2,7,1,1,7,1,7,1,1,1,1,1,7,1,7,1,1,7,2},
	{2,7,7,7,7,1,7,7,7,1,7,7,7,1,7,7,7,7,2},
	{2,2,2,2,7,1,1,1,7,1,7,1,1,1,7,2,2,2,2},
	{0,0,0,2,7,1,0,0,0,0,0,0,0,1,7,2,0,0,0},
	{3,3,3,2,7,1,0,5,5,0,5,5,0,1,7,2,3,3,3},
	{0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0},
	{3,3,3,2,7,1,0,5,5,5,5,5,0,1,7,2,3,3,3},
	{0,0,0,2,7,1,0,0,0,0,0,0,0,1,7,2,0,0,0},
	{2,2,2,2,7,1,1,1,7,1,7,1,1,1,7,2,2,2,2},
	{2,7,7,7,7,1,7,7,7,1,7,7,7,1,7,7,7,7,2},
	{2,7,1,1,7,1,7,1,1,1,1,1,7,1,7,1,1,7,2},
	{2,7,7,7,7,7,7,7,7,0,7,7,7,7,7,7,7,7,2},
	{2,7,1,1,7,1,1,1,7,1,7,1,1,1,7,1,1,7,2},
	{2,7,1,7,7,1,7,1,7,1,7,1,7,1,7,7,1,7,2},
	{2,7,7,7,7,7,7,7,7,1,7,7,7,7,7,7,7,7,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
};//游戏地图

int main()
{
	Wall wall_1 = { 2, 2 };
	Pacman pacMan = { {270,480}, 1, open };
	pacMan.spos.x = 16, pacMan.spos.y = 9;
	Pac pac = { Exist };
	Monster mon_1 = { {120, 120}, 0};
	mon_1.spos.x = 4, mon_1.spos.y = 4;
	Monster mon_2 = { {120, 210}, 0 };
	mon_2.spos.x = 7, mon_2.spos.y = 4;
	Monster mon_3 = { {480, 570}, 0 };
    mon_3.spos.x = 19, mon_3.spos.y = 16;
	Monster mon_4 = { {60, 570}, 0 };
	mon_4.spos.x = 19, mon_4.spos.y = 2;
	//背景图片
	IMAGE gameover;
	IMAGE back;
	IMAGE win;
	//加载图片
	//吃豆人
	loadimage(&pacMan.imga[0][1], _T("image/man_1_r.png"), 30, 30);
	loadimage(&pacMan.imga[1][1], _T("image/man_2_r.png"), 30, 30);
	loadimage(&pacMan.imga[2][1], _T("image/man_3_r.png"), 30, 30);
	loadimage(&pacMan.imga[3][1], _T("image/man_4_r.png"), 30, 30);

	loadimage(&pacMan.imga[0][3], _T("image/man_1_l.png"), 30, 30);
	loadimage(&pacMan.imga[1][3], _T("image/man_2_l.png"), 30, 30);
	loadimage(&pacMan.imga[2][3], _T("image/man_3_l.png"), 30, 30);
	loadimage(&pacMan.imga[3][3], _T("image/man_4_l.png"), 30, 30);

	loadimage(&pacMan.imga[0][0], _T("image/man_1_u.png"), 30, 30);
	loadimage(&pacMan.imga[1][0], _T("image/man_2_u.png"), 30, 30);
	loadimage(&pacMan.imga[2][0], _T("image/man_3_u.png"), 30, 30);
	loadimage(&pacMan.imga[3][0], _T("image/man_4_u.png"), 30, 30);

	loadimage(&pacMan.imga[0][2], _T("image/man_1_d.png"), 30, 30);
	loadimage(&pacMan.imga[1][2], _T("image/man_2_d.png"), 30, 30);
	loadimage(&pacMan.imga[2][2], _T("image/man_3_d.png"), 30, 30);
	loadimage(&pacMan.imga[3][2], _T("image/man_4_d.png"), 30, 30);
	
	//墙和豆子
	loadimage(&wall_1.imga[0], _T("image/Wall_1.png", 2, 2));
	loadimage(&wall_1.imga[1], _T("image/Wall_2.png", 20, 20));
	loadimage(&wall_1.imga[3], _T("image/transfer.png", 2, 2));
	loadimage(&pac.imga, _T("image/pac.png"));
	loadimage(&wall_1.imga[5], _T("image/Home.png", 20, 20));
	
	//怪物 红
	loadimage(&mon_1.imga[0][0], _T("image/red_u.png"), 30, 30);
	loadimage(&mon_1.imga[0][2], _T("image/red_d.png"), 30, 30);
	loadimage(&mon_1.imga[0][3], _T("image/red_l.png"), 30, 30);
	loadimage(&mon_1.imga[0][1], _T("image/red_r.png"), 30, 30);

	loadimage(&mon_1.imga[1][0], _T("image/red_2_u.png"), 30, 30);
	loadimage(&mon_1.imga[1][2], _T("image/red_2_d.png"), 30, 30);
	loadimage(&mon_1.imga[1][3], _T("image/red_2_l.png"), 30, 30);
	loadimage(&mon_1.imga[1][1], _T("image/red_2_r.png"), 30, 30);
	
	//怪物 蓝
	loadimage(&mon_2.imga[0][0], _T("image/blue_u.png"), 30, 30);
	loadimage(&mon_2.imga[0][2], _T("image/blue_d.png"), 30, 30);
	loadimage(&mon_2.imga[0][3], _T("image/blue_l.png"), 30, 30);
	loadimage(&mon_2.imga[0][1], _T("image/blue_r.png"), 30, 30);

	loadimage(&mon_2.imga[1][0], _T("image/blue_2_u.png"), 30, 30);
	loadimage(&mon_2.imga[1][2], _T("image/blue_2_d.png"), 30, 30);
	loadimage(&mon_2.imga[1][3], _T("image/blue_2_l.png"), 30, 30);
	loadimage(&mon_2.imga[1][1], _T("image/blue_2_r.png"), 30, 30);

	//怪物 绿
	loadimage(&mon_3.imga[0][0], _T("image/green_u.png"), 30, 30);
	loadimage(&mon_3.imga[0][2], _T("image/green_d.png"), 30, 30);
	loadimage(&mon_3.imga[0][3], _T("image/green_l.png"), 30, 30);
	loadimage(&mon_3.imga[0][1], _T("image/green_r.png"), 30, 30);

	loadimage(&mon_3.imga[1][0], _T("image/green_2_u.png"), 30, 30);
	loadimage(&mon_3.imga[1][2], _T("image/green_2_d.png"), 30, 30);
	loadimage(&mon_3.imga[1][3], _T("image/green_2_l.png"), 30, 30);
	loadimage(&mon_3.imga[1][1], _T("image/green_2_r.png"), 30, 30);

	//怪物 白
	loadimage(&mon_4.imga[0][0], _T("image/wh_u.png"), 30, 30);
	loadimage(&mon_4.imga[0][2], _T("image/wh_d.png"), 30, 30);
	loadimage(&mon_4.imga[0][3], _T("image/wh_l.png"), 30, 30);
	loadimage(&mon_4.imga[0][1], _T("image/wh_r.png"), 30, 30);

	loadimage(&mon_4.imga[1][0], _T("image/wh_2_u.png"), 30, 30);
	loadimage(&mon_4.imga[1][2], _T("image/wh_2_d.png"), 30, 30);
	loadimage(&mon_4.imga[1][3], _T("image/wh_2_l.png"), 30, 30);
	loadimage(&mon_4.imga[1][1], _T("image/wh_2_r.png"), 30, 30);

	//背景
	loadimage(&gameover, _T("image/gameover.jpg"), 570, 630);
	loadimage(&back, _T("image/back.jpg"), 570, 630);
	loadimage(&win, _T("image/success.jpg"), 570, 630);

	initgraph(570, 630);
	int c;
	//启动图画
	putimage(0, 0, &back);
	
	LOGFONT f;
	gettextstyle(&f);						// 获取当前字体设置
	f.lfHeight = 36;						// 设置字体高度为 48
	_tcscpy(f.lfFaceName, _T("黑体"));		// 设置字体为“黑体”
	f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&f);						// 设置字体样式
	settextcolor(YELLOW);

	//画豆子(a豆子);
	char welcome[] = "Welcome to Pacman!";
	char ask[] = "(press 'y' to start a game)";
	outtextxy(130, 50, welcome);
	outtextxy(50, 550, ask);
	char key = _getch();
	if (key == 'y') {
		//开始游戏
		while (true)
		{
			LOGFONT f;
			gettextstyle(&f);						
			f.lfHeight = 36;						
			_tcscpy(f.lfFaceName, _T("黑体"));		
			f.lfQuality = ANTIALIASED_QUALITY;		 
			settextstyle(&f);						
			settextcolor(YELLOW);

			BeginBatchDraw();
			checkEdge(&pacMan);
			drawMap(map, &wall_1, &pac);
			checkPac(map, &pacMan, &pac);
			c = GetCmd();
			//游戏进行时
			if (checkClear(map, &mon_1, &mon_2, &mon_3, &mon_4, &pacMan) == 1) {
				moveMon(&mon_1, map, rand() % 4 + 0);
				moveMon(&mon_2, map, rand() % 4 + 0);
				moveMon(&mon_3, map, rand() % 4 + 0);
				moveMon(&mon_4, map, rand() % 4 + 0);
				drawMonster(mon_1);
				drawMonster(mon_2);
				drawMonster(mon_3);
				drawMonster(mon_4);
				drawPacman(&pacMan);
				DispatchCmd(&pacMan, c, map);
				FlushBatchDraw();
				Sleep(100);
				cleardevice();
			}
			//游戏成功时
			if (checkClear(map, &mon_1, &mon_2, &mon_3, &mon_4, &pacMan) == 0) {
				drawPacman(&pacMan);
				putimage(0, 0, &win);
				char words[] = "You have succeed!";
				outtextxy(140, 170, words);
				FlushBatchDraw();
				Sleep(100);
				cleardevice();
			}
			//游戏失败时
			if (checkClear(map, &mon_1, &mon_2, &mon_3, &mon_4, &pacMan) == 3) {
				drawPacman(&pacMan);
				putimage(0, 0, &gameover);
				FlushBatchDraw();
				Sleep(100);
				cleardevice();
			}
		}
		EndBatchDraw();
		_getch();
		closegraph();
		return 0;
	}
}

void drawPacman(Pacman* a)
{
	openMouth(a);
	if (a->direc == right)
		if (a->amouth == openbb)
			putimage(a->pos.x, a->pos.y, &a->imga[3][1]);
		else if (a->amouth == openb)
			putimage(a->pos.x, a->pos.y, &a->imga[2][1]);
		else if (a->amouth == open)
			putimage(a->pos.x, a->pos.y, &a->imga[1][1]);
		else if (a->amouth == shut)
			putimage(a->pos.x, a->pos.y, &a->imga[0][1]);
	if (a->direc == left)
		if(a->amouth == openbb)
			putimage(a->pos.x, a->pos.y, &a->imga[3][3]);
		else if (a->amouth == openb)
			putimage(a->pos.x, a->pos.y, &a->imga[2][3]);
		else if (a->amouth == open)
			putimage(a->pos.x, a->pos.y, &a->imga[1][3]);
		else if (a->amouth == shut)
			putimage(a->pos.x, a->pos.y, &a->imga[0][3]);
	if (a->direc == down)
		if (a->amouth == openbb)
			putimage(a->pos.x, a->pos.y, &a->imga[3][2]);
		else if (a->amouth == openb)
			putimage(a->pos.x, a->pos.y, &a->imga[2][2]);
		else if (a->amouth == open)
			putimage(a->pos.x, a->pos.y, &a->imga[1][2]);
		else if (a->amouth == shut)
			putimage(a->pos.x, a->pos.y, &a->imga[0][2]);
	if (a->direc == up)
		if (a->amouth == openbb)
			putimage(a->pos.x, a->pos.y, &a->imga[3][0]);
		else if (a->amouth == openb)
			putimage(a->pos.x, a->pos.y, &a->imga[2][0]);
		else if (a->amouth == open)
			putimage(a->pos.x, a->pos.y, &a->imga[1][0]);
		else if (a->amouth == shut)
			putimage(a->pos.x, a->pos.y, &a->imga[0][0]);
}

void movePacman(Pacman* a, direc pdirec)
{
	a->direc = pdirec;
	if (pdirec == left) {
		a->pos.x -= 30;
		a->spos.y -= 1;
	}

	if (pdirec == right) {
		a->pos.x += 30;
		a->spos.y += 1;
	}
	if (pdirec == down) {
		a->pos.y += 30;
		a->spos.x += 1;
	}

	if (pdirec == up) {
		a->pos.y -= 30;
		a->spos.x -= 1;
	}
}

int GetCmd()
{
	int c = 0;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)		c |= CMD_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)   	c |= CMD_DOWN;
	return c;
}
// 处理用户输入的命令

void DispatchCmd(Pacman* a, int cmd, int map[21][19])
{
	if (cmd & CMD_UP)			OnUp(a);
	if (cmd & CMD_DOWN)			OnDown(a);
	if (cmd & CMD_LEFT)			OnLeft(a);
	if (cmd & CMD_RIGHT)		OnRight(a);
}

void OnUp(Pacman* a)
{
	if (map[a->spos.x - 1][a->spos.y] != 1 && map[a->spos.x - 1][a->spos.y] != 2 && map[a->spos.x - 1][a->spos.y] != 5)
	{
		movePacman(a, up);
	}
}
// 向left移动
void OnLeft(Pacman* a)
{
	if (map[a->spos.x][a->spos.y - 1] != 1 && map[a->spos.x][a->spos.y - 1] != 2 && map[a->spos.x][a->spos.y - 1] != 5)
	{
		movePacman(a, left);
		
	}
	//判断传送
	if (a->pos.x - 30 < 0) {
		a->pos.x  = 540;
		a->spos.y = 18;
		a->direc = 3;
	}
}
// 向right移动
void OnRight(Pacman* a)
{
	if (a->pos.x + 30 > 570) {
			a->pos.x = 0;
			a->spos.y = 0;
			a->direc = 1;
		}
	if (map[a->spos.x][a->spos.y + 1] != 1 && map[a->spos.x][a->spos.y + 1] != 2 && map[a->spos.x][a->spos.y + 1] != 5)
	{
		movePacman(a, right);
	}
	//判断传送
	
}
// 向down移动
void OnDown(Pacman* a)
{
	if (map[a->spos.x + 1][a->spos.y] != 1 && map[a->spos.x + 1][a->spos.y] != 2 && map[a->spos.x + 1][a->spos.y] != 5)
	{
		movePacman(a, down);
	}
}
//画地图
void drawMap(int map[21][19], Wall* wall_1, Pac* pac) {
	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 21; j++) {
			int x = 30 * i, y = 30 * j;
			switch (map[j][i])
			{
			case 0:
				break;
			case 1:
				putimage(x, y, &wall_1->imga[0]);
				break;
			case 2:
				putimage(x, y, &wall_1->imga[1]);
				break;
			case 3:
				putimage(x, y, &wall_1->imga[3]);
				break;
			case 5:
				putimage(x, y, &wall_1->imga[5]);
				break;
			case 7:
				putimage(x, y, &pac->imga);
			default:
				break;
			}
		}
	}
}
//吃豆
void checkPac(int map[21][19], Pacman* a, Pac* pac) {
	if (map[a->spos.x][a->spos.y] == 7) map[a->spos.x][a->spos.y] = 0;
}
//open
void openMouth(Pacman* a) {
	if (a->pos.x % 30 == 0)
		if (a->amouth == openbb)
			a->amouth = shut;
		else if (a->amouth == openb)
			a->amouth = openbb;
		else if (a->amouth == open)
			a->amouth = openb;
		else if(a->amouth == shut)
			a->amouth = open;
	if (a->pos.y % 30 == 0)
		if (a->amouth == openbb)
			a->amouth = shut;
		else if (a->amouth == openb)
			a->amouth = openbb;
		else if (a->amouth == open)
			a->amouth = openb;
		else if (a->amouth == shut)
			a->amouth = open;
}
//传送
void checkEdge(Pacman* a) {
	if (a->pos.x - 30 < 0) {
		a->pos.x = 570;
		a->direc = 3;
	}
	if (a->pos.x > 570) {
		a->pos.x = 0;
		a->direc = 1;
	}
}
//检查通关
int checkClear(int map[21][19], Monster* mon1, Monster* mon2, Monster* mon3, Monster* mon4, Pacman* a) {
	int n = 0;
	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 21; j++) {
			if (map[j][i] == 7) {
				n = 1;
			}
		}
	}
	if (mon1->pos.x == a->pos.x && mon1->pos.y == a->pos.y) return 3;
	if (mon2->pos.x == a->pos.x && mon2->pos.y == a->pos.y) return 3;
	if (mon3->pos.x == a->pos.x && mon3->pos.y == a->pos.y) return 3;
	if (mon4->pos.x == a->pos.x && mon4->pos.y == a->pos.y) return 3;
	else return n;
}
//移动怪物
void moveMon(Monster* mon, int map[21][19], int direc) {
	//up
	mon->direc = direc;
	if (direc == up) {
		if (map[mon->spos.x - 1][mon->spos.y] == 0 || map[mon->spos.x - 1][mon->spos.y] == 7)
		{
			mon->pos.y -= 30;
			mon->spos.x -= 1;
		}
	}
	//down
	if (direc == right) {
		if (map[mon->spos.x][mon->spos.y+1] == 0 || map[mon->spos.x][mon->spos.y+1] == 7)
		{
			mon->pos.x += 30;
			mon->spos.y += 1;
		}
	}
	//left
	if (direc == left) {
		if (map[mon->spos.x][mon->spos.y - 1] == 0 || map[mon->spos.x][mon->spos.y - 1] == 7)
		{
			mon->pos.x -= 30;
			mon->spos.y -= 1;
		}
	}
	//right
	if (direc == down) {
		if (map[mon->spos.x + 1][mon->spos.y] == 0 || map[mon->spos.x + 1][mon->spos.y] == 7)
		{
			mon->pos.y += 30;
			mon->spos.x += 1;
		}
		
	}
}

void drawMonster(Monster mon) {
	monsterWalk(&mon);
	if (mon.direc == up)
	{
		if (mon.stat == 0) putimage(mon.pos.x, mon.pos.y, &mon.imga[0][0]);
		else putimage(mon.pos.x, mon.pos.y, &mon.imga[1][0]);
	}
	if (mon.direc == down)
	{
		if (mon.stat == 0) putimage(mon.pos.x, mon.pos.y, &mon.imga[0][2]);
		else putimage(mon.pos.x, mon.pos.y, &mon.imga[1][2]);
	}
	if (mon.direc == left)
	{
		if (mon.stat == 0) putimage(mon.pos.x, mon.pos.y, &mon.imga[0][3]);
		else putimage(mon.pos.x, mon.pos.y, &mon.imga[1][3]);
	}
	if (mon.direc == right)
	{
		if (mon.stat == 0) putimage(mon.pos.x, mon.pos.y, &mon.imga[0][1]);
		else putimage(mon.pos.x, mon.pos.y, &mon.imga[1][1]);
	}
}

void monsterWalk(Monster* mon) {
	if (mon->pos.x % 30 == 0) {
		if (mon->stat == 0) mon->stat = 1;
		else if (mon->stat == 1) mon->stat = 0;
	}
	if (mon->pos.y % 30 == 0) {
		if (mon->stat == 0) mon->stat = 1;
		else if (mon->stat == 1) mon->stat = 0;
	}
}

void checkMonsterWall (int map[21][19], Pacman* a, Monster* mon) {
	int n = rand() % 4 + 0;
	int x = 0, b = 0, c=0, d=0;
	if (map[mon->spos.x-1][mon->spos.y] != 0 && map[mon->spos.x - 1][mon->spos.y] != 7) {
		x = 1;
		while (n == 0) n = rand() % 4 + 0;
		moveMon(mon, map, n);
	}
	else if (map[mon->spos.x + 1][mon->spos.y] != 0 && map[mon->spos.x + 1][mon->spos.y] != 7) {
		b = 1;
		while (n == 2) n = rand() % 4 + 0;
		moveMon(mon, map, n);
	}
	else if (map[mon->spos.x][mon->spos.y - 1] != 0 && map[mon->spos.x][mon->spos.y - 1] != 7) {
		c = 1;
		while (n == 3) n = rand() % 4 + 0;
		moveMon(mon, map, n);
	}
	else if (map[mon->spos.x][mon->spos.y + 1] != 0 && map[mon->spos.x][mon->spos.y+1] != 7) {
		d = 1;
		while (n == 1) n = rand() % 4 + 0;
		moveMon(mon, map, n);
	}
	else moveMon(mon, map, n);

	
	
}

