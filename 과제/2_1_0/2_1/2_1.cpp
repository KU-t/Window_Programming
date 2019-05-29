#include <windows.h>
#include <tchar.h>

#define WD_left 800
#define WD_top 600

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

	switch (uMsg) {
	case WM_CREATE:
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		char txt[10];
		int a, b, c;

		COLORREF text_color, bk_color;

		HDC hdc = BeginPaint(hWnd, &ps);

		for (int i = 1; i < 10; i++) {
			for (int j = 2; j < 10; j++) {
				a = rand() % 255;
				b = rand() % 255;
				c = rand() % 255;

				text_color = RGB(a, b, c);
				bk_color = RGB(255 - a, 255 - b, 255 - c);

				wsprintf(txt, "%d*%d=%d", j, i, i*j);
				SetBkColor(hdc, bk_color);

				SetTextColor(hdc, text_color);
				TextOut(hdc, (100 * (j - 2)), (50 * (i - 1)), txt, strlen(txt));


			}
		}
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}