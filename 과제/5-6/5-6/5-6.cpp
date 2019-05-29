#pragma comment(lib, "msimg32.lib")
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <time.h>
#include <math.h>


#define WD_WIDTH 800
#define WD_HEIGHT 600




class Block {
public:
	int turn;
	int dc_num;
	POINT d;
	
	int p_w, p_h;
	int dx;
	int dy;

	bool TF[4][4];
	HBITMAP bm;

};


BOOL RotateSizingImage(HDC hdc, HBITMAP hBmp,
	double dblAngle,
	int ixRotateAxis, int iyRotateAxis,
	int ixDisplay, int iyDisplay,
	double dblSizeRatio = 1,
	HBITMAP hMaskBmp = NULL, int ixMask = 0, int iyMask = 0);

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

	hWnd = CreateWindow(lpszClass, "window API", WS_OVERLAPPEDWINDOW, 0, 0, WD_WIDTH+15, WD_HEIGHT+38, NULL, (HMENU)NULL, hInstance, NULL);

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

	srand((unsigned)time(NULL));
	HDC hdc, memdc[6], savedc;
	
	PAINTSTRUCT ps;

	static HBITMAP hbitmap[6], oldbitmap[4], sbitmap, oldsbitmap;
	static BITMAP bmp[4];
	static int hbitmap_Width[4], hbitmap_Height[4];

	static int Random;
	static bool TF[11][10];

	static bool Newblock;

	static Block block[100];
	static int block_count;
	switch (uMsg) {

	case WM_CREATE:
		Random = 0;
		block_count = 0;
		hbitmap[0] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hbitmap[1] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		hbitmap[2] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		hbitmap[3] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));

		for (int i = 0; i < 4; i++) {
			GetObject(hbitmap[i], sizeof(BITMAP), &bmp[i]);
			hbitmap_Width[i] = bmp[i].bmWidth;
			hbitmap_Height[i] = bmp[i].bmHeight;
		}

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				TF[i][j] = FALSE;
			}
		}

		for (int i = 0; i < 10; i++)
			TF[11][i] = TRUE;

		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					block[i].TF[j][k] = FALSE;
				}
			}
		}
		Newblock = TRUE;

		

		SetTimer(hWnd, 1, 3000, NULL);		// 제출때 1000으로
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		for (int i = 0; i < 4; i++)
			memdc[i] = CreateCompatibleDC(hdc);
		
		savedc = CreateCompatibleDC(hdc);

		sbitmap = CreateCompatibleBitmap(hdc, WD_WIDTH, WD_HEIGHT);



		for (int i = 0; i < 4; i++)
			oldbitmap[i] = (HBITMAP)SelectObject(memdc[i], hbitmap[i]);


		oldsbitmap = (HBITMAP)SelectObject(savedc, sbitmap);

		Rectangle(savedc, -1, -1, WD_WIDTH, WD_HEIGHT);

		for (int i = 1; i <= 10; i++) {
			for (int j = 1; j <= 10; j++) {
				Rectangle(savedc, i * 32, j * 32, (i + 1) * 32, (j + 1) * 32);
			}
		}

		if (Newblock) {
			Random = rand() % 4;
			//test
			Random = 0;

			block[block_count].turn = 0;

			// 1x4
			if (Random == 0) {
				
				block[block_count].dc_num = 0;
				block[block_count].d = { 192,32 };
				block[block_count].dx = 4;
				block[block_count].p_w = 128;
				block[block_count].p_h = 32;
				block[block_count].bm = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			
				for (int i = block[block_count].dx; i < block[block_count].dx + 4; i++)
					TF[0][i] = TRUE;
			}

			// 2x3
			else if (Random == 1) {
				block[block_count].TF[1][1] = TRUE;
				block[block_count].TF[2][1] = TRUE;
				block[block_count].TF[2][2] = TRUE;
				block[block_count].TF[2][3] = TRUE;
				block[block_count].dc_num = 1;
				block[block_count].d = { 192,32 };
				block[block_count].dx = 5;
				block[block_count].p_w = 96;
				block[block_count].p_h = 64;
				block[block_count].bm = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			}

			//3x2
			else if (Random == 2) {
				block[block_count].TF[0][2] = TRUE;
				block[block_count].TF[1][2] = TRUE;
				block[block_count].TF[2][2] = TRUE;
				block[block_count].TF[2][3] = TRUE;
				block[block_count].dc_num = 2;
				block[block_count].d = { 192,32 };
				block[block_count].dx = 4;
				block[block_count].p_w = 64;
				block[block_count].p_h = 96;
				block[block_count].bm = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
			}

			//4x1
			else if (Random == 3) {
				for (int i = 0; i < 4; i++)
					block[block_count].TF[i][1] = TRUE;
				block[block_count].dc_num = 3;
				block[block_count].d = { 192,32 };
				block[block_count].dx = 4;
				block[block_count].p_w = 32;
				block[block_count].p_h = 128;
				block[block_count].bm = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
			}
			Newblock = FALSE;
		}

		//1x4
		if (block[block_count].dc_num == 0) {		
			if (block[block_count].turn == 0) {

				if (block[block_count].dx <= 6) {
					TransparentBlt(savedc, block[block_count].dx * 32 + 32, block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
						memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
				}

				else if (block[block_count].dx == 7) {
					TransparentBlt(savedc, block[block_count].dx * 32, block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
						memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
				}

				else if (block[block_count].dx == 8) {
					TransparentBlt(savedc, block[block_count].dx * 32 - 32, block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
						memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
				}

				else if (block[block_count].dx == 9) {
					TransparentBlt(savedc, block[block_count].dx * 32 - 64, block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
						memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
				}
			}

			else if (block[block_count].turn == 1) {

				if (block[block_count].dx < 10) {
					RotateSizingImage(savedc, block[block_count].bm, 90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 +48, block[block_count].d.y, 1, NULL, 0, 0);
				}
			}
		}

		//2x3 
		if (block[block_count].dc_num == 1) {		
			if (block[block_count].turn == 0) {

				if (block[block_count].dx <= 7) {
					TransparentBlt(savedc, block[block_count].dx * 32 +32, block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
						memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
				}

				
				else if (block[block_count].dx == 8) {
					TransparentBlt(savedc, block[block_count].dx * 32 , block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
						memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
				}

				else if (block[block_count].dx == 9) {
					TransparentBlt(savedc, block[block_count].dx * 32 - 32, block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
						memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
				}
			}

			else if (block[block_count].turn == 1) {
			
				if (block[block_count].dx == 9) {
					RotateSizingImage(savedc, block[block_count].bm, -90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 32, block[block_count].d.y + 16, 1, NULL, 0, 0);
				}

				else if (block[block_count].dx < 10) {
					RotateSizingImage(savedc, block[block_count].bm, -90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 64, block[block_count].d.y + 16, 1, NULL, 0, 0);
				}
			}

			else if (block[block_count].turn == 2) {

				if (block[block_count].dx == 9) {
				RotateSizingImage(savedc, block[block_count].bm, -180,
					hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
					block[block_count].dx * 32 + 16, block[block_count].d.y + 32, 1, NULL, 0, 0);
				}

				else if (block[block_count].dx == 0 ) {
					RotateSizingImage(savedc, block[block_count].bm, -180,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 80, block[block_count].d.y + 32, 1, NULL, 0, 0);
				}

				else if (block[block_count].dx < 10) {
					RotateSizingImage(savedc, block[block_count].bm, -180,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 48, block[block_count].d.y + 32, 1, NULL, 0, 0);
				}
			}

			else if (block[block_count].turn == 3) {

				if (block[block_count].dx == 9) {
					RotateSizingImage(savedc, block[block_count].bm, 90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 32, block[block_count].d.y + 16, 1, NULL, 0, 0);
				}

				else if (block[block_count].dx < 9) {
					RotateSizingImage(savedc, block[block_count].bm, 90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 64, block[block_count].d.y + 16, 1, NULL, 0, 0);
				}
			}

		}

		//3x2
		if (block[block_count].dc_num == 2) {
			if (block[block_count].turn == 0) {

				if (block[block_count].dx <= 8) {
					TransparentBlt(savedc, block[block_count].dx * 32 + 32, block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
						memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
				}


				/*else if (block[block_count].dx == 8) {
					TransparentBlt(savedc, block[block_count].dx * 32, block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
						memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
				}*/

				else if (block[block_count].dx == 9) {
					TransparentBlt(savedc, block[block_count].dx * 32 , block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
						memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
				}
			}

			else if (block[block_count].turn == 1) {

				if (block[block_count].dx == 9) {
					RotateSizingImage(savedc, block[block_count].bm, -90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 16, block[block_count].d.y + 64, 1, NULL, 0, 0);
				}

				if (block[block_count].dx == 8) {
					RotateSizingImage(savedc, block[block_count].bm, -90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 +48, block[block_count].d.y + 64, 1, NULL, 0, 0);
				}

				else if (block[block_count].dx < 8) {
					RotateSizingImage(savedc, block[block_count].bm, -90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 80, block[block_count].d.y +64, 1, NULL, 0, 0);
				}
			}

			else if (block[block_count].turn == 2) {

				if (block[block_count].dx == 9) {
					RotateSizingImage(savedc, block[block_count].bm, -180,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 +32 , block[block_count].d.y + 48, 1, NULL, 0, 0);
				}

				/*else if (block[block_count].dx == 0) {
					RotateSizingImage(savedc, block[block_count].bm, -180,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 96, block[block_count].d.y + 48, 1, NULL, 0, 0);
				}*/

				else if (block[block_count].dx < 10) {
					RotateSizingImage(savedc, block[block_count].bm, -180,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 64, block[block_count].d.y + 48, 1, NULL, 0, 0);
				}
			}

			else if (block[block_count].turn == 3) {

				/*if (block[block_count].dx == 0) {
					RotateSizingImage(savedc, block[block_count].bm, 90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 80, block[block_count].d.y + 64, 1, NULL, 0, 0);
				}*/

				if (block[block_count].dx == 9) {
					RotateSizingImage(savedc, block[block_count].bm, 90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 16, block[block_count].d.y + 64, 1, NULL, 0, 0);
				}

				else if (block[block_count].dx == 8) {
					RotateSizingImage(savedc, block[block_count].bm, 90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 48, block[block_count].d.y + 64, 1, NULL, 0, 0);
				}

				else if (block[block_count].dx < 8) {
					RotateSizingImage(savedc, block[block_count].bm, 90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 80, block[block_count].d.y + 64, 1, NULL, 0, 0);
				}
			}

		}


		//4x1
		if (block[block_count].dc_num == 3) {		
			if (block[block_count].turn == 0) {
				TransparentBlt(savedc, block[block_count].dx * 32 + 32, block[block_count].d.y, block[block_count].p_w, block[block_count].p_h,
					memdc[block[block_count].dc_num], 0, 0, hbitmap_Width[block[block_count].dc_num], hbitmap_Height[block[block_count].dc_num], RGB(0, 0, 0));
			}

			else if (block[block_count].turn == 1) {
				
				if (block[block_count].dx == 0) {
					RotateSizingImage(savedc, block[block_count].bm, 90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 96, block[block_count].d.y + 112, 1, NULL, 0, 0);
				}

				else if (block[block_count].dx == 1) {
					RotateSizingImage(savedc, block[block_count].bm, 90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 + 64, block[block_count].d.y + 112, 1, NULL, 0, 0);
				}

				else if (block[block_count].dx == 9) {
					RotateSizingImage(savedc, block[block_count].bm, 90,
						hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
						block[block_count].dx * 32 , block[block_count].d.y + 112, 1, NULL, 0, 0);
				}

				else if (block[block_count].dx < 9) {
				RotateSizingImage(savedc, block[block_count].bm, 90,
					hbitmap_Width[block[block_count].dc_num] / 2, hbitmap_Height[block[block_count].dc_num] / 2,
					block[block_count].dx * 32 + 32, block[block_count].d.y + 112, 1, NULL, 0, 0);
				}
			}


		}

		BitBlt(hdc, 0, 0, WD_WIDTH, WD_HEIGHT, savedc, 0, 0, SRCCOPY);
		(HBITMAP)SelectObject(savedc, oldsbitmap);
		DeleteObject(sbitmap);
		DeleteDC(savedc);
		for (int i = 0; i < 4; i++)
			DeleteDC(memdc[i]);
		EndPaint(hWnd, &ps);

		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (block[block_count].dc_num == 0) {
				if (block[block_count].turn == 0) {

				}
			}


			block[block_count].d.y += 32;
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_KEYDOWN:
		
		switch (wParam) {
		
		case VK_DOWN:			// 충돌체크 먼저
			block[block_count].d.y += 32;

			break;

		case VK_SPACE:
			
			// 1x4
			if (block[block_count].dc_num == 0) {		

				if (block[block_count].turn == 0) {
					block[block_count].turn = 1;	
					block[block_count].dy = (block[block_count].d.y / 32) - 1;
				}

				else if (block[block_count].turn == 1) {
					block[block_count].turn = 0;		
					block[block_count].dy = (block[block_count].d.y / 32) + 2;
				}
			}

			//2x3
			else if (block[block_count].dc_num == 1) {

				if (block[block_count].turn == 0) {
					block[block_count].turn = 1;
					block[block_count].dy = (block[block_count].d.y / 32) ;
				}

				else if (block[block_count].turn == 1) {
					block[block_count].turn = 2;
					block[block_count].dy = (block[block_count].d.y / 32) + 1;
				}
				
				else if (block[block_count].turn == 2) {
					block[block_count].turn = 3;
					block[block_count].dy = (block[block_count].d.y / 32) ;
				}

				else if (block[block_count].turn == 3) {
					block[block_count].turn = 0;
					block[block_count].dy = (block[block_count].d.y / 32) + 1;

				}
			}

			//3x2
			else if (block[block_count].dc_num == 2) {


				if (block[block_count].turn == 0) {
					block[block_count].turn = 1;
					block[block_count].dy = (block[block_count].d.y / 32) + 1;

				}

				else if (block[block_count].turn == 1) {
					block[block_count].turn = 2;
					block[block_count].dy = (block[block_count].d.y / 32);

				}

				else if (block[block_count].turn == 2) {
					block[block_count].turn = 3;
					block[block_count].dy = (block[block_count].d.y / 32) + 1;

				}

				else if (block[block_count].turn == 3) {
					block[block_count].turn = 0;
					block[block_count].dy = (block[block_count].d.y / 32);

				}
			}

			//4x1
			else if (block[block_count].dc_num == 3) {

				if (block[block_count].turn == 0) {
					block[block_count].turn = 1;
					block[block_count].dy = (block[block_count].d.y / 32) + 2;

					for (int i = 0; i < 4; i++)
						block[block_count].TF[i][1] = TRUE;
					block[block_count].TF[2][0] = FALSE;
					block[block_count].TF[2][2] = FALSE;
					block[block_count].TF[2][3] = FALSE;
				}

				else if (block[block_count].turn == 1) {
					block[block_count].turn = 0;
					block[block_count].dy = (block[block_count].d.y / 32) -1;

					for (int i = 0; i < 4; i++)
						block[block_count].TF[i][1] = FALSE;
					for (int i = 0; i < 4; i++)
						block[block_count].TF[2][i] = TRUE;				
				}
			}


			break;

		case VK_LEFT:
			
			// 1x4
			if (block[block_count].dc_num == 0) {
				
				if (block[block_count].turn == 0 && block[block_count].dx > 5) {
						block[block_count].dx = 5;
						
						for (int i = 0; i < 4; i++) {
							TF[block[block_count].dy][block[block_count].dx+i] = TRUE;
						}
				}

				else if (block[block_count].dx > 0 && block[block_count].turn == 0) {
					block[block_count].dx -= 1;
					TF[block[block_count].dy][block[block_count].dx] = TRUE;
					TF[block[block_count].dy][block[block_count].dx + 4] = FALSE;
				}

				//미완성
				else if (block[block_count].dx > 0 && block[block_count].turn == 1) {
					block[block_count].dx -= 1;
					//TF[block[block_count].dy][block[block_count].dx] = TRUE;
					//TF[block[block_count].dy][block[block_count].dx + 4] = FALSE;
				}
				
			}

			//2x3
			else if (block[block_count].dc_num == 1) {
				
				if (block[block_count].turn == 2 && block[block_count].dx > 2) {
					block[block_count].dx -= 1;
				}

				else if (block[block_count].turn == 3 && block[block_count].dx == 9) {
					block[block_count].dx = 8; block[block_count].dx -= 1;
				}

				else if (block[block_count].turn == 2 && block[block_count].dx == 9) {
					block[block_count].dx = 8; block[block_count].dx -= 1;
				}

				else if (block[block_count].turn == 1 && block[block_count].dx == 9) {
					block[block_count].dx = 8; block[block_count].dx -= 1;
				}

				else if (block[block_count].turn == 0 && block[block_count].dx > 7) {
					block[block_count].dx = 7; block[block_count].dx -= 1;
				}

				else if (block[block_count].dx > 0) {
					block[block_count].dx -= 1;

				}
			}


			//3x2
			else if (block[block_count].dc_num == 2) {


				if (block[block_count].turn == 0 && block[block_count].dx == 9) {
					block[block_count].dx = 8; block[block_count].dx -= 1;
				}

				else if (block[block_count].turn == 1 && block[block_count].dx == 9) {
					block[block_count].dx = 7; block[block_count].dx -= 1;
				}

				else if (block[block_count].turn == 2 && block[block_count].dx == 9) {
					block[block_count].dx = 8; block[block_count].dx -= 1;
				}

				else if (block[block_count].turn == 3 && block[block_count].dx >= 8) {
					block[block_count].dx = 7; block[block_count].dx -= 1;
				}

				else if (block[block_count].dx > 0) {
					block[block_count].dx -= 1;
				}
			}

			// 4x1
			else if (block[block_count].dc_num == 3) {		
				
				if (block[block_count].dx == 9 && block[block_count].turn == 1) {
							block[block_count].dx = 7;
				}

				else if (block[block_count].dx > 0) {
					block[block_count].dx -= 1;
				}
			}

			break;

		case VK_RIGHT:

			// 1x4
			if (block[block_count].dc_num == 0) {
				if (block[block_count].dx < 9) {
					block[block_count].dx += 1;
				}
			}

			//2x3
			else if (block[block_count].dc_num == 1) {
				
				if (block[block_count].turn == 2 && block[block_count].dx == 8) {		}

				else if (block[block_count].dx == 0 && block[block_count].turn == 2) {
					block[block_count].dx = 2;
				}
			
				else if (block[block_count].dx < 9) {
					block[block_count].dx += 1;
				}
			}
			
			//3x2
			else if (block[block_count].dc_num == 2) {

				/*if (block[block_count].turn == 2 && block[block_count].dx == 8) {}

				else if (block[block_count].dx == 0 && block[block_count].turn == 2) {
					block[block_count].dx = 2;
				}
*/
				if (block[block_count].dx < 9) {
					block[block_count].dx += 1;
				}
			}

			// 4x1
			else if (block[block_count].dc_num == 3) {		
				
				if ((block[block_count].dx == 0 || block[block_count].dx == 1) && block[block_count].turn == 1) {
						block[block_count].dx = 3;
				}
				
				else if (block[block_count].dx < 9) {
					block[block_count].dx += 1;
				}

				

				
			}
				
			
			break;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL RotateSizingImage(HDC hdc, HBITMAP hBmp,
	double dblAngle,
	int ixRotateAxis, int iyRotateAxis,
	int ixDisplay, int iyDisplay,
	double dblSizeRatio ,
	HBITMAP hMaskBmp  , int ixMask , int iyMask ) {

	int i;
	BITMAP bm;
	GetObject(hBmp, sizeof(BITMAP), &bm);
	POINT apt[3] = { 0 };
	double dblWidth = (double)bm.bmWidth*dblSizeRatio;
	double dblHeight = (double)bm.bmHeight*dblSizeRatio;
	double ixRotate = (int)((double)ixRotateAxis*dblSizeRatio); // 크기가 변하는 것 고려
	double iyRotate = (int)((double)iyRotateAxis*dblSizeRatio);
	const double pi = 3.14159265358979323846;

	double dblRadian, dblx, dbly, dblxDest, dblyDest, cosVal, sinVal;
	dblRadian = dblAngle * pi / 180.0f;
	cosVal = cos(dblRadian), sinVal = sin(dblRadian);
	// 1. 회전축을 기준으로 상대좌표를 구하고 
	// 2. 회전후 위치좌표(상대좌표)를 얻은 후
	// 3. 얻은 값을 원래의 좌표에 적용.
	for (i = 0; i<3; i++) {
		if (i == 0) { dblx = -ixRotate, dbly = -iyRotate; }    // left up  꼭지점 부분
		else if (i == 1) { dblx = dblWidth - ixRotate, dbly = -iyRotate; }  // right up 꼭지점 부분
		else if (i == 2) { dblx = -ixRotate, dbly = dblHeight - iyRotate; } // left low 꼭지점 부분
		dblxDest = dblx * cosVal - dbly * sinVal;
		dblyDest = dblx * sinVal + dbly * cosVal;
		dblxDest += ixRotate, dblyDest += iyRotate;
		apt[i].x = ixDisplay - (long)ixRotate + (long)dblxDest;
		apt[i].y = iyDisplay - (long)iyRotate + (long)dblyDest;
	}

	HDC hMemdc;
	HBITMAP hOldBmp;
	hMemdc = CreateCompatibleDC(hdc);
	hOldBmp = (HBITMAP)SelectObject(hMemdc, hBmp);
	BOOL iRes = PlgBlt(hdc, apt, hMemdc, 0, 0, bm.bmWidth, bm.bmHeight, hMaskBmp, ixMask, iyMask);
	SelectObject(hMemdc, hOldBmp);
	DeleteDC(hMemdc);
	return iRes;
}