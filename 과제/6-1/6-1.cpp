#include <Windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

enum COLOR { RED, GREEN };
enum SHAPE { NO, CIRCLE, RECTANGLE };
enum DIR { LEFT = -1, RIGHT = 1 };

bool w_diagBoundCheck;
int w_color;
int w_shape;

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static RECT wSize;
	static RECT w_rect;
	static int w_diagDir;

	static HPEN hPen[2], oldPen;
	static HBRUSH hBrush[2], oldBrush;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &wSize);
		SetTimer(hWnd, 1, 50, NULL);
		w_rect = { 100, 100, 150, 150 };
		w_diagDir = RIGHT;

		hPen[RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		hPen[GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		hBrush[RED] = CreateSolidBrush(RGB(255, 0, 0));
		hBrush[GREEN] = CreateSolidBrush(RGB(0, 255, 0));
		break;
	case WM_TIMER:
		if (w_diagBoundCheck && wParam == 1)
		{
			w_rect.top += (10 * w_diagDir);
			w_rect.bottom += (10 * w_diagDir);

			if (w_rect.bottom >= wSize.bottom)
				w_diagDir = LEFT;
			else if (w_rect.top <= wSize.top)
				w_diagDir = RIGHT;

			if (w_rect.right <= 200) {
				w_rect.right += 10;
				w_rect.left += 10;
			}

			else if (w_rect.left >= 100) {
				w_rect.right -= 10;
				w_rect.left -= 10;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (w_diagBoundCheck)
		{
			oldPen = (HPEN)SelectObject(hdc, hPen[w_color]);
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush[w_color]);
			if (w_shape == CIRCLE)
				Ellipse(hdc, w_rect.left, w_rect.top, w_rect.right, w_rect.bottom);
			else if (w_shape == RECTANGLE)
				Rectangle(hdc, w_rect.left, w_rect.top, w_rect.right, w_rect.bottom);
			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);
		}

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
	HWND hBtn;
	PAINTSTRUCT ps;
	static HDC whdc, dhdc, backdc;
	static HBITMAP backBit;
	static RECT dSize;

	static HPEN hPen[2], oldPen;
	static HBRUSH hBrush[2], oldBrush;

	static RECT d_rect;
	static bool d_diagBoundCheck;
	static int d_color;
	static int d_shape;
	static int d_diagDir;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		GetClientRect(hDlg, &dSize);

		hBtn = GetDlgItem(hDlg, IDC_BUTTON_DSTOP);
		EnableWindow(hBtn, FALSE);
		hBtn = GetDlgItem(hDlg, IDC_BUTTON_WSTOP);
		EnableWindow(hBtn, FALSE);

		d_diagBoundCheck = FALSE;
		w_diagBoundCheck = FALSE;

		hPen[RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		hPen[GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		hBrush[RED] = CreateSolidBrush(RGB(255, 0, 0));
		hBrush[GREEN] = CreateSolidBrush(RGB(0, 255, 0));

		d_rect = { 200, 150, 230, 180 };
		d_diagDir = RIGHT;
		return true;
	case WM_TIMER:
		if (wParam == 1 && d_diagBoundCheck)
		{
			d_rect.left += (10 * d_diagDir);
			d_rect.right += (10 * d_diagDir);

			if (d_rect.right >= 580)
				d_diagDir = LEFT;
			else if (d_rect.left <= 0)
				d_diagDir = RIGHT;

			if (d_rect.bottom <= 200) {
				d_rect.bottom += 10;
				d_rect.top += 10;
			}

			else if (d_rect.top >= 100) {
				d_rect.bottom -= 10;
				d_rect.top -= 10;
			}
				

			InvalidateRect(hDlg, NULL, TRUE);
		}
		break;
	case WM_PAINT:
		dhdc = BeginPaint(hDlg, &ps);
		//whdc = BeginPaint(hWnd, &ps);
		backdc = CreateCompatibleDC(dhdc);
		backBit = CreateCompatibleBitmap(dhdc, dSize.right, dSize.bottom);

		SelectObject(backdc, backBit);

		//Rectangle(hdc, 10, 100, 570, 350);

		if (d_diagBoundCheck)
		{
			oldPen = (HPEN)SelectObject(dhdc, hPen[d_color]);
			oldBrush = (HBRUSH)SelectObject(dhdc, hBrush[d_color]);
			if (d_shape == CIRCLE)
				Ellipse(dhdc, d_rect.left, d_rect.top, d_rect.right, d_rect.bottom);
			else if (d_shape == RECTANGLE)
				Rectangle(dhdc, d_rect.left, d_rect.top, d_rect.right, d_rect.bottom);
			SelectObject(dhdc, oldPen);
			SelectObject(dhdc, oldBrush);
		}

		BitBlt(dhdc, 0, 0, dSize.right, dSize.bottom, dhdc, 0, 0, SRCCOPY);

		DeleteObject(backBit);
		DeleteDC(backdc);
		EndPaint(hDlg, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_DSTART:
			hBtn = GetDlgItem(hDlg, IDC_BUTTON_DSTART);
			EnableWindow(hBtn, FALSE);
			hBtn = GetDlgItem(hDlg, IDC_BUTTON_DSTOP);
			EnableWindow(hBtn, TRUE);
			SetTimer(hDlg, 1, 50, NULL);

			d_diagBoundCheck = TRUE;
			break;
		case IDC_BUTTON_DSTOP:
			hBtn = GetDlgItem(hDlg, IDC_BUTTON_DSTOP);
			EnableWindow(hBtn, FALSE);
			hBtn = GetDlgItem(hDlg, IDC_BUTTON_DSTART);
			EnableWindow(hBtn, TRUE);
			
			d_diagBoundCheck = FALSE;
			break;
		case IDC_BUTTON_DEXIT:
			KillTimer(hDlg, 1);
			EndDialog(hDlg, 0);
			break;
		case IDC_BUTTON_WSTART:
			hBtn = GetDlgItem(hDlg, IDC_BUTTON_WSTART);
			EnableWindow(hBtn, FALSE);
			hBtn = GetDlgItem(hDlg, IDC_BUTTON_WSTOP);
			EnableWindow(hBtn, TRUE);
			
			w_diagBoundCheck = TRUE;
			break;
		case IDC_BUTTON_WSTOP:
			hBtn = GetDlgItem(hDlg, IDC_BUTTON_WSTOP);
			EnableWindow(hBtn, FALSE);
			hBtn = GetDlgItem(hDlg, IDC_BUTTON_WSTART);
			EnableWindow(hBtn, TRUE);
			
			w_diagBoundCheck = FALSE;
			break;
		case IDC_BUTTON_WEXIT:
			exit(1);
			break;
		case IDC_RADIO_CIRCLE:
			d_shape = w_shape = CIRCLE;
			break;
		case IDC_RADIO_RECT:
			d_shape = w_shape = RECTANGLE;
			break;
		case IDC_RADIO_RED:
			d_color = w_color = RED;
			break;
		case IDC_RADIO_GREEN:
			d_color = w_color = GREEN;
			break;
		}
		break;
	}
	return 0;
}