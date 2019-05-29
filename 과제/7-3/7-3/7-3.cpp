#pragma comment(lib, "msimg32.lib")
#include <Windows.h>
#include <iostream>
#include <atlimage.h>

#define WIDTH 800
#define HEIGHT 800
#define IDC_BEFORE		100
#define IDC_AFTER		101
#define ID_LISTBOX		102
#define IDC_SELECT		103
#define IDC_FIN				104
#define IDC_EDIT			105
HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

RECT win_size;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM IParam);
LRESULT CALLBACK ChildProc1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void Draw(HDC hdc, HBITMAP hbit, int x, int y, int sizeX, int sizeY);

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

	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszClassName = "Child1";
	WndClass.lpfnWndProc = ChildProc1;
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "7-3", WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

int count;
CImage img[10];


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HWND childhWnd1;
	static HWND b1, b2, b3, b4, hList, hEdit;

	static int select;
	static char name[10];
	static char num[10];
	static bool TF[10];
	static int TF_count;
	switch (iMessage)
	{

	case WM_CREATE:
		childhWnd1 = CreateWindow("Child1", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 20, 600, 600, hWnd, NULL, g_hInst, NULL);
		b1 = CreateWindow("button", "<-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 630, 80, 25, hWnd, (HMENU)IDC_BEFORE, g_hInst, NULL);
		b2 = CreateWindow("button", "->", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 630, 80, 25, hWnd, (HMENU)IDC_AFTER, g_hInst, NULL);
		b3 = CreateWindow("button", "select", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 640, 630, 100, 25, hWnd, (HMENU)IDC_SELECT, g_hInst, NULL);
		b4 = CreateWindow("button", "complete", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 640, 700, 100, 25, hWnd, (HMENU)IDC_FIN, g_hInst, NULL);
		hList = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD, 640, 20, 100, 600, hWnd, (HMENU)ID_LISTBOX, g_hInst, NULL);
		hEdit = CreateWindow("edit", "1번", WS_CHILD | WS_VISIBLE | WS_BORDER, 225, 700, 100, 25, hWnd, (HMENU)IDC_EDIT, g_hInst, NULL);
		TF_count = 0;
		
		for (int i = 0; i <= 9; ++i)
		{
			TF[i] = FALSE;
			char a[10];
			sprintf_s(a, "%d번", i+1);
			SendMessage(hList, LB_INSERTSTRING, i, (LPARAM)a);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BEFORE:
			if (TF_count > 1)
				TF_count--;
			if (count > 0)
				--count;
			sprintf_s(num, "%d번", count + 1);
			SetDlgItemText(hWnd, IDC_EDIT, LPCSTR(num));
			break;
		case IDC_AFTER:
			if (TF_count < 9)
				TF_count++;
			if (count < 9)
				++count;
			sprintf_s(num, "%d번", count + 1);
			SetDlgItemText(hWnd, IDC_EDIT, LPCSTR(num));
			break;
		case IDC_SELECT:
			if (TF[TF_count] == FALSE) {
				img[count].Load(TEXT(name));
				TF[TF_count] = TRUE;
			}
			
			break;
		case IDC_FIN:
			EnableWindow(b3, FALSE);
			break;
		case ID_LISTBOX:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				if (HIWORD(wParam) == LBN_SELCHANGE)
				{
					select = SendMessage(hList, LB_GETCURSEL, 0, 0);
					sprintf_s(name, "%d.jpg", select+1);
				}
				break;
			}
			break;
		}
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


LRESULT CALLBACK  ChildProc1(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, memDC;
	HBITMAP hBitmap;


	switch (iMessage)
	{

	case WM_CREATE:
		GetClientRect(hWnd, &win_size);
		SetTimer(hWnd, 1, 1000 / 60, NULL);
		// 70*70 위 아래 오 왼
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			InvalidateRect(hWnd, NULL, false);
			break;
		}
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, win_size.right, win_size.bottom);
		SelectObject(memDC, (HBITMAP)hBitmap);
		PatBlt(memDC, win_size.left, win_size.top, win_size.right, win_size.bottom, WHITENESS);
		//---------------------------------------------------------------------------------
		if (!img[count].IsNull())
			img[count].Draw(memDC, 0, 0, 600, 600);
		//---------------------------------------------------------------------------------
		BitBlt(hDC, 0, 0, win_size.right, win_size.bottom, memDC, 0, 0, SRCCOPY);
		DeleteDC(memDC);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}