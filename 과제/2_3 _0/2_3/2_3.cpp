#include <windows.h>
#include <tchar.h>

#define WD_left 1000
#define WD_top 1000

int array_num = 0;

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
	HDC hdc;
	PAINTSTRUCT ps;
	COLORREF color;
	char array[26];
	char Input;
	int Output = 0;
	static int ar[2][1000];
	int x, y;


	color = RGB(70 + array_num, 70+ array_num , 70 + array_num);
	

	// 배열 초기화;
	for (int i = 65; i < 91; i++)
		array[i - 65] = i;
	for(int j = 0; j<2;j++) {
		for (int i = 0; i < 1000; i++)
			ar[j][i] = 0;
	}
	

	switch (uMsg) {
	case WM_CREATE:
		break;
	
	case WM_CHAR:
		
		hdc = GetDC(hWnd);
		Input = wParam;


		if ((wParam == 'q') || (wParam == 'Q')) 
			exit(1);

		//입력-배열 동일지점
		for (int i = 0; i < 26; i++) {
			if (array[i] == Input)
				Output = i;
		}

		//배열 출력
		x = rand() % 800;
		y = rand() % 600;
		ar[0][x] = 1;
		ar[1][y] = 1;
		//if (!(ar[0][x] == 1 && ar[1][y] == 1)) 
			SetBkColor(hdc, color);
			TextOut(hdc, x, y, array + Output, 26 - Output);
			if(array_num < 180) array_num += 2;

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