#include <windows.h>
#include <tchar.h>
#include "resource.h"


#define WD_WIDTH 1000
#define WD_HEIGHT 664
#define Move 10

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

	hWnd = CreateWindow(lpszClass, "window API", WS_OVERLAPPEDWINDOW, 0, 0, WD_WIDTH+17, WD_HEIGHT+40, NULL, (HMENU)NULL, hInstance, NULL);

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
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	static BOOL selection;
	static HBITMAP hbitmap;
	static DWORD hbitmap_word;
	static int hbitmap_move;
	static HBITMAP hBitmap[2][2];
	static DWORD hBitmap_word[2][2];
	static int hBitmap_move[2][2];
	static int count = 0;
	static int hbitmap_mWidth, hbitmap_mHeight,hBitmap_mWidth[2][2],hBitmap_mHeight[2][2];
	static BITMAP bmp;
	static int mx, my, w, h;
	static int Choose,numbering;
	HPEN hpen, oldpen;
	
	switch (uMsg) {
	case WM_CREATE:
		selection = FALSE;
		hbitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				hBitmap[i][j] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
				hBitmap_move[i][j] = 0;
			}
		}
		GetObject(hbitmap, sizeof(BITMAP), &bmp);
		hbitmap_mWidth = bmp.bmWidth;
		hbitmap_mHeight = bmp.bmHeight;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				hBitmap_word[i][j] = SRCCOPY;
		}
		w = 5; h = 5;
		hbitmap_word = SRCCOPY;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		memdc = CreateCompatibleDC(hdc);
		switch (count) {
		case 1:
			SelectObject(memdc, hbitmap);
			StretchBlt(hdc, 0, 0, WD_WIDTH - Move * hbitmap_move * 2, WD_HEIGHT, memdc, Move*hbitmap_move, 0, hbitmap_mWidth - Move * hbitmap_move, hbitmap_mHeight, hbitmap_word);
			StretchBlt(hdc, WD_WIDTH - Move * hbitmap_move * 2, 0, Move * hbitmap_move * 2, WD_HEIGHT, memdc, 0, 0, Move * hbitmap_move, hbitmap_mHeight, hbitmap_word);
			break;
		
		case 2:
			for (int j = 1; j >= 0; j--) {
				for (int i = 1; i >=0; i--) {
					SelectObject(memdc, hBitmap[i][j]);
					StretchBlt(hdc, ((j + 1)*WD_WIDTH / 2) - (Move * hBitmap_move[i][j]), i*WD_HEIGHT / 2, Move*hBitmap_move[i][j], (i / 2 + 1)*WD_HEIGHT / 2, memdc, 0, 0, Move*hBitmap_move[i][j], hbitmap_mHeight, hBitmap_word[i][j]);

					StretchBlt(hdc, (j*WD_WIDTH / 2) , i*WD_HEIGHT / 2, ((j / 2 + 1)*WD_WIDTH / 2)-(Move*hBitmap_move[i][j]), (i / 2 + 1)*WD_HEIGHT / 2, memdc,Move*hBitmap_move[i][j] , 0, hbitmap_mWidth-(Move*hBitmap_move[i][j]), hbitmap_mHeight, hBitmap_word[i][j]);					
				}
			}
			
			break;
		}
		DeleteDC(memdc);
		
		if (selection) {
			hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			oldpen = (HPEN)SelectObject(hdc, hpen);
			for (int i = 0; i < 2; i++) {
				MoveToEx(hdc, (w+i)*WD_WIDTH / 2, (h)*WD_HEIGHT / 2,NULL);
				LineTo(hdc, (w+i)*WD_WIDTH / 2, (h + 1)*WD_HEIGHT / 2);

				MoveToEx(hdc, (w )*WD_WIDTH / 2, (h+i)*WD_HEIGHT / 2, NULL);
				LineTo(hdc, (w + 1 )*WD_WIDTH / 2, (h + i)*WD_HEIGHT / 2);
			}
			SelectObject(hdc, oldpen);
			DeleteObject(hpen);
		}

		EndPaint(hWnd, &ps);
		break;
	
	case WM_KEYDOWN:
		switch (wParam) {
		case 'a': case 'A':
			selection = FALSE;
			Choose = 0;
			count = 1;
			break;

		case '2':
			count = 2;
			break;

		case 'r': case 'R':
			switch (Choose) {
			case 0:
				if (hbitmap_word == SRCCOPY)
					hbitmap_word = NOTSRCCOPY;
				else if (hbitmap_word == NOTSRCCOPY)
					hbitmap_word = SRCCOPY;
				break;

			case 1:
				if (hBitmap_word[0][0] == SRCCOPY)
					hBitmap_word[0][0] = NOTSRCCOPY;
				else if (hBitmap_word[0][0] == NOTSRCCOPY)
					hBitmap_word[0][0] = SRCCOPY;
				break;

			case 2:
				if (hBitmap_word[1][0] == SRCCOPY)
					hBitmap_word[1][0] = NOTSRCCOPY;
				else if (hBitmap_word[1][0] == NOTSRCCOPY)
					hBitmap_word[1][0] = SRCCOPY;
				break;


			case 3:
				if (hBitmap_word[0][1] == SRCCOPY)
					hBitmap_word[0][1] = NOTSRCCOPY;
				else if (hBitmap_word[0][1] == NOTSRCCOPY)
					hBitmap_word[0][1] = SRCCOPY;
				break;

			
			case 4:
				if (hBitmap_word[1][1] == SRCCOPY)
					hBitmap_word[1][1] = NOTSRCCOPY;
				else if (hBitmap_word[1][1] == NOTSRCCOPY)
					hBitmap_word[1][1] = SRCCOPY;
				break;
			}
			break;
			
		case VK_LEFT:
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (hBitmap_move[i][j] == 50)
						hBitmap_move[i][j] = 0;
				}
			}
			if (hbitmap_move == 50)
				hbitmap_move = 0;

			switch (Choose) {
			case 0:
				hbitmap_move++;
				break;
			case 1:
				hBitmap_move[0][0]++;
				break;
			case 2:
				hBitmap_move[1][0]++;
				break;
			case 3:
				hBitmap_move[0][1]++;
				break;
			case 4:
				hBitmap_move[1][1]++;
				break;
			}
			break;
			
		case VK_RIGHT:
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (hBitmap_move[i][j] == 0)
						hBitmap_move[i][j] = 50;
				}
			}
			if (hbitmap_move == 0)
				hbitmap_move = 50;

			switch(Choose){
			case 0:
				hbitmap_move--;
				break;
			case 1:
				hBitmap_move[0][0]--;
				break;
			case 2:
				hBitmap_move[1][0]--;
				break;
			case 3:
				hBitmap_move[0][1]--;
				break;
			case 4:
				hBitmap_move[1][1]--;
				break;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		
		if (count == 2) {
			numbering = 0;
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
		

			
			if (mx >= 0 && mx <= WD_WIDTH / 2 && my >= 0 && my <= WD_WIDTH / 2) {
				Choose = 1;
				selection = TRUE;
			}
			else if (mx >= 0 && mx <= WD_WIDTH / 2 && my >= WD_WIDTH / 2 && my <= WD_WIDTH) {
				Choose = 2;
				selection = TRUE;
			}
			else if (mx >= WD_WIDTH / 2 && mx <= WD_WIDTH && my >= 0 && my <= WD_WIDTH / 2) {
				Choose = 3;
				selection = TRUE;
			}
			else if (mx >= WD_WIDTH / 2 && mx <= WD_WIDTH && my >= WD_WIDTH / 2 && my <= WD_WIDTH) {
				Choose = 4;
				selection = TRUE;
			}
				
				

			switch (Choose) {
			case 0: w = 3; h = 3; break;
			case 1: w = 0; h = 0; break;
			case 2: w = 0; h = 1; break;
			case 3: w = 1; h = 0; break;
			case 4: w = 1; h = 1; break;
			}
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

