#include <windows.h>
#include <tchar.h>

#define WD_left 1000
#define WD_top 1000

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

int c_x = 0, c_y = 0;

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
	PAINTSTRUCT ps;
	int count;

	switch (uMsg) {
	case WM_CREATE:
		count = 0;
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;

	case WM_KEYDOWN:
		hdc = BeginPaint(hWnd, &ps);
				
		switch (wParam) {
		case VK_LEFT:	if (c_x >= 13) c_x -= 13;
			break;
		case VK_RIGHT:	if (c_x <= 630) c_x += 13;
			break;
		case VK_UP:		if (c_y >= 13) c_y -= 13;
			break;
		case VK_DOWN:	if (c_y <= 630) c_y += 13;
			break;
		}
		
		SetCaretPos(c_x + 8, c_y);
		EndPaint(hWnd, &ps);


		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);
	
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				TextOut(hdc, 13 * j, 13 * i, "a", 1);
			}
		}
		SetCaretPos(c_x + 8, c_y);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}