#include <windows.h>
#include <tchar.h>
#include "resource.h"

#define WD_WIDTH 720
#define WD_HEIGHT 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";



class Cell {
public :
	int i, j;	// Cell 이미지 좌표
	bool TF;	// Cell 이미지출력 스위치
};


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
	
	PAINTSTRUCT ps;
	
	HDC hdc, memdc,memdc1;
	

	static HBITMAP hbitmap,oldbitmap,hbitmap1,oldbitmap1;
	static BITMAP bmp;
	static int hbitmap_Width, hbitmap_Height;

	static HPEN hpen, oldpen;

	static int Cell_num_switch;	// Cell 크기를 고르는 스위치
	static Cell c[5][5];	// Cell 이미지 정보 클래스

	static float mx, my;	// 마우스 위치
	static bool Drag;

	static float I, J;		// I - J 선언
	static float Mx, My;	// 드래그할때 마우스 기준 이미지 그릴 좌표

	switch (uMsg) {
	case WM_CREATE:
		hbitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		GetObject(hbitmap, sizeof(BITMAP), &bmp);
		hbitmap_Width = bmp.bmWidth;
		hbitmap_Height = bmp.bmHeight;
		Cell_num_switch = 1;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				c[i][j].i = 10;
				c[i][j].j = 10;
				c[i][j].TF = FALSE;
			}
		}
		Drag = FALSE;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		memdc1 = CreateCompatibleDC(hdc);
		hbitmap1 = CreateCompatibleBitmap(hdc, WD_WIDTH, WD_HEIGHT);

		oldbitmap = (HBITMAP)SelectObject(memdc, hbitmap);
		oldbitmap1 = (HBITMAP)SelectObject(memdc1, hbitmap1);

		


		// 우측 Cell
		for (int i = Cell_num_switch; i < Cell_num_switch*2; i++) {
			for (int j = 0; j < Cell_num_switch; j++) {
				Rectangle(memdc1, i*WD_WIDTH / Cell_num_switch /2, j*WD_HEIGHT / Cell_num_switch, (i + 1)*WD_WIDTH / Cell_num_switch /2, (j+1)*WD_HEIGHT / Cell_num_switch);
			}
		}

		// 좌측 기본 이미지
		//oldbitmap = (HBITMAP)SelectObject(memdc, hbitmap);
		StretchBlt(memdc1, 0, 0, WD_WIDTH/2, WD_HEIGHT, memdc, 0, 0, hbitmap_Width, hbitmap_Height, SRCCOPY);
		//SelectObject(memdc, oldbitmap);
		

		// Cell 이미지
		for (int i = 0; i < Cell_num_switch; i++) {
			for (int j = 0; j < Cell_num_switch; j++) {
				if (c[i][j].TF == TRUE) {
				//	oldbitmap = (HBITMAP)SelectObject(memdc, hbitmap);
					StretchBlt(memdc1, i*WD_WIDTH / Cell_num_switch / 2 + WD_WIDTH/2, j*WD_HEIGHT / Cell_num_switch ,  WD_WIDTH / 2 / Cell_num_switch, WD_HEIGHT / Cell_num_switch, memdc, c[i][j].i * hbitmap_Width / Cell_num_switch, c[i][j].j * hbitmap_Height / Cell_num_switch, hbitmap_Width / Cell_num_switch, hbitmap_Height / Cell_num_switch, SRCCOPY);
				//	SelectObject(memdc, oldbitmap);
				
				}
			}
		}


		// Drag == TRUE 일때 마우스 - 이미지 
		if (Drag == TRUE) {
		//	oldbitmap = (HBITMAP)SelectObject(memdc, hbitmap);
			StretchBlt(memdc1, mx , my , WD_WIDTH/2/Cell_num_switch, WD_HEIGHT/Cell_num_switch, memdc, I*hbitmap_Width / Cell_num_switch, J*hbitmap_Height / Cell_num_switch, hbitmap_Width / Cell_num_switch, hbitmap_Height / Cell_num_switch, SRCCOPY);
		//	SelectObject(memdc, oldbitmap);
			
		}

		//SelectObject(memdc, oldbitmap);
		BitBlt(hdc, 0, 0, WD_WIDTH, WD_HEIGHT, memdc1, 0, 0, SRCCOPY);
		(HBITMAP)SelectObject(memdc1, oldbitmap1);
		DeleteObject(hbitmap1);
		DeleteDC(memdc1);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	
	case WM_KEYDOWN:
		
		// Cell에 보이는 이미지 모두 지우기
		for (int i = 0; i < Cell_num_switch; i++) {
			for (int j = 0; j < Cell_num_switch; j++) {
				c[i][j].TF = FALSE;
			}
		}

		for (int i = 0; i < Cell_num_switch; i++) {
			for (int j = 0; j < Cell_num_switch; j++) {
				c[i][j].i = 10;
				c[i][j].j = 10;
			}
		}

		switch (wParam) {
		case '1': Cell_num_switch = 1;
			break;
		case '2': Cell_num_switch = 2;
			break;
		case '3': Cell_num_switch = 3;
			break;
		case '4': Cell_num_switch = 4;
			break;
		case '5': Cell_num_switch = 5;
			break;
		}

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_LBUTTONDOWN:
		
		if (Drag == FALSE) {

			for (int i = 0; i < Cell_num_switch; i++) {
				for (int j = 0; j < Cell_num_switch; j++) {
					if (mx > i * WD_WIDTH / 2 / Cell_num_switch && mx < (i + 1) * WD_WIDTH / 2 / Cell_num_switch && my > j * WD_HEIGHT / Cell_num_switch && my < (j + 1) * WD_HEIGHT / Cell_num_switch) {
						I = i;		J = j;
						//Mx =   mx - i * WD_WIDTH / 2 / Cell_num_switch;	
						//My =   my - j * WD_WIDTH / Cell_num_switch;
						Drag = TRUE;
					}
				}
			}

			for (int i = 0; i < Cell_num_switch; i++) {
				for (int j = 0; j < Cell_num_switch; j++) {
					if (mx > i * WD_WIDTH / 2 / Cell_num_switch + WD_WIDTH / 2&& mx < (i + 1) * WD_WIDTH / 2 / Cell_num_switch + WD_WIDTH / 2 && my > j * WD_HEIGHT / Cell_num_switch && my < (j + 1) * WD_HEIGHT / Cell_num_switch) {
						I = c[i][j].i;		J = c[i][j].j;		
						c[i][j].TF = FALSE;
						Drag = TRUE;
						c[i][j].i = 10;
						c[i][j].j = 10;
					}
				}
			}
			
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP:
		if (Drag == TRUE) {
			for (int i = 0; i < Cell_num_switch; i++) {
				for (int j = 0; j < Cell_num_switch; j++) {
					if (mx > i * WD_WIDTH / 2 / Cell_num_switch  + WD_WIDTH / 2 && mx < (i + 1) * WD_WIDTH / 2 / Cell_num_switch + WD_WIDTH / 2 && my > j * WD_HEIGHT / Cell_num_switch && my < (j + 1) * WD_HEIGHT / Cell_num_switch) {
						c[i][j].i = I;		c[i][j].j = J;
						c[i][j].TF = TRUE;
					}
				}
			}
			Drag = FALSE;
		}
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

