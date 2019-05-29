#include <Windows.h>
#include <math.h>
#include "resource.h"

#define RAD(x) 3.14 / 180 * (x)

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

enum GRAPH { NO, SIN, ZIG, SPRING };

int r, g, b;
int graph;
bool exitCheck;
bool flowCheck;
bool varCheck;
bool revCheck;
int Check;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DiagProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


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

	hWnd = CreateWindow(lpszClass, "Window API", WS_OVERLAPPEDWINDOW, 0, 0, 500, 300, NULL, (HMENU)NULL, hInstance, NULL);

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
	HPEN hPen, oldPen;
	static HDC backdc;
	static HBITMAP backBit;
	static RECT wSize;
	


	double xPos, yPos;
	static int mx, my;
	static int rad;
	static int varDir;
	static double DGR;
	static double x, y;
	static bool openD;
	static int count;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &wSize);
		SetTimer(hWnd, 1, 50, NULL);
		
		Check = 0;
		count = 0;

		mx = wSize.right / 2;
		my = wSize.bottom / 2;
		r = 0, g = 0, b = 0;
		DGR = 0;
		varDir = 1;
		rad = 20;
		exitCheck = FALSE;
		flowCheck = FALSE;
		varCheck = FALSE;
		revCheck = FALSE;
		openD = FALSE;
		break;

	case WM_TIMER:
		hdc = GetDC(hWnd);
		backBit = CreateCompatibleBitmap(hdc, wSize.right, wSize.bottom);
		backdc = CreateCompatibleDC(hdc);
		SelectObject(backdc, backBit);

		// Draw Background
		Rectangle(backdc, -10, -10, wSize.right + 10, wSize.bottom + 10);
		/*MoveToEx(backdc, 0, my, NULL);
		LineTo(backdc, wSize.right, my);
		MoveToEx(backdc, mx, 0, NULL);
		LineTo(backdc, mx, wSize.bottom);*/

		{
			if (graph != NO)
			{
				if (!revCheck) hPen = CreatePen(PS_SOLID, 3, RGB(r, g, b));
				else hPen = CreatePen(PS_SOLID, 3, RGB(255 - r, 255 - g, 255 - b));
				oldPen = (HPEN)SelectObject(backdc, hPen);

				if (graph == SIN)
				{
					for (int x = wSize.right, dgr = DGR; x >= wSize.left - mx; x--)
					{
						yPos = (double)rad * sin(RAD(dgr));

						Ellipse(backdc, x - 1, my + yPos - 1, x + 1, my + yPos + 1);

						dgr -= 5;
						if (dgr < 0)
							dgr = 360;
					}
				}


				else if (graph == ZIG)
				{
					
					for (int x = wSize.right, dgr = DGR; x >= wSize.left - mx; x--)
					{
						yPos = 2 * (dgr % 72 - 18);
						
						Ellipse(backdc, x - 1, my + yPos - 1, x + 1, my + yPos + 1);

						dgr -= 1;
						if (dgr < 0)
							dgr = 360;
					}



					for (int x = wSize.right, dgr = DGR; x >= wSize.left - mx; x--)
					{
						yPos = 2 * (-dgr % 72 + 54);

						Ellipse(backdc, x - 1, my + yPos - 1, x + 1, my + yPos + 1);

						dgr -= 1;
						if (dgr < 0)
							dgr = 360;
					}

					hPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
					oldPen = (HPEN)SelectObject(backdc, hPen);

					Rectangle(backdc, 0, my + 37, wSize.right + 10, wSize.bottom + 10);

					SelectObject(backdc, oldPen);
					DeleteObject(hPen);

					if (Check == 1)
						count-=5;
					else if (Check == 2)
						count+=5;
				}


				else if (graph == SPRING)
				{
					for (int x = -50, dgr = DGR - 50; x <= wSize.right + 50; x++)
					{
						xPos = (double)rad * cos(RAD(dgr));
						yPos = (double)rad * sin(RAD(dgr));

						Ellipse(backdc, x + xPos - 1, my + yPos - 1, x + xPos + 1, my + yPos + 1);

						dgr += 5;
						if (dgr > 365)
							dgr = 5;
					}
				}
				      
				SelectObject(backdc, oldPen);
				DeleteObject(hPen);

				if (flowCheck)
				{
					DGR += 5;
					if (DGR == 365) DGR = 0;
				}
				if (varCheck)
				{
					rad += (1 * varDir);

					if (rad > 50)
						varDir = -1;
					else if (rad < 10)
						varDir = 1;

					
					if (count== 50)
						Check = 1;
					else if (count == 0)
						Check = 2;
				}
			}
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

		if (graph == SIN || graph == SPRING) {
			Rectangle(hdc, -10, -10, wSize.right + 10, wSize.bottom + 10);
			BitBlt(hdc, 0, 0, wSize.right, wSize.bottom, backdc, 0, 0, SRCCOPY);
		}

		else if (graph == ZIG)
			StretchBlt(hdc, 0, 50+ count, wSize.right, wSize.bottom - 2 * count, backdc, 0, 0, wSize.right, wSize.right, SRCCOPY);
			//BitBlt(hdc, 0, count, wSize.right, wSize.bottom - count, backdc, 0, 0, SRCCOPY);

		DeleteDC(backdc);
		EndPaint(hWnd, &ps);
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

	switch (iMsg)
	{
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_Flow:
			flowCheck = TRUE;
			break;
		case IDC_VAR:
			varCheck = TRUE;
			Check = 1;
			break;
		case IDC_STOP:
			flowCheck = FALSE;
			varCheck = FALSE;
			Check = 0;
			break;
		case IDC_RESET:
			flowCheck = FALSE;
			varCheck = FALSE;
			graph = NO;
			Check = 0;
			break;
		case IDC_EXIT:
			exitCheck = TRUE;
			EndDialog(hDlg, 0);
			break;
		case IDC_SIN:
			graph = SIN;
			break;
		case IDC_COS:
			graph = ZIG;
			break;
		case IDC_SPRING:
			graph = SPRING;
			break;
		case IDC_Red:
			if (r == 0)
				r = 255;
			else if (r == 255)
				r = 0;
			break;
		case IDC_GREEN:
			if (g == 0)
				g = 255;
			else if (g == 255)
				g = 0;
			break;
		case IDC_BLUE:
			if (b == 0)
				b = 255;
			else if (b == 255)
				b = 0;
			break;
		case IDC_REVERT:
			if (!revCheck)
				revCheck = TRUE;
			else
				revCheck = FALSE;
			break;
		}
		break;
	}
	return 0;
}