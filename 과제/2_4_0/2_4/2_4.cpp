#include <windows.h>
#include <tchar.h>
#include <time.h>

#define WD_left 1000
#define WD_top 1000

int R = 0, G = 0, B = 0;

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";


LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow) {

	srand((unsigned)time(NULL));

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

	hWnd = CreateWindow(lpszClass, "window API", WS_OVERLAPPEDWINDOW, 0, 0, WD_left, WD_top, NULL, (HMENU)NULL, hInstance, NULL);

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
	char Input;
	int tra_Input;

	switch (uMsg) {
	case WM_CREATE:
		break;
	case WM_CHAR:
		
		hdc = GetDC(hWnd);

		PAINTSTRUCT ps;
		char txt[10];
		COLORREF text_color;
		
		Input = wParam;

		R = (rand() % 255);
		G = (rand() % 255);
		B = (rand() % 255);
		text_color = RGB(R, G, B);
		
		switch (Input) {
		case '2':	tra_Input = 2;
			break;
		case '3':	tra_Input = 3;
			break;
		case '4':	tra_Input = 4;
			break;
		case '5':	tra_Input = 5;
			break;
		case '6':	tra_Input = 6;
			break;
		case '7':	tra_Input = 7;
			break;
		case '8':	tra_Input = 8;
			break;
		case '9':	tra_Input = 9;
			break;
		}

		SetTextColor(hdc,text_color);
		for (int i = 1; i < 10; i++) {
			
			wsprintf(txt, "%d+%d=%d", tra_Input, i, tra_Input*i);

			TextOut(hdc,  (70 * (tra_Input-1))-50, 10 + (20 * i), txt, strlen(txt));

		}


		EndPaint(hWnd, &ps);
		break;

	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}