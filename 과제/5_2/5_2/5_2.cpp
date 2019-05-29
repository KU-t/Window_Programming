#include <Windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;

	static HBITMAP hBitmap;
	static RECT wSize;
	static BITMAP bmp; // ºñÆ®¸Ê
	static RECT rg;   // reading glasses
	static RECT rg_copy;
	static RECT temp;
	static SIZE tempS;
	int mx, my, oldx, oldy;

	static bool clickCheck;
	static bool rgCheck;
	static bool copyCheck;
	static double zoom;

	static bool Copy_Click;
	static float Copy_x, Copy_y;

	static bool h_button;
	static bool v_button;

	static bool Copy_button;
	switch (uMsg)
	{
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		GetClientRect(hWnd, &wSize);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		Copy_Click = FALSE;
		h_button = FALSE;
		v_button = FALSE;
		Copy_button = FALSE;

		clickCheck = FALSE;
		rgCheck = FALSE;
		copyCheck = FALSE;
		zoom = 1;
		break;


	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);

		SelectObject(memdc, hBitmap);
		GetClientRect(hWnd, &wSize);

		// Draw background image
		StretchBlt(hdc, 0, 0, wSize.right, wSize.bottom, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

		// Draw big image
		if (rgCheck)
		{
			hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Rectangle(hdc, rg.left, rg.top, rg.right, rg.bottom);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);

			if (h_button == FALSE && v_button == FALSE) {
				StretchBlt(hdc, rg.left, rg.top, rg.right - rg.left, rg.bottom - rg.top,
					memdc, rg.left, rg.top, (rg.right - rg.left) / zoom, (rg.bottom - rg.top) / zoom, SRCCOPY);
			}

			else if (h_button == TRUE && v_button == FALSE) {
				StretchBlt(hdc, rg.left, rg.top, rg.right - rg.left, rg.bottom - rg.top,
					memdc, rg.right, rg.top, -(rg.right - rg.left) / zoom, (rg.bottom - rg.top) / zoom, SRCCOPY);
			}

			else if (h_button == FALSE && v_button == TRUE) {
				StretchBlt(hdc, rg.left, rg.top, rg.right - rg.left, rg.bottom - rg.top,
					memdc, rg.left, rg.bottom, (rg.right - rg.left) / zoom, -(rg.bottom - rg.top) / zoom, SRCCOPY);
			}

			else if (h_button == TRUE && v_button == TRUE) {
				StretchBlt(hdc, rg.left, rg.top, rg.right - rg.left, rg.bottom - rg.top,
					memdc, rg.right, rg.bottom, -(rg.right - rg.left) / zoom, -(rg.bottom - rg.top) / zoom, SRCCOPY);
			}

			if (copyCheck)
			{
				if (Copy_Click == TRUE) {
					hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(hdc, hPen);
					Rectangle(hdc, Copy_x, Copy_y, Copy_x + temp.right, Copy_y + temp.bottom );
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);

					StretchBlt(hdc, Copy_x, Copy_y, temp.right, temp.bottom, memdc, temp.left, temp.top, tempS.cx, tempS.cy, SRCCOPY);
					Copy_Click = FALSE;
				}
			}
		}

		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;


	case WM_CHAR:
		if (rgCheck)
		{
			if (wParam == '0')
			{
				zoom = 1;
			}
			else if (wParam == '1')
			{
				zoom = 1.2;
			}
			else if (wParam == '2')
			{
				zoom = 1.5;
			}
			else if (wParam == '3')
			{
				zoom = 2;
			}
			else if (wParam == 'c' || wParam == 'C')
			{
				copyCheck = FALSE;

				temp = { rg.left, rg.top, rg.right - rg.left, rg.bottom - rg.top };
				tempS = { (LONG)((rg.right - rg.left) / zoom), (LONG)((rg.bottom - rg.top) / zoom) };
			}
			else if (wParam == 'p' || wParam == 'P')
			{
				copyCheck = TRUE;
			}
			else if (wParam == 'e' || wParam == 'E')   // Smaller
			{
				if ((rg.right - rg.left > 95) && (rg.bottom - rg.top > 95))
				{
					rg.left += 5;
					rg.right -= 5;
					rg.top += 5;
					rg.bottom -= 5;
				}
			}
			else if (wParam == 's' || wParam == 'S') // Larger
			{
				if ((rg.right - rg.left <= 500) && (rg.bottom - rg.top <= 500))
				{
					rg.left -= 5;
					rg.right += 5;
					rg.top -= 5;
					rg.bottom += 5;
				}
			}
			else if (wParam == 'h' || wParam == 'H')
			{
				if (h_button == FALSE)	h_button = TRUE;
				else if (h_button == TRUE)	h_button = FALSE;

			}
			else if (wParam == 'v' || wParam == 'V')
			{
				if (v_button == FALSE)	v_button = TRUE;
				else if (v_button == TRUE)	v_button = FALSE;

			}
			else if (wParam == 'i' || wParam == 'I'|| wParam == 'j' || wParam == 'J'||
				wParam == 'k' || wParam == 'K' || wParam == 'l' || wParam == 'L' || wParam == 'r' || wParam == 'R')
			{
				
				if (Copy_button == FALSE) {
					Copy_button = TRUE;
					rg_copy.bottom = rg.bottom;
					rg_copy.left = rg.left;
					rg_copy.top = rg.top;
					rg_copy.right = rg.right;
				}

				if (wParam == 'i' || wParam == 'I')				rg.top -= 5;
				else if (wParam == 'j' || wParam == 'J')		rg.left -= 5;
				else if (wParam == 'k' || wParam == 'K')		rg.bottom += 5;
				else if (wParam == 'l' || wParam == 'L')		rg.right += 5;
				else if (wParam == 'r' || wParam == 'R') {
					if (Copy_button = TRUE) {
						Copy_button = FALSE;
						rg.bottom =	rg_copy.bottom;
						rg.left	= rg_copy.left;
						rg.top	=	rg_copy.top;
						rg.right =	rg_copy.right;
					}
				}
			}

			else
				break;

			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;


	case WM_KEYDOWN:
		if (rgCheck)
		{
			if (wParam == VK_LEFT)
			{
				int gap = rg.right - rg.left;

				rg.left -= 10;
				rg.right -= 10;

				if (rg.left <= wSize.left)
				{
					rg.left = 0;
					rg.right = gap;
				}
			}
			else if (wParam == VK_RIGHT)
			{
				int gap = rg.right - rg.left;

				rg.left += 10; rg.right += 10;

				if (rg.right >= wSize.right)
				{
					rg.right = wSize.right;
					rg.left = rg.right - gap;
				}
			}
			else if (wParam == VK_UP)
			{
				int gap = rg.bottom - rg.top;

				rg.top -= 10; rg.bottom -= 10;

				if (rg.top <= wSize.top)
				{
					rg.top = 0;
					rg.bottom = gap;
				}
			}
			else if (wParam == VK_DOWN)
			{
				int gap = rg.bottom - rg.top;

				rg.top += 10; rg.bottom += 10;

				if (rg.bottom >= wSize.bottom)
				{
					rg.bottom = wSize.bottom;
					rg.top = rg.bottom - gap;
				}
			}
			else break;

			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;


	case WM_LBUTTONDOWN:
		if (!clickCheck)
		{
			clickCheck = TRUE;

			rg.left = LOWORD(lParam);
			rg.top = HIWORD(lParam);
			rg.right = rg.left;
			rg.bottom = rg.top;
		}
		break;


	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		memdc = CreateCompatibleDC(hdc);

		if (clickCheck)
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);

			SelectObject(memdc, hBitmap);
			StretchBlt(hdc, 0, 0, wSize.right, wSize.bottom, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

			SetROP2(hdc, R2_MERGEPEN);
			SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH));
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
			Rectangle(hdc, rg.left, rg.top, rg.right, rg.bottom);

			rg.right = mx;
			rg.bottom = my;
		}

		DeleteDC(memdc);
		ReleaseDC(hWnd, hdc);
		break;


	case WM_LBUTTONUP:
		if (clickCheck)
		{
			clickCheck = FALSE;
			rgCheck = TRUE;
		}
		break;


	case WM_RBUTTONDOWN:
		if (Copy_Click == FALSE) {
			Copy_x = LOWORD(lParam);
			Copy_y = HIWORD(lParam);
			Copy_Click = TRUE;
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}