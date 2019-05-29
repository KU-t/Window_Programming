#include <Windows.h>

#define IDC_LH		100
#define IDC_LV		101
#define IDC_LJ		102
#define IDC_RH		103
#define IDC_RV		104
#define IDC_RJ		105
#define IDC_STOP	106
#define IDC_CNG		107
#define IDC_EXIT	108
#define GAP 10

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

enum POS { AA, BB };
enum SHAPE { RECTANGLE, ELLIPSE };
enum DIR { STOP, WIDTH, HEIGHT };

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszClassName = "Child1";
	WndClass.lpfnWndProc = ChildProc1;
	RegisterClassEx(&WndClass);

	WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszClassName = "Child2";
	WndClass.lpfnWndProc = ChildProc2;
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "Window API", WS_OVERLAPPEDWINDOW, 0, 0, 876, 550, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

HBRUSH brush[2];
int shape[2];
RECT rect[2];
int rise[2];
int dir[2];
int jump[2];
int left, right;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND childhWnd1, childhWnd2;
	static HWND b1, b2, b3, b4, b5, b6, b7, b8, b9;
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC backdc, memdc;
	static HBITMAP hBit, backBit;
	static BITMAP bmp;
	static RECT wSize;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &wSize);

		childhWnd1 = CreateWindow("Child1", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 20, 400, 400, hWnd, NULL, g_hInst, NULL);
		childhWnd2 = CreateWindow("Child2", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 440, 20, 400, 400, hWnd, NULL, g_hInst, NULL);

		b1 = CreateWindow("button", "ÁÂ¿ì", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 80, 430, 80, 25, hWnd, (HMENU)IDC_LH, g_hInst, NULL);
		b2 = CreateWindow("button", "»óÇÏ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 180, 430, 80, 25, hWnd, (HMENU)IDC_LV, g_hInst, NULL);
		b3 = CreateWindow("button", "Á¡ÇÁ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 280, 430, 80, 25, hWnd, (HMENU)IDC_LJ, g_hInst, NULL);
		b4 = CreateWindow("button", "ÁÂ¿ì", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 500, 430, 80, 25, hWnd, (HMENU)IDC_RH, g_hInst, NULL);
		b5 = CreateWindow("button", "»óÇÏ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 600, 430, 80, 25, hWnd, (HMENU)IDC_RV, g_hInst, NULL);
		b6 = CreateWindow("button", "Á¡ÇÁ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 700, 430, 80, 25, hWnd, (HMENU)IDC_RJ, g_hInst, NULL);
		b7 = CreateWindow("button", "¸ØÃã", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 290, 470, 80, 25, hWnd, (HMENU)IDC_STOP, g_hInst, NULL);
		b8 = CreateWindow("button", "¹Ù²Þ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 390, 470, 80, 25, hWnd, (HMENU)IDC_CNG, g_hInst, NULL);
		b9 = CreateWindow("button", "EXIT", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 490, 470, 80, 25, hWnd, (HMENU)IDC_EXIT, g_hInst, NULL);

		brush[0] = CreateSolidBrush(RGB(255, 0, 0));
		brush[1] = CreateSolidBrush(RGB(0, 0, 255));
		shape[0] = RECTANGLE;
		shape[1] = ELLIPSE;
		rect[0] = { 0, 0, 20, 20 };
		rect[1] = { 0, 0, 20, 20 };
		rise[0] = 1;
		rise[1] = 1;
		dir[0] = STOP;
		dir[1] = STOP;
		jump[0] = -1;
		jump[1] = -1;

		left = AA; right = BB;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LH:
			dir[left] = WIDTH;
			break;
		case IDC_LV:
			dir[left] = HEIGHT;
			break;
		case IDC_LJ:
			if (jump[left] == -1)
				jump[left] = 0;
			break;
		case IDC_RH:
			dir[right] = WIDTH;
			break;
		case IDC_RV:
			dir[right] = HEIGHT;
			break;
		case IDC_RJ:
			if (jump[right] == -1)
				jump[right] = 0;
			break;
		case IDC_STOP:
			dir[left] = STOP;
			dir[right] = STOP;
			break;
		case IDC_CNG:
			if (left == AA && right == BB)
			{
				left = BB;
				right = AA;
			}
			else if (left == BB && right == AA)
			{
				left = AA;
				right = BB;
			}
			break;
		case IDC_EXIT:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK ChildProc1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC backdc, memdc;
	static HBITMAP hBit, backBit;
	static BITMAP bmp;
	static RECT cSize;
	static int count = 1;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &cSize);
		SetTimer(hWnd, 1, 100, NULL);
		break;
	case WM_TIMER:
	{
		if (jump[left] >= 0)
		{
			jump[left]++;
			if (jump[left] <= 3)
			{
				if (dir[left] == WIDTH || dir[left] == STOP)
				{
					rect[left].top -= 5;
					rect[left].bottom -= 5;
				}
				else if (dir[left] == HEIGHT)
				{
					rect[left].left -= 5;
					rect[left].right -= 5;
				}
			}
			else
			{
				if (dir[left] == WIDTH || dir[left] == STOP)
				{
					rect[left].top += 5;
					rect[left].bottom += 5;
				}
				else if (dir[left] == HEIGHT)
				{
					rect[left].left += 5;
					rect[left].right += 5;
				}
			}

			if (jump[left] == 6)
				jump[left] = -1;
		}

		else if (dir[left] == WIDTH)
		{
			rect[left].left += (rise[left] * GAP);
			rect[left].right += (rise[left] * GAP);

			if (rect[left].right >= cSize.right) rise[left] = -1;
			else if (rect[left].left <= cSize.left) rise[left] = 1;
			rect[left].top += 5 * count;
			rect[left].bottom += 5 * count;

			count *= -1;
		}
		else if (dir[left] == HEIGHT)
		{
			rect[left].top += (rise[left] * GAP);
			rect[left].bottom += (rise[left] * GAP);

			if (rect[left].bottom >= cSize.bottom) rise[left] = -1;
			else if (rect[left].top <= cSize.top) rise[left] = 1;
			rect[left].left += 5 * count;
			rect[left].right += 5 * count;

			count *= -1;
		}


	}

	InvalidateRect(hWnd, NULL, FALSE);
	break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		backBit = CreateCompatibleBitmap(hdc, cSize.right, cSize.bottom);
		backdc = CreateCompatibleDC(hdc);
		SelectObject(backdc, backBit);

		{
			SelectObject(backdc, (HBRUSH)GetStockObject(BLACK_BRUSH));
			Rectangle(backdc, cSize.left, cSize.top, cSize.right, cSize.bottom);

			SelectObject(backdc, brush[left]);
			if (shape[left] == RECTANGLE)
				Rectangle(backdc, rect[left].left, rect[left].top, rect[left].right, rect[left].bottom);
			else if (shape[left] == ELLIPSE)
				Ellipse(backdc, rect[left].left, rect[left].top, rect[left].right, rect[left].bottom);
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
LRESULT CALLBACK ChildProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC backdc, memdc;
	static HBITMAP hBit, backBit;
	static BITMAP bmp;
	static RECT cSize;
	static int count = 1;
	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &cSize);
		SetTimer(hWnd, 1, 100, NULL);
		break;
	case WM_TIMER:
	{

		if (jump[right] >= 0)
		{
			jump[right]++;
			if (jump[right] <= 3)
			{
				if (dir[right] == WIDTH || dir[right] == STOP)
				{
					rect[right].top -= 5;
					rect[right].bottom -= 5;
				}
				else if (dir[right] == HEIGHT)
				{
					rect[right].left -= 5;
					rect[right].right -= 5;
				}
			}
			else
			{
				if (dir[right] == WIDTH || dir[right] == STOP)
				{
					rect[right].top += 5;
					rect[right].bottom += 5;
				}
				else if (dir[right] == HEIGHT)
				{
					rect[right].left += 5;
					rect[right].right += 5;
				}
			}

			if (jump[right] == 6)
				jump[right] = -1;
		}

		else if (dir[right] == WIDTH)
		{
			rect[right].left += (rise[right] * 2 * GAP);
			rect[right].right += (rise[right] * 2 * GAP);

			if (rect[right].right >= cSize.right) rise[right] = -1;
			else if (rect[right].left <= cSize.left) rise[right] = 1;

			rect[right].top += 5 * count;
			rect[right].bottom += 5 * count;

			count *= -1;
		}
		else if (dir[right] == HEIGHT)
		{
			rect[right].top += (rise[right] * 2 * GAP);
			rect[right].bottom += (rise[right] * 2 * GAP);

			if (rect[right].bottom >= cSize.bottom) rise[right] = -1;
			else if (rect[right].top <= cSize.top) rise[right] = 1;
			rect[right].left += 5 * count;
			rect[right].right += 5 * count;

			count *= -1;
		}


	}

	InvalidateRect(hWnd, NULL, FALSE);
	break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		backBit = CreateCompatibleBitmap(hdc, cSize.right, cSize.bottom);
		backdc = CreateCompatibleDC(hdc);
		SelectObject(backdc, backBit);

		{
			SelectObject(backdc, (HBRUSH)GetStockObject(BLACK_BRUSH));
			Rectangle(backdc, cSize.left, cSize.top, cSize.right, cSize.bottom);

			SelectObject(backdc, brush[right]);
			if (shape[right] == RECTANGLE)
				Rectangle(backdc, rect[right].left, rect[right].top, rect[right].right, rect[right].bottom);
			else if (shape[right] == ELLIPSE)
				Ellipse(backdc, rect[right].left, rect[right].top, rect[right].right, rect[right].bottom);
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