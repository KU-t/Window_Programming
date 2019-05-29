#include <Windows.h>
#include <time.h>
#include "resource.h"

#define X 900
#define Y 950

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

enum XDIR { XNO, LEFT, RIGHT };
enum YDIR { YNO, TOP, BOTTOM };

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "5-7", WS_OVERLAPPEDWINDOW, 0, 0, X, Y, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	static HDC backdc, memdc;
	static HBITMAP hBit, backBit;
	static BITMAP bmp;
	static HPEN boardPen;
	static RECT wSize;
	static RECT board;
	static POINT p, bp;

	int mx, my; // 마우스값

	static int tile[5][5];
	static int wlen, blen, div;
	static bool fullCheck;
	static bool gameCheck; // s누르면 게임시작 알리는 스위치
	static bool moveCheck;
	static bool initCheck;

	static bool moving;
	static int xdir, ydir;
	static int moveCount;
	static int move;

	switch (uMsg)
	{
	case WM_CREATE:
		hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		GetClientRect(hWnd, &wSize);
		GetObject(hBit, sizeof(BITMAP), &bmp);

		board = { 40, 40, 840, 840 }; // 사진이 윈도우창 중앙에 오도록 값 지정

		boardPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		div = 3; // 처음 시작 3등분

		fullCheck = FALSE;
		gameCheck = FALSE;
		moveCheck = FALSE;
		initCheck = FALSE;
		moving = FALSE;

		moveCount = 0;
		move = 0;
		p = { -1, -1 };
		break;

	case WM_TIMER:
		if (moveCheck || fullCheck)
		{
			hdc = GetDC(hWnd);
			backBit = CreateCompatibleBitmap(hdc, wSize.right, wSize.bottom);

			backdc = CreateCompatibleDC(hdc);
			memdc = CreateCompatibleDC(hdc);

			SelectObject(backdc, backBit);
			SelectObject(memdc, hBit);

			{
				SelectObject(backdc, (HBRUSH)GetStockObject(GRAY_BRUSH));
				Rectangle(backdc, 0, 0, wSize.right, wSize.bottom);

				SelectObject(backdc, boardPen);
				Rectangle(backdc, board.left, board.top, board.right, board.bottom);

				for (int i = 0; i < div; i++)
					for (int j = 0; j < div; j++)
					{
						if (tile[i][j] == -1) continue;
						if (p.x == j && p.y == i) continue;
						else
						{
							int y = tile[i][j] / 10;
							int x = tile[i][j] % 10;

							StretchBlt(backdc, board.left + (wlen * j), board.top + (wlen * i), wlen, wlen,
								memdc, blen * x, blen * y, blen, blen, SRCCOPY);
						}
					}

				moveCheck = FALSE;
				if (moving && !fullCheck)
				{
					moveCheck = TRUE;
					move++;

					if (xdir == LEFT)
					{
						StretchBlt(backdc, board.left + (wlen * p.x) - (move * 4), board.top + (wlen * p.y), wlen, wlen,
							memdc, blen * bp.x, blen * bp.y, blen, blen, SRCCOPY);
					}
					else if (xdir == RIGHT)
					{
						StretchBlt(backdc, board.left + (wlen * p.x) + (move * 4), board.top + (wlen * p.y), wlen, wlen,
							memdc, blen * bp.x, blen * bp.y, blen, blen, SRCCOPY);
					}
					else if (ydir == TOP)
					{
						StretchBlt(backdc, board.left + (wlen * p.x), board.top + (wlen * p.y) - (move * 4), wlen, wlen,
							memdc, blen * bp.x, blen * bp.y, blen, blen, SRCCOPY);
					}
					else if (ydir == BOTTOM)
					{
						StretchBlt(backdc, board.left + (wlen * p.x), board.top + (wlen * p.y) + (move * 4), wlen, wlen,
							memdc, blen * bp.x, blen * bp.y, blen, blen, SRCCOPY);
					}

					if (move == moveCount)
					{
						moving = FALSE;
						moveCheck = FALSE;

						if (xdir == LEFT)
						{
							tile[p.y][p.x - 1] = tile[p.y][p.x];
							tile[p.y][p.x] = -1;
						}
						else if (xdir == RIGHT)
						{
							tile[p.y][p.x + 1] = tile[p.y][p.x];
							tile[p.y][p.x] = -1;
						}
						else if (ydir == TOP)
						{
							tile[p.y - 1][p.x] = tile[p.y][p.x];
							tile[p.y][p.x] = -1;
						}
						else if (ydir == BOTTOM)
						{
							tile[p.y + 1][p.x] = tile[p.y][p.x];
							tile[p.y][p.x] = -1;
						}
					}
				}

				if (fullCheck)
				{
					StretchBlt(backdc, board.left, board.top, board.right - board.left, board.bottom - board.top,
						memdc, 0, 0, board.right - board.left, board.bottom - board.top, SRCCOPY);
				}
			}

			DeleteDC(memdc);
			ReleaseDC(hWnd, hdc);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		BitBlt(hdc, 0, 0, wSize.right, wSize.bottom, backdc, 0, 0, SRCCOPY);

		DeleteObject(backBit);
		DeleteDC(backdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 's':
		case 'S':
			if (!gameCheck)
			{
				gameCheck = TRUE;
				SetTimer(hWnd, 1, 1, NULL);
				initCheck = TRUE;
			}
			break;
		case 'q':
		case 'Q':
			if (gameCheck)
			{
				gameCheck = FALSE;
				KillTimer(hWnd, 1);
				PostQuitMessage(0);
			}
			break;
		case '3':
			div = 3;
			p = { -1, -1 };
			initCheck = TRUE;
			break;
			break;
		case '4':
			div = 4;
			p = { -1, -1 };
			initCheck = TRUE;
			break;
		case '5':
			div = 5;
			p = { -1, -1 };
			initCheck = TRUE;
			break;
		}

		if (initCheck)
		{
			int arr[25];
			int a = 0;
			wlen = (board.right - board.left) / div;
			blen = bmp.bmHeight / div;

			for (int i = 0; i < div; i++)
				for (int j = 0; j < div; j++)
					arr[a++] = 10 * i + j;

			for (int i = 0; i < div; i++)
				for (int j = 0; j < div; j++)
				{
					int index = rand() % (div * div);
					if (arr[index] != -1)
					{
						tile[i][j] = arr[index];
						arr[index] = -1;
					}
					else
					{
						j--;
					}
				}

			tile[rand() % div][rand() % div] = -1;

			initCheck = FALSE;
			moveCheck = TRUE;
		}
		break;

		// f누를때만 사진 보임
	case WM_KEYDOWN:
		if (wParam == 'F')
			fullCheck = TRUE;
		break;

		// f떼면 사진 ㄴㄴㄴ
	case WM_KEYUP:
		if (wParam == 'F')
		{
			fullCheck = FALSE;
			moveCheck = TRUE;
		}
		break;

		// 마우스 좌클릭 퍼즐이동
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (!moveCheck && !fullCheck && (board.left <= mx && mx <= board.right) && (board.top <= my && my <= board.bottom))
		{
			int x = (mx - board.left) / wlen;
			int y = (my - board.top) / wlen;

			p = { x, y };
			move = 0;
			if (0 <= x && x <= div)
			{
				if (tile[y][x - 1] == -1)
				{
					moving = TRUE;
					moveCheck = TRUE;
					xdir = LEFT;
					ydir = YNO;
					moveCount = wlen / 4;
					bp = { tile[y][x] % 10, tile[y][x] / 10 };
					break;
				}
				else if (tile[y][x + 1] == -1)
				{
					moving = TRUE;
					moveCheck = TRUE;
					xdir = RIGHT;
					ydir = YNO;
					moveCount = wlen / 4;
					bp = { tile[y][x] % 10, tile[y][x] / 10 };
					break;
				}
			}
			if (0 <= y && y <= div)
			{
				if (tile[y - 1][x] == -1)
				{
					moving = TRUE;
					moveCheck = TRUE;
					xdir = XNO;
					ydir = TOP;
					moveCount = wlen / 4;
					bp = { tile[y][x] % 10, tile[y][x] / 10 };
					break;
				}
				else if (tile[y + 1][x] == -1)
				{
					moving = TRUE;
					moveCheck = TRUE;
					xdir = XNO;
					ydir = BOTTOM;
					moveCount = wlen / 4;
					bp = { tile[y][x] % 10, tile[y][x] / 10 };
					break;
				}
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}