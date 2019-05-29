#pragma comment(lib, "msimg32.lib")
#include <Windows.h>
#include <stdio.h>
#include "resource.h"

#define Hsize 520
#define Vsize 500

const COLORREF TPColor = RGB(255, 0, 255);
HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow
	(lpszClass, "5-4",
		WS_OVERLAPPEDWINDOW,
		0, 0, Hsize, Vsize, NULL, (HMENU)NULL,
		hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDC, memDC[2], saveDC;
	HBRUSH hBrush, oldBrush;
	BITMAP bmp;
	static HBITMAP hBitmap, oldBitmap, bgBitmap, oldbgBitmap, sBitmap, oldsBitmap;
	static int bmpWidth, bmpHeight;
	static int tx = 0;
	static int ty = 70;
	static int xPos = 330;
	static int  yPos = 270;

	static bool l, r, u, d;
	static int jump = 0;

	static int px, py;

	static int turn = 5;

	static bool tOn = false;
	static bool click = false;
	static bool click2 = false;
	static bool paste = false;

	static int mouseX, mouseY;

	static int bubble;
	static int bubble2;

	static int temp;

	static RECT R;


	switch (iMessage) {
	case WM_CREATE:
	{
		hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));	//¹èÂî
		bgBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));	//¹è°æ
		SetTimer(hWnd, 2, 10, NULL);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		bmpWidth = bmp.bmWidth;
		bmpHeight = bmp.bmHeight;
		break;
	}

	case WM_TIMER:
	{
		switch (wParam)
		{
		case 1:
			++jump;

			if (jump < 50)
			{
				yPos -= 1;
				if (paste)
				{
					R.top -= 1;
				}

			}
			else if (jump > 50)
			{
				yPos += 1;
				if (paste)
				{
					R.top += 1;
				}
			}
			if (jump == 100)
			{
				jump = 0;
				KillTimer(hWnd, 1);
			}

			break;
		case 2:
			if (!click)
			{
				tx += 70;
				if (paste)
				{
					R.right += 70;
					if (R.right > 210)
						R.right = 0;
					if (l)
					{
						R.left -= 1;
					}
					else if (r)
					{
						R.left += 1;
					}
					else if (u)
					{
						R.top -= 1;
					}
					else if (d)
					{
						R.top += 1;
					}
				}
				if (tx > 210)
					tx = 0;
				if (l)
				{
					xPos -= 1;
				}
				else if (r)
				{
					xPos += 1;
				}
				else if (u)
				{
					yPos -= 1;
				}
				else if (d)
				{
					yPos += 1;
				}
			}
			break;
		case 3:
			px += turn;
			py += turn;
			if (px > 50)
				turn *= -1;
			if (px < 0)
			{
				px = 0;
				py = 0;
				turn = 1;
				KillTimer(hWnd, 3);
			}
			break;
		case 4:
			px += turn;
			py += turn;
			if (px < -50)
				turn *= -1;
			if (px > 0)
			{
				px = 0;
				py = 0;
				KillTimer(hWnd, 4);
			}
			break;

		case 5:
			if (mouseX<xPos + 70 && mouseX>xPos&&mouseY<yPos + 70 && mouseY>yPos)
			{
				++bubble;
				ty = 280;
				if (bubble > 20)
				{
					bubble = 0;
					mouseX = 0;
					mouseY = 0;
					if (l)
						ty = 210;
					if (r)   ty = 140;
					if (u) ty = 0;
					if (d) ty = 70;
					KillTimer(hWnd, 5);
				}
			}

			else if (mouseX > R.left && mouseX < R.right && mouseY > R.top && mouseY < R.bottom) {
				++bubble2;
				ty = 280;
				if (bubble2 > 20)
				{
					bubble2 = 0;
					mouseX = 0;
					mouseY = 0;
					if (l)
						ty = 210;
					if (r)   ty = 140;
					if (u) ty = 0;
					if (d) ty = 70;
					KillTimer(hWnd, 5);
				}
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);

		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_LEFT)
		{
			ty = 210;

			l = true;
			r = false;
			u = false;
			d = false;
			if (paste)
			{
				R.bottom = 210;
			}
		}
		else if (wParam == VK_RIGHT)
		{
			ty = 140;

			l = false;
			r = true;
			u = false;
			d = false;
			if (paste)
			{
				R.bottom = 140;
			}
		}
		else if (wParam == VK_UP)
		{
			ty = 0;

			l = false;
			r = false;
			u = true;
			d = false;
			if (paste)
			{
				R.bottom = 0;
			}

		}
		else if (wParam == VK_DOWN)
		{
			ty = 70;

			l = false;
			r = false;
			u = false;
			d = true;
			if (paste)
			{
				R.bottom = 70;
			}

		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	case WM_CHAR:
	{
		char input = wParam;
		switch (input) {

		case 'j':
			SetTimer(hWnd, 1, 10, NULL);
			break;

		case 'e':
			turn = 5;
			SetTimer(hWnd, 3, 10, NULL);
			break;
		case 's':
			turn = -5;
			SetTimer(hWnd, 4, 10, NULL);
			break;
		case 't':
			tOn = true;
			R.left = mouseX;
			R.top = mouseY;
			R.right = tx;
			R.bottom = ty;
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);
		memDC[0] = CreateCompatibleDC(hDC);
		memDC[1] = CreateCompatibleDC(hDC);
		saveDC = CreateCompatibleDC(hDC);

		sBitmap = CreateCompatibleBitmap(hDC, Hsize, Vsize);

		oldBitmap = (HBITMAP)SelectObject(memDC[0], hBitmap);	//¹èÂî
		oldsBitmap = (HBITMAP)SelectObject(saveDC, sBitmap);
		oldbgBitmap = (HBITMAP)SelectObject(memDC[1], bgBitmap);	//¹è°æ

		//SelectObject(memDC, (HBITMAP)hBitmap);

		BitBlt(saveDC, 0, 0, Hsize, Vsize, memDC[1], 0, 0, SRCCOPY);

		TransparentBlt(saveDC, xPos, yPos, px + 70, py + 70, memDC[0], tx, ty, 70, 70, RGB(0, 255, 0));

		if (paste)
		{
			TransparentBlt(saveDC, R.left, R.top, px + 70, py + 70, memDC[0], R.right, R.bottom, 70, 70, RGB(0, 255, 0));
		}

		BitBlt(hDC, 0, 0, Hsize, Vsize, saveDC, 0, 0, SRCCOPY);
		(HBITMAP)SelectObject(saveDC, oldsBitmap);
		DeleteObject(sBitmap);
		DeleteDC(saveDC);
		DeleteDC(memDC[0]);
		DeleteDC(memDC[1]);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		if (!tOn)
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
			if (mouseX<xPos + 70 && mouseX>xPos&&mouseY<yPos + 70 && mouseY>yPos)
				click = true;
			//SetTimer(hWnd, 5, 10, NULL);

			
		}

		else
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
			if (mouseX<xPos + 70 && mouseX>xPos&&mouseY<yPos + 70 && mouseY>yPos) {
				click = true;
				//SetTimer(hWnd, 5, 10, NULL);
			}
				
			
			paste = true;

			if (mouseX > R.left && mouseX < R.left +70 && mouseY > R.top && mouseY < R.top + 70) {
				click2 = true;
				//SetTimer(hWnd, 5, 10, NULL);
			}
			SetTimer(hWnd, 5, 10, NULL);
		}


		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	case WM_MOUSEMOVE:
	{
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		if (click)
		{
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			ty = 280;

		}

		else if (click2)
		{
			R.left = LOWORD(lParam);
			R.top = HIWORD(lParam);
			ty = 280;

		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_LBUTTONUP:
	{
		if (click) {
			click = false;
			if (l)
				ty = 210;
			if (r)   ty = 140;
			if (u) ty = 0;
			if (d) ty = 70;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		
		if (click2) {
			click2 = false;
			if (l)
				ty = 210;
			if (r)   ty = 140;
			if (u) ty = 0;
			if (d) ty = 70;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}