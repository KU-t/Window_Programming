#include <Windows.h>
#include <math.h>
#define RAD(x) 3.14 / 180 * (x)


#define IDC_INSIZE		100
#define IDC_OUTSIZE		101
#define IDC_SMALL		102
#define IDC_NORMAL		103
#define IDC_BIG			104
#define IDC_REVERSE		105
#define IDC_EXIT		106
#define IDC_CIRCLE		107
#define IDC_SIN			108
#define IDC_SPRING		109


HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

enum SHAPE { CIRCLE, SIN, SPRING };
enum DIR { CLOCK = -1, RCLOCK = 1 };
enum CSIZE { SMALL = 15, NORMAL = 30, BIG = 45 };
typedef struct _MYPOINT {
	double x;
	double y;
} MYPOINT;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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

	WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpszClassName = "Child";
	WndClass.lpfnWndProc = ChildProc;
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "Window API", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

double inRad, outRad;
double inDgr, outDgr;
int inddr, outddr;
int dir;
int size;
int Shape;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND childhWnd;
	static HWND scr_outSize;
	static HWND shape_circle, shape_sin, shape_spring;
	static HWND rad_small, rad_normal, rad_big;
	static HWND btn_rev, btn_exit;
	PAINTSTRUCT ps;
	HDC hdc;

	static HDC backdc, memdc;
	static HBITMAP hBit, backBit;
	static BITMAP bmp;
	static RECT wSize;
	static char str[30];

	static int scrPos;
	static RECT inRect;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &wSize);
		scrPos = 5;
		Shape = CIRCLE;


		childhWnd = CreateWindow("Child", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 30, 30, 450, 500, hWnd, NULL, g_hInst, NULL);

		shape_circle = CreateWindow("button", "Circle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 600, 80, 100, 25, hWnd, (HMENU)IDC_CIRCLE, g_hInst, NULL);
		shape_sin = CreateWindow("button", "Sin", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 600, 110, 100, 25, hWnd, (HMENU)IDC_SIN, g_hInst, NULL);
		shape_spring = CreateWindow("button", "Spring", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 600, 140, 100, 25, hWnd, (HMENU)IDC_SPRING, g_hInst, NULL);

		scr_outSize = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 510, 250, 240, 20, hWnd, (HMENU)IDC_INSIZE, g_hInst, NULL);

		rad_small = CreateWindow("button", "Small", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 600, 280, 100, 25, hWnd, (HMENU)IDC_SMALL, g_hInst, NULL);
		rad_normal = CreateWindow("button", "Normal", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 600, 310, 100, 25, hWnd, (HMENU)IDC_NORMAL, g_hInst, NULL);
		rad_big = CreateWindow("button", "Big", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 600, 340, 100, 25, hWnd, (HMENU)IDC_BIG, g_hInst, NULL);

		btn_rev = CreateWindow("button", "Reverse", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 510, 400, 80, 25, hWnd, (HMENU)IDC_REVERSE, g_hInst, NULL);
		btn_exit = CreateWindow("button", "Exit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 650, 400, 80, 25, hWnd, (HMENU)IDC_EXIT, g_hInst, NULL);

		CheckRadioButton(hWnd, IDC_CIRCLE, IDC_SIN, IDC_SPRING);
		CheckRadioButton(hWnd, IDC_SMALL, IDC_BIG, IDC_NORMAL);


		SetScrollRange(scr_outSize, SB_CTL, 1, 10, TRUE);

		SetScrollPos(scr_outSize, SB_CTL, 5, TRUE);

		inRect = { 505, 0, wSize.right, wSize.bottom };
		inRad = 100; outRad = 200;
		inDgr = 0, outDgr = 0;
		inddr = 5, outddr = 5;
		dir = CLOCK;
		size = NORMAL;
		break;

	case WM_HSCROLL:

		if ((HWND)lParam == scr_outSize) scrPos = outddr;

		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			if (scrPos > 1) scrPos--;
			break;
		case SB_LINERIGHT:
			if (scrPos < 10) scrPos++;
			break;
		case SB_PAGELEFT:
			if (scrPos > 1) scrPos--;
			break;
		case SB_PAGERIGHT:
			if (scrPos < 10) scrPos++;
			break;
		case SB_THUMBTRACK:
			scrPos = HIWORD(wParam);
			break;
		}


		if ((HWND)lParam == scr_outSize) outddr = scrPos;
		SetScrollPos((HWND)lParam, SB_CTL, scrPos, TRUE);

		InvalidateRect(hWnd, &inRect, TRUE);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CIRCLE:
			Shape = CIRCLE;
			break;
		case IDC_SIN:
			Shape = SIN;
			outDgr = -200;
			break;
		case IDC_SPRING:
			Shape = SPRING;
			outDgr = -1000;
			break;
		case IDC_SMALL:
			size = SMALL;
			break;
		case IDC_NORMAL:
			size = NORMAL;
			break;
		case IDC_BIG:
			size = BIG;
			break;
		case IDC_REVERSE:
			if (dir == CLOCK) dir = RCLOCK;
			else dir = CLOCK;
			break;
		case IDC_EXIT:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);


		wsprintf(str, "Speed : %d", outddr);
		TextOut(hdc, 600, 230, str, strlen(str));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC backdc, memdc;
	static HBITMAP hBit, backBit;
	static BITMAP bmp;
	static RECT cSize;
	static MYPOINT mp, ip, op;
	static HBRUSH inBrush, outBrush;
	static double xPos, yPos;
	static int rad;
	static double DGR;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &cSize);
		SetTimer(hWnd, 1, 40, NULL);
		inBrush = CreateSolidBrush(RGB(255, 0, 0));
		outBrush = CreateSolidBrush(RGB(0, 0, 255));

		rad = 20;
		DGR = 0;

		mp.x = (cSize.right - cSize.left) / 2;
		mp.y = (cSize.bottom - cSize.top) / 2;


		op = { mp.x + outRad, mp.y };
		break;

	case WM_TIMER:
		hdc = GetDC(hWnd);
		backBit = CreateCompatibleBitmap(hdc, cSize.right, cSize.bottom);

		backdc = CreateCompatibleDC(hdc);
		memdc = CreateCompatibleDC(hdc);

		SelectObject(backdc, backBit);
		SelectObject(memdc, hBit);

		if (Shape == CIRCLE) {
			outDgr += (dir * outddr);


			if (outDgr < 0) outDgr = 360 - outddr;
			else if (outDgr > 360) outDgr = 0 + outddr;

			op = { outRad * cos(RAD(outDgr)), outRad * sin(RAD(outDgr)) };
		}

		else if (Shape == SIN) {

			outDgr += (-dir * outddr);


			if (outDgr  > 200)	outDgr = -200;
			else if (outDgr <= -200) outDgr = 200;

			op = { outDgr,  100 * sin(RAD(outDgr)) };
		}

		else if (Shape == SPRING) {

			outDgr += (-dir * outddr);


			//if (outDgr  > 200)	outDgr = -200;
			//else if (outDgr <= -200) outDgr = 200;

			op = { outDgr / 5 + 100 * cos(RAD(outDgr)),  100 * sin(RAD(outDgr)) };
		}

		DeleteDC(memdc);
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		{
			Rectangle(backdc, cSize.left, cSize.top, cSize.right, cSize.bottom);

			if (Shape == CIRCLE)
				Ellipse(backdc, mp.x - outRad, mp.y - outRad, mp.x + outRad, mp.y + outRad);

			else if (Shape == SIN) {
				for (int i = -1000, dgr = DGR; i <= 1000; i++) {
					yPos = (double)rad * sin(RAD(dgr));

					Ellipse(backdc, i - 1 - 40, cSize.bottom / 2 + 5 * (yPos - 1), i + 1 - 40, cSize.bottom / 2 + 5 * (yPos + 1));

					dgr -= 1;
					if (dgr < 0)
						dgr = 360;
				}
			}

			else if (Shape == SPRING) {
				for (int x = -1000, dgr = DGR - 50; x <= 1000 + 50; x++)
				{
					for (int j = 0; j < 10; j++) {
						xPos = 45 + (double)rad * cos(RAD(dgr - j)) * 5;
						yPos = (double)rad * sin(RAD(dgr - j)) * 5;

						Ellipse(backdc, x + xPos - 1, cSize.bottom / 2 + yPos - 1, x + xPos + 1, cSize.bottom / 2 + yPos + 1);

					}

					dgr += 5;
					if (dgr > 365)
						dgr = 5;
				}
			}


			MoveToEx(backdc, mp.x, cSize.top, NULL);
			LineTo(backdc, mp.x, cSize.bottom);
			MoveToEx(backdc, cSize.left, mp.y, NULL);
			LineTo(backdc, cSize.right, mp.y);

			SelectObject(backdc, outBrush);
			Ellipse(backdc, mp.x + op.x - size, mp.y + op.y - size, mp.x + op.x + size, mp.y + op.y + size);
		}

		BitBlt(hdc, 0, 0, cSize.right, cSize.bottom, backdc, 0, 0, SRCCOPY);

		DeleteObject(backBit);
		DeleteDC(backdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}