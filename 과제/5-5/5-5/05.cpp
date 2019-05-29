#include <Windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Animation_bg(HDC backdc, RECT win);
void Animation_g(HDC backdc, RECT win);
void Animation_Char(HDC backdc, int xPos, int yPos);
void Animation_Enemy(HDC backdc, int xPos, int yPos);
void Animation_Enemy2(HDC backdc, int xPos, int yPos);
bool Animation_exp(HDC backdc, int xPos, int yPos);

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
	//WndClass.lpszMenuName = MAKEINTRESOURCE (IDR_MENU1);
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "Window API", WS_OVERLAPPEDWINDOW, 0, 0, 1000, 800, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static HDC memdc, backdc;
	static HBITMAP hBit, oldBit, hBackBit, oldBackBit;
	static BITMAP bmp;
	static RECT wSize;
	PAINTSTRUCT ps;

	static int cx, cy, ex, ey, E1x, E1y, E2x, E2y;
	static int jumpCount;
	static int sleepCount;
	static bool expCheck;
	static bool jumpCheck;
	static bool sleepCheck;
	static bool colCheck;
	static WPARAM key;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &wSize);
		SetTimer(hWnd, 1, 70, NULL);
		cx = 0; cy = 450;
		ex = 800; ey = 470;
		E1x = 800; E1y = 60;
		E2x = 800; E2y = 150;
		key = 0;
		expCheck = FALSE;
		jumpCheck = FALSE;
		colCheck = FALSE;
		sleepCheck = FALSE;
		break;

	case WM_TIMER:
		hdc = GetDC(hWnd);
		hBackBit = CreateCompatibleBitmap(hdc, wSize.right, wSize.bottom);
		backdc = CreateCompatibleDC(hdc);
		oldBackBit = (HBITMAP)SelectObject(backdc, hBackBit);

		{
			E1x -= 5;
			if (E1x + 90 < 0)
				E1x = wSize.right - 10;

			E2x -= 10;
			if (E2x + 90 < 0)
				E2x = wSize.right - 10;

			if (!colCheck && ((ex < cx + 200 && cx + 200 < ex + 80) && (ey + 10 < cy + 50 && cy + 50 < ey + 120)))
			{
				colCheck = TRUE;
			}

			if (!colCheck)
			{
				ex -= 15;
				if (ex + 90 < 0)
					ex = wSize.right - 10;
				
			}

			if (jumpCheck)
			{
				if (jumpCount < 11) cy -= 10;
				else cy += 10;

				if (jumpCount == 20) jumpCheck = FALSE;
				jumpCount++;
			}

			else if (sleepCheck)
			{
				if (sleepCount < 11) cy += 7;
				else 	cy -= 7;
					
				if (sleepCount == 20) sleepCheck = FALSE;
				sleepCount++;
			}
		
			if (key == 'A')
			{
				if (cx > 0)
					cx -= 10;
			}
			else if (key == 'D')
			{
				if (cx < wSize.right - 250)
					cx += 10;
			}

		}

		Animation_bg(backdc, wSize); // 배경 함수 호출
		Animation_Enemy(backdc, ex, ey);
		Animation_Enemy2(backdc, E1x, E1y);
		Animation_Enemy2(backdc, E2x, E2y);
		Animation_Char(backdc, cx, cy);

		if (sleepCheck) {
			Animation_g(backdc, wSize); // 땅 함수 호출
		}

		if (colCheck) // 충돌시 발생
		{
			colCheck = Animation_exp(backdc, ex, ey);

			if (!colCheck)
				ex = wSize.right - 10;
		}

		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		BitBlt(hdc, 0, 0, wSize.right, wSize.bottom, backdc, 0, 0, SRCCOPY); //  backdc에 있는 데이터들을 그림

		SelectObject(hdc, oldBackBit);
		DeleteDC(backdc); // 지움
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:

		// 스페이스바 누르면 점프
		if (wParam == VK_SPACE && !jumpCheck)
		{
			jumpCount = 1;
			jumpCheck = TRUE;
		}

		// s 누르면 땅속으로
		else if (wParam == 's'|| wParam == 'S' && !sleepCheck)
		{
			sleepCount = 1;
			sleepCheck = TRUE;
		}

		// 좌로 움직임
		else if (wParam == 'A') 
			key = 'A';
		
		//우로 움직임
		else if (wParam == 'D')
			key = 'D';

		break;

	case WM_KEYUP:
		if (wParam == key)
			key = 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 배경 함수
void Animation_bg(HDC backdc, RECT win)
{
	HDC memdc;
	HBITMAP skyBit, gBit, oldBit;
	BITMAP bmp;
	static int scount, gcount;
	int slen, glen;

	scount++;
	scount = scount % 300;
	gcount++;
	gcount = gcount % 50;

	skyBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SKY)); // 하늘
	gBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GROUND)); // 땅

	memdc = CreateCompatibleDC(backdc);

	oldBit = (HBITMAP)SelectObject(memdc, skyBit);
	GetObject(skyBit, sizeof(BITMAP), &bmp);
	slen = (bmp.bmWidth / 2) / 300;
	StretchBlt(backdc, 0, 0, win.right, win.bottom, memdc, scount * slen, 0, bmp.bmWidth / 2, bmp.bmHeight, SRCCOPY);

	SelectObject(memdc, gBit);
	GetObject(gBit, sizeof(BITMAP), &bmp);
	glen = (bmp.bmWidth / 2) / 50;
	StretchBlt(backdc, 0, 600, win.right, win.bottom - 600, memdc, gcount * glen, 0, bmp.bmWidth / 2, bmp.bmHeight, SRCCOPY);

	SelectObject(memdc, oldBit);
	DeleteObject(skyBit);
	DeleteObject(gBit);
	DeleteDC(memdc);
}

// 땅함수
void Animation_g(HDC backdc, RECT win)
{
	HDC memdc;
	HBITMAP  gBit, oldBit;
	BITMAP bmp;
	static int  gcount;
	int  glen;

	
	gcount++;
	gcount = gcount % 50;


	gBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GROUND)); // 땅

	memdc = CreateCompatibleDC(backdc);


	oldBit = (HBITMAP)SelectObject(memdc, gBit);
	GetObject(gBit, sizeof(BITMAP), &bmp);
	glen = (bmp.bmWidth / 2) / 50;
	StretchBlt(backdc, 0, 600, win.right, win.bottom - 600, memdc, gcount * glen, 0, bmp.bmWidth / 2, bmp.bmHeight, SRCCOPY);

	SelectObject(memdc, oldBit);
	DeleteObject(gBit);
	DeleteDC(memdc);
}

// 메인 캐릭터 그리기
void Animation_Char(HDC backdc, int xPos, int yPos)
{
	HDC memdc;
	HBITMAP _char[12], _mask[12], hBit, oldBit;
	BITMAP bmp;
	static int count;

	count++;
	count = count % 12;

	// 기본 말 틀
	_char[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW01));
	_char[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW02));
	_char[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW03));
	_char[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW04));
	_char[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW05));
	_char[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW06));
	_char[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW07));
	_char[7] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW08));
	_char[8] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW09));
	_char[9] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW10));
	_char[10] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW11));
	_char[11] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_DRAW12));


	// 말 위에 불이 날리는 효과
	_mask[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK01));
	_mask[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK02));
	_mask[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK03));
	_mask[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK04));
	_mask[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK05));
	_mask[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK06));
	_mask[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK07));
	_mask[7] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK08));
	_mask[8] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK09));
	_mask[9] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK10));
	_mask[10] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK11));
	_mask[11] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HORSE_MASK12));

	memdc = CreateCompatibleDC(backdc);

	SelectObject(memdc, _mask[count]);
	GetObject(_mask[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos + 250, yPos, -250, 170, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);
	SelectObject(memdc, _char[count]);
	GetObject(_char[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos + 250, yPos, -250, 170, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);

	for (int i = 0; i < 12; i++)
	{
		DeleteObject(_char[i]);
		DeleteObject(_mask[i]);
	}
	DeleteDC(memdc);
}

// 적2 그리기
void Animation_Enemy2(HDC backdc, int xPos, int yPos)
{
	HDC memdc;
	HBITMAP _enemy[5], _mask[5], hBit, oldBit;
	BITMAP bmp;
	static int count;

	count++;
	count = count % 5;

	_enemy[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_DRAW01));
	_enemy[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_DRAW02));
	_enemy[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_DRAW03));
	_enemy[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_DRAW04));
	_enemy[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_DRAW05));

	_mask[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_MASK01));
	_mask[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_MASK02));
	_mask[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_MASK03));
	_mask[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_MASK04));
	_mask[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_MASK05));

	memdc = CreateCompatibleDC(backdc);

	SelectObject(memdc, _mask[count]);
	GetObject(_mask[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos, yPos, 63, 98, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);
	SelectObject(memdc, _enemy[count]);
	GetObject(_enemy[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos, yPos, 63, 98, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);

	for (int i = 0; i < 5; i++)
	{
		DeleteObject(_enemy[i]);
		DeleteObject(_mask[i]);
	}
	DeleteDC(memdc);
}

void Animation_Enemy(HDC backdc, int xPos, int yPos)
{
	HDC memdc;
	HBITMAP _enemy[5], _mask[5], hBit, oldBit;
	BITMAP bmp;
	static int count;

	count++;
	count = count % 5;

	_enemy[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_DRAW01));
	_enemy[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_DRAW02));
	_enemy[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_DRAW03));
	_enemy[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_DRAW04));
	_enemy[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_DRAW05));

	_mask[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_MASK01));
	_mask[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_MASK02));
	_mask[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_MASK03));
	_mask[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_MASK04));
	_mask[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENEMY_MASK05));

	memdc = CreateCompatibleDC(backdc);

	SelectObject(memdc, _mask[count]);
	GetObject(_mask[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos, yPos, 90, 140, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);
	SelectObject(memdc, _enemy[count]);
	GetObject(_enemy[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos, yPos, 90, 140, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);

	for (int i = 0; i < 5; i++)
	{
		DeleteObject(_enemy[i]);
		DeleteObject(_mask[i]);
	}
	DeleteDC(memdc);
}

// 충돌 시 폭발 애니메이션
bool Animation_exp(HDC backdc, int xPos, int yPos)
{
	HDC memdc;
	HBITMAP _exp[6], _mask[6], oldBit;
	BITMAP bmp;
	static int count;

	_exp[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_DRAW01));
	_exp[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_DRAW02));
	_exp[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_DRAW03));
	_exp[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_DRAW04));
	_exp[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_DRAW05));
	_exp[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_DRAW06));

	_mask[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_MASK01));
	_mask[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_MASK02));
	_mask[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_MASK03));
	_mask[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_MASK04));
	_mask[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_MASK05));
	_mask[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_EXP_MASK06));

	memdc = CreateCompatibleDC(backdc);

	SelectObject(memdc, _mask[count]);
	GetObject(_mask[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos, yPos, 140, 140, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);

	SelectObject(memdc, _exp[count]);
	GetObject(_exp[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos, yPos, 140, 140, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);

	for (int i = 0; i < 6; i++)
	{
		DeleteObject(_exp[i]);
		DeleteObject(_mask[i]);
	}
	DeleteDC(memdc);

	if (count == 5)
	{
		count = 0;
		return FALSE;
	}
	else
	{
		count++;
		return TRUE;
	}
}