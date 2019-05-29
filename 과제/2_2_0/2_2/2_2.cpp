#include <windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;


#define WD_left 1000
#define WD_top 1000

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow) {

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

	hWnd = CreateWindow(lpszClass, "window API", WS_OVERLAPPEDWINDOW|WS_THICKFRAME|WS_HSCROLL|WS_VSCROLL, 0, 0, WD_left, WD_top, 0, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;

}



LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int shape = 0;

	switch (uMsg) {

	
	case WM_CHAR:
	
		if (wParam == 'a')
			shape = 1;
		else if (wParam == 'b')
			shape = 2;
		else if (wParam == 'c')
			shape = 3;
		else if (wParam == 'd')
			shape = 4;
		else if (wParam == 'e')
			shape = 5;
		else if (wParam == 'q')
			exit(1);
		
		InvalidateRect(hWnd, NULL, TRUE);
		

		break;

	case WM_CREATE:
		break;

	case WM_PAINT:
		
		hdc = BeginPaint(hWnd, &ps);


		switch (shape) {
		case 1:

			for (int i = 0; i < 40; i++) {
				TextOut(hdc, 10 * i, 10 * i, "*", 1);
				TextOut(hdc, 10 * i, 10 * (40 - i), "*", 1);
			}
			break;

		case 2:

			for (int i = 0; i < 40; i++) {
				for (int j = i; j < 40 - i; j++) {
					TextOut(hdc, 10 * (j), 20 * (i), "*", 1);
				}
			}
			break;

		case 3:

			for (int i = 0; i < 40; i++) {
				for (int j = i; j < 40 - i; j++) {
					TextOut(hdc, 200 - 10 * (i), 10 * (j), "*", 1);
					TextOut(hdc, 200 + 10 * (i), 10 * (j), "*", 1);

				}
			}
			break;

		case 4:

			for (int i = 0; i < 40; i++) {
				for (int j = i; j < 40 - i; j++) {
					TextOut(hdc, 390 - 10 * (i), 10 * (j), "*", 1);
					TextOut(hdc, 10 * (i), 10 * (j), "*", 1);

				}
			}
			break;

		case 5:

			for (int i = 0; i < 40; i++) {
				for (int j = i; j < 40 - i; j++) {
					TextOut(hdc, 15 * (j), 585 - 15 * i, "*", 1);
					TextOut(hdc, 15 * (j), 15 * i, "*", 1);

				}
			}
			break;

		}

		EndPaint(hWnd, &ps);
	break;
	

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


