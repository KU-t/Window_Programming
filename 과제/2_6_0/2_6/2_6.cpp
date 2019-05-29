#include <windows.h>
#include <tchar.h>

#define WD_left 1500
#define WD_top 300


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

	hWnd = CreateWindow(lpszClass, "window API", WS_OVERLAPPEDWINDOW| WS_HSCROLL | WS_VSCROLL, 0, 0, WD_left, WD_top, NULL, (HMENU)NULL, hInstance, NULL);

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
	SIZE size;
	static int count;
	static int n = 0, m[10] = {}; // 배열자리
	static char array[50][101];
	static char array_2[50][101];
	static int Switch = 0;


	switch (uMsg) {
	case WM_CREATE:
		count = 0;
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;

	case WM_CHAR:
		if (n < 50) {
			if (wParam >= 32 && wParam <= 126) {

				hdc = BeginPaint(hWnd, &ps);
				
			
				if (count <= 99) {
					for (int i = 1; i < count; i++) {
						if (array[n][i] == '\0') {
							array[n][i] = ' ';
						}
					}
					if (Switch == 0) {
						array[n][count++] = wParam;
						array[n][count] = '\0';
					}
					else {
						if (wParam >= 97 && wParam <= 122) {
							array[n][count] = wParam;
							array[n][count++] = array[n][count]-32;
							array[n][count] = '\0';
						}
						else {
							array[n][count++] = wParam;
							array[n][count] = '\0';
						}
					}
				}

				m[n] = count;

				InvalidateRect(hWnd, NULL, TRUE);
				EndPaint(hWnd, &ps);
			}
		}
		
			break;

	case WM_KEYDOWN:
		
		hdc = BeginPaint(hWnd, &ps);

		

			switch (wParam) {
			case VK_RETURN: // 엔터

				if (n < 49) {
					n++;
					m[n] = count;
					count = 0;
				}
				
				break;

			case VK_BACK: // 백스페이스
				//앞에 \0
				if (count > 0) {

					if (array[n][count] == '\0') {
						array[n][--count] = '\0';
					}

					
					else {
						for (int i = count; i < 100; i++) {
							array[n][i - 1] = array[n][i];

						}
						count--;
					}
				}

				//첫글자에서 지울때
				else if (count == 0) {
					if (n > 0)
						count = m[--n];
				}
	
				break;

			case VK_ESCAPE: // ESC
				n = 0;	count = 0;
				for (int i = 0; i < 49; i++) {
					for (int j = 1; j < 99; j++) {
						array[i][0] = '\0';
						array[i][j] = ' ';
					}
				}
					
				break;

			case VK_TAB: // 탭
				
				for (int i = 0; i < 4; i++)
					if(count<100)
					array[n][count++] = ' ';
				break;

			case VK_HOME: // 홈
				count = 0;
				break;

			case VK_DELETE://Del
				count = 0;
				for (int i = 0; i < 49; i++) {
					for (int j = 0; j < 100; j++) {
						array_2[0][j] = ' ';
					}
				}
				for (int i = n; i < 49; i++) {
					for (int j = 0; j < 100; j++) {
						array[i][j] = array_2[i][j];
					}
					wsprintf(array[i], array[i + 1]);
				}
					
				break;

			case VK_LEFT: //왼쪽
				if(count>0)
					count--;
				break;

			case VK_UP: //위쪽
				if(n>0)
					n--;
				break;

			case VK_RIGHT: //오른쪽
				if (count<100)
					count++;
				break;

			case VK_DOWN:// 아래쪽
				if(n<49)
					n++;
				break;

			case VK_CAPITAL: // tab
				if (Switch == 0) 
					Switch = 1;
				else 
					Switch = 0;
				
				break;
			}
		
		InvalidateRect(hWnd, NULL, TRUE);
		EndPaint(hWnd, &ps);

		break;

	case WM_PAINT:
		
		hdc = BeginPaint(hWnd, &ps);
		

		//입력값
		for(int i = 0; i < 10 ; i++)
			TextOut(hdc, 13 , 13 * (i + 1), array[i], strlen(array[i]));
		
		//캐럿출력
		GetTextExtentPoint(hdc, array[n], count, &size);
		if (n < 10) {
			if (count != 0)
				SetCaretPos(size.cx + 13, size.cy + (13 * n));
			else
				SetCaretPos(size.cx + 13, size.cy + (13 * (n + 1)));
		}
		
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