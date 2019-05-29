#include <Windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

enum ANI { HORSE, TIGER };

RECT wSize;

int cx, cy, ex, ey, tx, ty;
int wid, hei;
int jumpCount;
int sleepCount;
int speed;
int animal;
bool jumpCheck;
bool sleepCheck;
bool writeCheck;
bool shadeCheck;
bool exitCheck;
bool openD;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DiagProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void Animation_bg(HDC backdc, RECT win);
void Animation_g(HDC backdc, RECT win);
void Animation_Horse(HDC backdc, int xPos, int yPos);
void Animation_Tiger(HDC backdc, int xPos, int yPos);
void Animation_Enemy(HDC backdc, int xPos, int yPos);
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
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC memdc, backdc;
	static HBITMAP hBit, oldBit, hBackBit, oldBackBit;
	static BITMAP bmp;
	static HFONT hFont, oldFont;

	static int textDir;
	static bool expCheck;
	static bool colCheck;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &wSize);
		SetTimer(hWnd, 1, 70, NULL);
		cx = 0; cy = 450;
		ex = 800; ey = 470;
		tx = 0, ty = 100;
		wid = 250, hei = 170;
		textDir = 0;
		speed = 20;
		animal = HORSE;
		expCheck = FALSE;
		jumpCheck = FALSE;
		sleepCheck = FALSE;
		colCheck = FALSE;
		writeCheck = FALSE;
		shadeCheck = FALSE;
		exitCheck = FALSE;
		openD = FALSE;

		hFont = CreateFont(50, 0, 0, 0, 500, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Consolas");
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);
		hBackBit = CreateCompatibleBitmap(hdc, wSize.right, wSize.bottom);
		backdc = CreateCompatibleDC(hdc);
		oldBackBit = (HBITMAP)SelectObject(backdc, hBackBit);

		{
			if (!colCheck && ((ex < cx + wid - (wid / 5) && cx + wid - (wid / 5) < ex + 80) 
				&& (ey + 10 < cy + hei - (hei / 2) && cy + hei - (hei / 2) < ey + 120)))
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

			if (sleepCheck)
			{
				if (sleepCount < 11) cy += 10;
				else cy -= 10;

				if (sleepCount == 20) sleepCheck = FALSE;
				sleepCount++;
			}
		}

		// Draw Animation
		Animation_bg(backdc, wSize);
		Animation_Enemy(backdc, ex, ey);
		if (animal == HORSE)
			Animation_Horse(backdc, cx, cy);
		else if (animal == TIGER)
			Animation_Tiger(backdc, cx, cy);
		Animation_g(backdc, wSize); // ¶¥ ÇÔ¼ö È£Ãâ

		// Draw Explosion
		if (colCheck)
		{
			colCheck = Animation_exp(backdc, ex, ey);

			if (!colCheck)
				ex = wSize.right - 10;
		}

		// Write at sky
		if (writeCheck)
		{
			oldFont = (HFONT)SelectObject(backdc, hFont);
			SetBkMode(backdc, TRANSPARENT);
			TextOut(backdc, cx+100, cy-50, "ME", 2);

			
			SelectObject(backdc, oldFont);
		}

		if (exitCheck)
		{
			KillTimer(hWnd, 1);
			PostQuitMessage(0);
		}
		if (!openD)
		{
			openD = TRUE;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DiagProc1);
		}

		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		BitBlt(hdc, 0, 0, wSize.right, wSize.bottom, backdc, 0, 0, SRCCOPY);

		SelectObject(hdc, oldBackBit);
		DeleteDC(backdc);
		EndPaint(hWnd, &ps);
		break; 
	case WM_LBUTTONDOWN:
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DiagProc1);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK DiagProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static char str[10];

	switch (iMsg)
	{
	case WM_INITDIALOG:
		wsprintf(str, "Speed : %d", speed);
		SetDlgItemText(hDlg, IDC_SPEED, str);
		CheckRadioButton(hDlg, IDC_HORSES, IDC_TIGER, IDC_HORSES);
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_JUMP:
			jumpCount = 1;
			jumpCheck = TRUE;
			break;
		case IDC_DOWN:
			sleepCount = 1;
			sleepCheck = TRUE;
			break;
		case IDC_LEFT:
			if (cx > 0)
				cx -= speed;
			break;
		case IDC_RIGHT:
			if (cx < wSize.right - 250)
				cx += speed;
			break;
		case IDC_SUP:
			if (speed <= 50)
				speed += 2;
			wsprintf(str, "Speed : %d", speed);
			SetDlgItemText(hDlg, IDC_SPEED, str);
			break;
		case IDC_SDOWN:
			if (speed >= 6)
				speed -= 2;
			wsprintf(str, "Speed : %d", speed);
			SetDlgItemText(hDlg, IDC_SPEED, str);
			break;
		case IDC_WRITE:
			if (!writeCheck)
				writeCheck = TRUE;
			else
				writeCheck = FALSE;
			break;
		case IDC_HORSES:
			animal = HORSE;
			break;
		case IDC_TIGER:
			animal = TIGER;
			break;
		case IDC_FOLLOW:
			if (!shadeCheck)
				shadeCheck = TRUE;
			else
				shadeCheck = FALSE;
			break;
		case IDC_SHRINK:
			wid -= 10;
			hei -= 5;
			break;
		case IDC_NORMAL:
			wid = 250;
			hei = 170;
			break;
		case IDC_EXPAND:
			wid += 10;
			hei += 5;
			break;
		case IDC_EXIT:
			exitCheck = TRUE;
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}

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

	skyBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SKY));
	gBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GROUND));

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

void Animation_g(HDC backdc, RECT win)
{
	HDC memdc;
	HBITMAP  gBit, oldBit;
	BITMAP bmp;
	static int  gcount;
	int  glen;


	gcount++;
	gcount = gcount % 50;


	gBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GROUND)); // ¶¥

	memdc = CreateCompatibleDC(backdc);


	oldBit = (HBITMAP)SelectObject(memdc, gBit);
	GetObject(gBit, sizeof(BITMAP), &bmp);
	glen = (bmp.bmWidth / 2) / 50;
	StretchBlt(backdc, 0, 600, win.right, win.bottom - 600, memdc, gcount * glen, 0, bmp.bmWidth / 2, bmp.bmHeight, SRCCOPY);

	SelectObject(memdc, oldBit);
	DeleteObject(gBit);
	DeleteDC(memdc);
}

void Animation_Horse(HDC backdc, int xPos, int yPos)
{
	HDC memdc;
	HBITMAP _char[12], _mask[12], hBit, oldBit;
	BITMAP bmp;
	static int count;

	count++;
	count = count % 12;

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
	StretchBlt(backdc, xPos + wid, yPos + (170 - hei), wid*(-1), hei, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);
	SelectObject(memdc, _char[count]);
	GetObject(_char[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos + wid, yPos + (170 - hei), wid*(-1), hei, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);

	for (int i = 0; i < 12; i++)
	{
		DeleteObject(_char[i]);
		DeleteObject(_mask[i]);
	}
	DeleteDC(memdc);
}
void Animation_Tiger(HDC backdc, int xPos, int yPos)
{
	HDC memdc;
	HBITMAP _char[10], _mask[10], hBit, oldBit;
	BITMAP bmp;
	static int count;

	count++;
	count = count % 10;

	_char[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_DRAW_01));
	_char[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_DRAW_02));
	_char[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_DRAW_03));
	_char[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_DRAW_04));
	_char[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_DRAW_05));
	_char[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_DRAW_06));
	_char[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_DRAW_07));
	_char[7] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_DRAW_08));
	_char[8] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_DRAW_09));
	_char[9] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_DRAW_10));

	_mask[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_MASK_01));
	_mask[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_MASK_02));
	_mask[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_MASK_03));
	_mask[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_MASK_04));
	_mask[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_MASK_05));
	_mask[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_MASK_06));
	_mask[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_MASK_07));
	_mask[7] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_MASK_08));
	_mask[8] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_MASK_09));
	_mask[9] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIGER_MASK_10));

	memdc = CreateCompatibleDC(backdc);

	SelectObject(memdc, _mask[count]);
	GetObject(_mask[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos, yPos + 10 + (170 - hei), wid, hei, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);
	SelectObject(memdc, _char[count]);
	GetObject(_char[count], sizeof(BITMAP), &bmp);
	StretchBlt(backdc, xPos, yPos + 10 + (170 - hei), wid, hei, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);

	for (int i = 0; i < 10; i++)
	{
		DeleteObject(_char[i]);
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

	if (shadeCheck)
	{
		SelectObject(memdc, _mask[count]);
		GetObject(_mask[count], sizeof(BITMAP), &bmp);
		StretchBlt(backdc, xPos + 90, yPos, 90, 140, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);
		SelectObject(memdc, _enemy[count]);
		GetObject(_enemy[count], sizeof(BITMAP), &bmp);
		StretchBlt(backdc, xPos + 90, yPos, 90, 140, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);
	}

	for (int i = 0; i < 5; i++)
	{
		DeleteObject(_enemy[i]);
		DeleteObject(_mask[i]);
	}
	DeleteDC(memdc);
}
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