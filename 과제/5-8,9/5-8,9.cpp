#pragma comment(lib, "msimg32.lib")
#include <Windows.h>
#include <iostream>
#include <time.h>
#include "resource.h"

#define WIDTH 1000
#define HEIGHT 800
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

RECT win_size;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM IParam);
void Draw(HDC hdc, HBITMAP hbit, int x, int y, int sizeX, int sizeY);
void DrawB(HDC hdc, HBITMAP hbit, int x, int y);
bool CollideBox(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;


	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "5-8,9", WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	srand(unsigned(time(NULL)));
	PAINTSTRUCT ps;
	HDC hDC,memDC;
	HBITMAP hBitmap;
	
	static HBITMAP Player;

	static int playerX = 465;
	static int playerY = 365;
	static int pAniX = 0;
	static int pAniY = 0;
	static int pDirect = 0;
	static int pCollide = 10;


	static HBITMAP Bullet;

	static int bulletX[100];
	static int bulletY[100];
	static int bDirect[100];
	static bool bulletOn[100];
	static int bCount = 0;

	static HBITMAP Hurdle;
	static HBITMAP Hurdle2;
	static int hurdleX[3];
	static int hurdleY[3];
	static int hurdleSizeX[3];
	static int hurdleSizeY[3];
	static bool hurdleOn[3];
	static bool crack[3];

	static HBITMAP Monster;

	static int monsterX[3];
	static int monsterY[3];
	static int mAniX[3];
	static int mAniY[3];
	static int mDirect[3];
	static bool mCollide[3];
	static bool mDead[3];

	static int dead[3];

	static char buf[100];

	switch (iMessage)
	{

	case WM_CREATE:
		GetClientRect(hWnd, &win_size);
		// 70*70 위 아래 오 왼
		Player = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_PLAYER));
		Bullet = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BULLET));
		Monster = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_MONSTER));
		Hurdle = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		Hurdle2 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		pDirect = rand() % 4;
		pAniY = pDirect * 70;
		SetTimer(hWnd, 1, 50, NULL);
		SetTimer(hWnd, 2, 5, NULL);
		SetTimer(hWnd, 3, 50, NULL);
		SetTimer(hWnd, 4, 50, NULL);

		for (int i = 0; i < 3; ++i)
		{
			hurdleX[i] = rand() % 800 + 100;
			hurdleY[i] = rand() % 600 + 100;
			hurdleSizeX[i] = rand() % 60 + 50;
			hurdleSizeY[i] = rand() % 60 + 50;
			monsterX[i] = hurdleX[i];
			monsterY[i] = hurdleY[i];
			mDirect[i] = rand() % 4;
			hurdleOn[i] = true;
			for (int j = 0; j < i; ++j)
				if (CollideBox(hurdleX[i], hurdleY[i], hurdleSizeX[i], hurdleSizeY[i], hurdleX[j], hurdleY[j], hurdleSizeX[j], hurdleSizeY[j]))
					--i;
		}

		return 0;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			// 캐릭터 이동 및 애니메이션
		{

			pAniY = pDirect * 70;
			pAniX += 70;
			if (pAniX > 210)
				pAniX = 0;
					if (pDirect == UP)
					{
						if (playerY > 0&&pDirect!=pCollide)
							playerY -= 5;
					}
					else if (pDirect == DOWN)
					{
						if (playerY < win_size.bottom - 70 && pDirect != pCollide)
							playerY += 5;
					}
					else if (pDirect == RIGHT)
					{
						if (playerX < win_size.right - 70 && pDirect != pCollide)
							playerX += 5;
					}
					else if (pDirect == LEFT)
					{
						if (playerX > 0 && pDirect != pCollide)
							playerX -= 5;
					}
					for (int i = 0; i < 3; ++i)
					{
						if (CollideBox(playerX, playerY, 70, 70, hurdleX[i], hurdleY[i], hurdleSizeX[i], hurdleSizeY[i]))
							pCollide = pDirect;
					}
					if (!CollideBox(playerX, playerY, 70, 70, hurdleX[0], hurdleY[0], hurdleSizeX[0], hurdleSizeY[0]))
						if (!CollideBox(playerX, playerY, 70, 70, hurdleX[1], hurdleY[1], hurdleSizeX[1], hurdleSizeY[1]))
							if (!CollideBox(playerX, playerY, 70, 70, hurdleX[2], hurdleY[2], hurdleSizeX[2], hurdleSizeY[2]))
						pCollide = 10;
			break;
		}
		
		case 2:
			// 총알 발사
		{
			for (int i = 0; i <= bCount; ++i)
			{
				if (bDirect[i] == LEFT)
				{
					bulletX[i] -= 5;
				}
				else if (bDirect[i] == RIGHT)
				{
					bulletX[i] += 5;

				}
				else if (bDirect[i] == UP)
				{
					bulletY[i] -= 5;

				}
				else if (bDirect[i] == DOWN)
				{
					bulletY[i] += 5;

				}
			}
			break;
		}
		
		case 3:
			// 몬스터 이동 및 애니메이션
		{
			for (int i = 0; i < 3; ++i)
			{
				if (dead[i] > 0)
					dead[i]++;
					mAniX[i] += 70;
				if (dead[i] < 5)
				{
					if (mAniX[i] > 210)
						mAniX[i] = 0;
				}
				else
				{
					mDead[i] = true;
				}
				if(!mCollide[i]){
				mAniY[i] = mDirect[i] * 70;
				if (mDirect[i] == UP)
				{
					if (monsterY[i] > 0)
						monsterY[i] -= 5;
					else mDirect[i] = rand() % 4;
				}
				else if (mDirect[i] == DOWN)
				{
					if (monsterY[i] < win_size.bottom - 70)
						monsterY[i] += 5;
					else mDirect[i] = rand() % 4;
				}
				else if (mDirect[i] == RIGHT)
				{
					if (monsterX[i] < win_size.right - 70)
						monsterX[i] += 5;
					else mDirect[i] = rand() % 4;
				}
				else if (mDirect[i] == LEFT)
				{
					if (monsterX[i] > 0)
						monsterX[i] -= 5;
					else mDirect[i] = rand() % 4;
				}
				}
				for (int i = 0; i < 3; ++i)
				{
					if (CollideBox(monsterX[i], monsterY[i], 70, 70, hurdleX[i], hurdleY[i], hurdleSizeX[i], hurdleSizeY[i]))
						mDirect[i] = rand() % 4;
				}
			}
			break;
		}

		case 4:
		// 몬스터 총알 충돌
		{
			for (int j = 0; j < 100; ++j)
			{
				for (int i = 0; i < 3; ++i) {
					if (bulletOn[j]&&!mDead[i])
					{
						if (mCollide[i])
						{
							if (CollideBox(bulletX[j] - 9, bulletY[j] - 9, 18, 18, monsterX[i], monsterY[i], 70, 70))
							{
								mAniY[i] = 420;
								SetTimer(hWnd, 10, 50, NULL);
								bulletOn[j] = false;
								dead[i]++;
							}
						}
						else if (CollideBox(bulletX[j] - 9, bulletY[j] - 9, 18, 18, monsterX[i], monsterY[i], 70, 70))
						{
							mAniY[i] = 280;
							mCollide[i] = true;
							bulletOn[j] = false;

						}
					}
				}
				for (int i = 0; i < 3; ++i)
				{
					if (bulletOn[j] && hurdleOn[i])
						if (CollideBox(bulletX[j] - 9, bulletY[j] - 9, 18, 18, hurdleX[i], hurdleY[i], hurdleSizeX[i], hurdleSizeY[i]))
						{
							bulletOn[j] = false;
							if (!crack[i])
								crack[i] = true;
							else
							{
								hurdleOn[i] = false;
								hurdleSizeX[i] = 0;
								hurdleSizeY[i] = 0;
								hurdleX[i] = 0;
								hurdleY[i] = 0;
							}
						}

				}
			}
			break;
		}

		case 10:
		{
			
			break;
		}
		}
		InvalidateRect(hWnd, NULL, FALSE);  // WM_PAINT 메시지 발생 
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, win_size.right, win_size.bottom);
		SelectObject(memDC, (HBITMAP)hBitmap);
		PatBlt(memDC, win_size.left, win_size.top, win_size.right, win_size.bottom, WHITENESS);
		//---------------------------------------------------------------------------------

		for (int i = 0; i < 3; ++i)
			if (hurdleOn[i])
			{
				if(!crack[i])
					Draw(memDC, Hurdle, hurdleX[i], hurdleY[i], 0, 0);
				else
				{
					Draw(memDC, Hurdle2, hurdleX[i], hurdleY[i], 0, 0);
				}
			}
		

		for (int i = 0; i < 100; ++i)
		{
			if (bulletOn[i])
				DrawB(memDC, Bullet, bulletX[i], bulletY[i]);
		}
		Draw(memDC, Player, playerX, playerY, pAniX, pAniY);
		for(int i = 0 ;i < 3; ++i)
			Draw(memDC, Monster, monsterX[i], monsterY[i], mAniX[i], mAniY[i]);

		sprintf_s(buf,"총알 개수: %d", 100 - bCount);
		TextOut(memDC, 10, 10, buf, strlen(buf));
		TextOut(memDC, 10, 30, "장전: r", strlen("장전: r"));



		//---------------------------------------------------------------------------------
		BitBlt(hDC, 0, 0, win_size.right, win_size.bottom, memDC, 0, 0, SRCCOPY);
		DeleteDC(memDC);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
	
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		case VK_LEFT:
			pDirect = LEFT;
			break;

		case VK_RIGHT:
			pDirect = RIGHT;
			break;

		case VK_UP:
			pDirect = UP;
			break;

		case VK_DOWN:
			pDirect = DOWN;
			break;

		case VK_SPACE:
			if (bCount < 100)
			{
				bulletX[bCount] = playerX + 35;
				bulletY[bCount] = playerY + 35;
				bDirect[bCount] = pDirect;
				bulletOn[bCount] = true;
      				bCount++;
			}
			break;
		}

		InvalidateRect(hWnd, NULL, FALSE);  // WM_PAINT 메시지 발생 
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'r':
			for (int i = 0; i <= bCount; ++i)
				bulletOn[i] = false;
			bCount = 0;
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void Draw(HDC hdc, HBITMAP hbit, int x, int y,int aX, int aY)
{
	BITMAP bmp;
	HDC memdc;

	memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, hbit);
	GetObject(hbit, sizeof(BITMAP), &bmp);

	TransparentBlt(hdc, x, y, 70, 70, memdc, aX, aY, 70, 70, RGB(0, 255, 0));
	
	DeleteDC(memdc);
}
void DrawB(HDC hdc, HBITMAP hbit, int x, int y)
{
	BITMAP bmp;
	HDC memdc;

	memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, hbit);
	GetObject(hbit, sizeof(BITMAP), &bmp);

	TransparentBlt(hdc, x - 9, y - 9, 18, 18, memdc, 0, 0, 18, 18, RGB(0, 255, 0));

	DeleteDC(memdc);
}
bool CollideBox(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
{
	RECT a;
	RECT b;
	a.left = ax;
	a.right = ax + aw;
	a.top = ay;
	a.bottom = ay+ah;
	b.left = bx;
	b.right = bx + bw;
	b.top = by;
	b.bottom = by+bh;

	if(a.left<b.right&&a.right>b.left&&a.top<b.bottom&&a.bottom>b.top)
		return true;
	else return false;
}