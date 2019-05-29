#include <Windows.h>
#include "resource.h"
#include "MyFuncs.h"
#include <stdio.h>

#define MAP_N 4
#define OBJ_N 3

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DiagProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

enum MAP { MAP_NO, MAP_GRASS, MAP_SAND, MAP_OCEAN, MAP_FLOOR };
enum TILE { TILE_NO, TILE_GRASS, TILE_SAND, TILE_OCEAN, TILE_FLOOR, TILE_FLOWER, TILE_ROSE };
enum OBJECT { OBJ_NO, OBJ_BOX, OBJ_ROCK };

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

	hWnd = CreateWindow(lpszClass, "Window API", WS_OVERLAPPEDWINDOW, 0, 0, 816, 839, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

int tWid, tHei;
int mWid, mHei;
int map_select;
int tile_select;
int obj_select;

bool exitCheck, updateCheck, tileCheck, objCheck, collicheck;

FILE* fp;

int mx, my;
static HDC backdc, memdc;
static HBITMAP hBit, backBit;
static BITMAP bmp;
static RECT wSize;

static bool openCheck;
static int tileState[30][30];
static int objState[30][30];
static HBITMAP hMap[MAP_N], hTile[6], hDraw[OBJ_N], hMask[OBJ_N];

static HWND hButton, hList;
static HBITMAP tGrass, tSand, tOcean, tFloor;
static HBITMAP oBox, oRock, oFlower;

char str[5];
static int selection;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HWND hDlg = NULL;
	
	

	switch (uMsg)
	{
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_F1:
			fp = fopen("data.txt", "r");
			fscanf(fp, "%d", &tWid);
			fscanf(fp, "%d", &tHei);
			fscanf(fp, "%d", &mWid);
			fscanf(fp, "%d", &mHei);
			fscanf(fp, "%d", &map_select);
			fscanf(fp, "%d", &tile_select);
			fscanf(fp, "%d", &obj_select);
			fscanf(fp, "%d", &exitCheck);
			fscanf(fp, "%d", &updateCheck);
			fscanf(fp, "%d", &tileCheck);
			fscanf(fp, "%d", &objCheck);
			fscanf(fp, "%d", &collicheck);
			fscanf(fp, "%d", &wSize.bottom);
			fscanf(fp, "%d", &wSize.left);
			fscanf(fp, "%d", &wSize.right);
			fscanf(fp, "%d", &wSize.top);
			fscanf(fp, "%d", &openCheck);
			for (int i = 0; i < 30; i++) {
				for (int j = 0; j < 30; j++) {
					fscanf(fp, "%d", &tileState[i][j]);
				}
			}
			for (int i = 0; i < 30; i++) {
				for (int j = 0; j < 30; j++) {
					fscanf(fp, "%d", &objState[i][j]);
				}
			}
			
			fclose(fp);
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case VK_F2:
			fp = fopen("data.txt", "w");
			fprintf(fp, "%d\n", tWid);
			fprintf(fp, "%d\n", tHei);
			fprintf(fp, "%d\n", mWid);
			fprintf(fp, "%d\n", mHei);
			fprintf(fp, "%d\n", map_select);
			fprintf(fp, "%d\n", tile_select);
			fprintf(fp, "%d\n", obj_select);
			fprintf(fp, "%d\n", exitCheck);
			fprintf(fp, "%d\n", updateCheck);
			fprintf(fp, "%d\n", tileCheck);
			fprintf(fp, "%d\n", objCheck);
			fprintf(fp, "%d\n", collicheck);
			fprintf(fp, "%d\n", wSize.bottom);
			fprintf(fp, "%d\n", wSize.left);
			fprintf(fp, "%d\n", wSize.right);
			fprintf(fp, "%d\n", wSize.top);
			fprintf(fp, "%d\n", openCheck);
			for (int i = 0; i < 30; i++) {
				for (int j = 0; j < 30; j++) {
					fprintf(fp, "%d\n", tileState[i][j]);
				}
			}
			for (int i = 0; i < 30; i++) {
				for (int j = 0; j < 30; j++) {
					fprintf(fp, "%d\n", objState[i][j]);
				}
			}
			fclose(fp);
			break;
		}
		break;

	case WM_CREATE:
		GetClientRect(hWnd, &wSize);
		SetTimer(hWnd, 1, 100, NULL);

		hMap[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GRASS));
		hMap[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SAND));
		hMap[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_OCEAN));
		hMap[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR));

		hTile[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GRASS_TILE));
		hTile[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SAND_TILE));
		hTile[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_OCEAN_TILE));
		hTile[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR_TILE));
		hTile[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOWER_DRAW));
		hTile[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ROCK_DRAW));


		hDraw[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOX_DRAW));
		hDraw[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ROCK_DRAW));
		//hDraw[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOWER_DRAW));

		hMask[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOX_MASK));
		hMask[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ROCK_MASK));
		//hMask[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOWER_MASK));

		mWid = wSize.right, mHei = wSize.bottom;
		tWid = 10; tHei = 10;
		map_select = MAP_NO;

		exitCheck = FALSE;
		openCheck = FALSE;
		updateCheck = FALSE;
		tileCheck = FALSE;
		objCheck = FALSE;
		mx = 0;
		my = 0;
		collicheck = false;
		break;

	case WM_TIMER:
		if (!openCheck)
		{
			openCheck = TRUE;
			hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DiagProc1);
			ShowWindow(hDlg, SW_SHOW);
		}
		if (exitCheck)
		{
			KillTimer(hWnd, 1);
			PostQuitMessage(0);
		}
		if (updateCheck)
		{
			updateCheck = FALSE;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		backdc = CreateCompatibleDC(hdc);
		memdc = CreateCompatibleDC(hdc);
		backBit = CreateCompatibleBitmap(hdc, wSize.right, wSize.bottom);
		SelectObject(backdc, backBit);

		{


			// Draw Background
			if (map_select == MAP_NO)
			{
				Rectangle(backdc, 0, 0, mWid, mHei);

			}
			else
			{
				SelectObject(memdc, hMap[map_select - 1]);
				StretchBlt(backdc, 0, 0, mWid, mHei, memdc, 0, 0, mWid, mHei, SRCCOPY);
			}
			// Draw Grid
			for (int i = 0; i < mWid; i += mWid / tWid) {
				MoveToEx(backdc, i, 0, NULL);
				LineTo(backdc, i, mHei);
			}
			for (int j = 0; j < mHei; j += mHei / tHei) {
				//Rectangle(backdc, i, j, i + mWid / tWid, j + mHei / tHei);
				MoveToEx(backdc, 0, j, NULL);
				LineTo(backdc, mWid, j);
			}

			// Draw Tile and Object
			for (int y = 0, gap; y < tHei; y++)
			{
				for (int x = 0; x < tWid; x++)
				{
					// Tile
					if (tileState[y][x] != TILE_NO)
					{
						gap = mWid / tWid;

						SelectObject(memdc, hTile[tileState[y][x] - 1]);
						GetObject(hTile[tileState[y][x] - 1], sizeof(BITMAP), &bmp);
						StretchBlt(backdc, x * gap, y * gap, gap, gap, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
					}

					// Object
					if (objState[y][x] != OBJ_NO)
					{
						gap = mWid / tWid;
						if (objState[y][x] == OBJ_BOX)
							TextOut(backdc, x * gap, y * gap, "1", 1);
						if (objState[y][x] == OBJ_ROCK)
							TextOut(backdc, x * gap, y * gap, "0", 1);

						SelectObject(memdc, hMask[objState[y][x] - 1]);
						GetObject(hMask[objState[y][x] - 1], sizeof(BITMAP), &bmp);
						StretchBlt(backdc, x * gap, y * gap, gap, gap, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);

						SelectObject(memdc, hDraw[objState[y][x] - 1]);
						GetObject(hDraw[objState[y][x] - 1], sizeof(BITMAP), &bmp);
						StretchBlt(backdc, x * gap, y * gap, gap, gap, memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);
					}

				}
			}



		}

		BitBlt(hdc, 0, 0, wSize.right, wSize.bottom, backdc, 0, 0, SRCCOPY);

		DeleteObject(backBit);
		DeleteDC(memdc);
		DeleteDC(backdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (tileCheck)
		{
			int xPos = mx / (mWid / tWid);
			int yPos = my / (mHei / tHei);

			tileState[yPos][xPos] = tile_select;
			updateCheck = TRUE;
		}
		else if (objCheck)
		{
			int xPos = mx / (mWid / tWid);
			int yPos = my / (mHei / tHei);

			if (objState[yPos][xPos] != OBJ_ROCK)
				objState[yPos][xPos] = obj_select;
			updateCheck = TRUE;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK DiagProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	

	

	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_F1:
			fp = fopen("data2.txt", "r");
			for (int i = 0; i < 5; i++) {
				fscanf(fp, "%d", &str[i]);
			}
			fscanf(fp, "%d", &selection);
			fclose(fp);
			break;

		case VK_F2:
			fp = fopen("data2.txt", "w");
			for (int i = 0; i < 5; i++) {
				fprintf(fp, "%d", str[i]);
			}
			fprintf(fp, "%d", selection);
			fclose(fp);
			break;
		}
		
		
		break;

	case WM_INITDIALOG:
		tGrass = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GRASS_TILE));
		tSand = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SAND_TILE));
		tOcean = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_OCEAN_TILE));
		tFloor = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR_TILE));
		oBox = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOX_DRAW));
		oRock = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ROCK_DRAW));
		oFlower = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOWER_DRAW));

		hButton = GetDlgItem(hDlg, IDC_TILE1);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)tGrass);

		hButton = GetDlgItem(hDlg, IDC_TILE2);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)tSand);

		hButton = GetDlgItem(hDlg, IDC_TILE3);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)tOcean);

		hButton = GetDlgItem(hDlg, IDC_TILE4);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)tFloor);

		hButton = GetDlgItem(hDlg, IDC_TILE5);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)oFlower);

		hButton = GetDlgItem(hDlg, IDC_TILE6);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)oRock);

		// Àå¾Ö¹°
		hButton = GetDlgItem(hDlg, IDC_OBJECT1);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)oBox);
		hButton = GetDlgItem(hDlg, IDC_OBJECT2);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)oRock);


		itoa(mWid, str, 10);
		SetDlgItemText(hDlg, IDC_MAP_WIDTH, str);
		itoa(mHei, str, 10);
		SetDlgItemText(hDlg, IDC_MAP_HEIGHT, str);
		itoa(tWid, str, 10);
		SetDlgItemText(hDlg, IDC_TILE_WIDTH, str);
		itoa(tHei, str, 10);
		SetDlgItemText(hDlg, IDC_TILE_HEIGHT, str);

		hList = GetDlgItem(hDlg, IDC_LIST2);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"Blank");
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"Grass");
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"Sand");
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"Ocean");
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"Floor");

		selection = 0;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_MAP_APPLY:
		{
			// ===== Map Width
			GetDlgItemText(hDlg, IDC_MAP_WIDTH, str, 5);
			if (atoi(str) < 128)
			{
				mWid = 128;
				SetDlgItemText(hDlg, IDC_MAP_WIDTH, "128");
				SetDlgItemText(hDlg, IDC_MAP_HEIGHT, "128");
			}
			else if (atoi(str) > 800)
			{
				mWid = 800;
				SetDlgItemText(hDlg, IDC_MAP_WIDTH, "800");
				SetDlgItemText(hDlg, IDC_MAP_HEIGHT, "800");
			}
			else {
				mWid = mHei = atoi(str);
				SetDlgItemText(hDlg, IDC_MAP_HEIGHT, str);
			}

			updateCheck = TRUE;
			break;
		}
		case IDC_TILE_APPLY:
		{
			// ===== Tile Width
			GetDlgItemText(hDlg, IDC_TILE_WIDTH, str, 5);
			if (atoi(str) < 5)
			{
				tWid = 5;
				SetDlgItemText(hDlg, IDC_TILE_WIDTH, "5");
				SetDlgItemText(hDlg, IDC_TILE_HEIGHT, "5");
			}
			else if (atoi(str) > 30)
			{
				tWid = 30;
				SetDlgItemText(hDlg, IDC_TILE_WIDTH, "30");
				SetDlgItemText(hDlg, IDC_TILE_HEIGHT, "30");
			}
			else
			{
				tWid = tHei = atoi(str);
				SetDlgItemText(hDlg, IDC_TILE_HEIGHT, str);
			}

			updateCheck = TRUE;
			break;
		}
		case IDC_SELECT:
			selection = SendMessage(hList, LB_GETCURSEL, 0, 0);

			map_select = selection;
			updateCheck = TRUE;
			break;
		case IDC_TILE1:
			tile_select = TILE_GRASS;
			tileCheck = TRUE;
			objCheck = FALSE;
			break;
		case IDC_TILE2:
			tile_select = TILE_SAND;
			tileCheck = TRUE;
			objCheck = FALSE;
			break;
		case IDC_TILE3:
			tile_select = TILE_OCEAN;
			tileCheck = TRUE;
			objCheck = FALSE;
			break;
		case IDC_TILE4:
			tile_select = TILE_FLOOR;
			tileCheck = TRUE;
			objCheck = FALSE;
			break;
		case IDC_TILE5:
			tile_select = TILE_FLOWER;
			tileCheck = TRUE;
			objCheck = FALSE;
			break;
		case IDC_TILE6:
			tile_select = TILE_ROSE;
			tileCheck = TRUE;
			objCheck = FALSE;
			break;


		case IDC_OBJECT1:
			obj_select = OBJ_BOX;
			tileCheck = FALSE;
			objCheck = TRUE;
			break;
		case IDC_OBJECT2:
			obj_select = OBJ_ROCK;
			tileCheck = FALSE;
			objCheck = TRUE;
			break;

		case IDC_EXIT:
			exitCheck = TRUE;
			DestroyWindow(hDlg);
			hDlg = NULL;
			break;
		}
		break;
		

	}
	return 0;
}