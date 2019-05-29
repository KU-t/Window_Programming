#include <Windows.h>
#include <time.h>


#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define mapSize 50
#define MAX 100

struct stat {
	int x, x2;
	int y, y2;
	int dir = 0;
	bool isReal = false;
};

stat head;
stat body[MAX - 1];
stat Food[MAX];

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	srand((unsigned int)time(NULL));
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

	hWnd = CreateWindow
	(lpszClass, "window program 3-1",
		WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, (HMENU)NULL,
		hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush, oldBrush;

	static int count = 0;
	static int food_count = 0;
	static bool isStart = false;
	static bool isUp = false;
	static int jump = 0;
	static int Speed = 200;
	static bool isOn = false;
	static bool isBig = false;
	static int boom = 0;

	switch (iMessage) {
	case WM_CREATE:
	{
		head.x = mapSize / 2;
		head.y = mapSize / 2;
		head.x2 = head.x + 1;
		head.y2 = head.y + 1;
		head.dir = 1;
		head.isReal = true;

		SetTimer(hWnd, 1, Speed, NULL); // 빨간돌 움직
		SetTimer(hWnd, 2, 3000, NULL); // 파란돌 생성
		SetTimer(hWnd, 3, 1000, NULL); // 파란돌 움직임
		break;
	}
	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		for (int i = 0; i < mapSize; ++i) {
			for (int j = 0; j < mapSize; ++j) {
				
				Rectangle(hDC, j*((WINDOW_WIDTH - 50) / mapSize), i*((WINDOW_HEIGHT - 50) / mapSize), (j + 1)*((WINDOW_WIDTH - 50) / mapSize), (i + 1)*((WINDOW_HEIGHT - 50) / mapSize));
			}
		}
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		DeleteObject(oldBrush);

		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		if (isOn) {
			switch (boom) {
			case 0:
				Ellipse(hDC, head.x * ((WINDOW_WIDTH - 50) / mapSize), head.y * ((WINDOW_HEIGHT - 50) / mapSize), head.x2 * ((WINDOW_WIDTH - 50) / mapSize), head.y2 * ((WINDOW_HEIGHT - 50) / mapSize));
				break;
			case 1:
				Ellipse(hDC, (head.x - 1) * ((WINDOW_WIDTH - 50) / mapSize), (head.y - 1) * ((WINDOW_HEIGHT - 50) / mapSize), (head.x2 + 1) * ((WINDOW_WIDTH - 50) / mapSize), (head.y2 + 1) * ((WINDOW_HEIGHT - 50) / mapSize));
				break;
			}
		}
		else
			Ellipse(hDC, head.x * ((WINDOW_WIDTH - 50) / mapSize), head.y * ((WINDOW_HEIGHT - 50) / mapSize), head.x2 * ((WINDOW_WIDTH - 50) / mapSize), head.y2 * ((WINDOW_HEIGHT - 50) / mapSize));
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		DeleteObject(oldBrush);

		for (int i = 0; i < MAX; ++i) {
			if (Food[i].isReal) {
				hBrush = CreateSolidBrush(RGB(0, 0, 255));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, Food[i].x * ((WINDOW_WIDTH - 50) / mapSize), Food[i].y * ((WINDOW_HEIGHT - 50) / mapSize), Food[i].x2 * ((WINDOW_WIDTH - 50) / mapSize), Food[i].y2 * ((WINDOW_HEIGHT - 50) / mapSize));
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
				DeleteObject(oldBrush);
			
			}
		}

		for (int i = 0; i < count; ++i) {
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, body[i].x * ((WINDOW_WIDTH - 50) / mapSize), body[i].y * ((WINDOW_HEIGHT - 50) / mapSize), body[i].x2 * ((WINDOW_WIDTH - 50) / mapSize), body[i].y2 * ((WINDOW_HEIGHT - 50) / mapSize));
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
			DeleteObject(oldBrush);
		}
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_UP) {
			head.dir = 1;
		}
		else if (wParam == VK_RIGHT) {
			head.dir = 2;
		}
		else if (wParam == VK_DOWN) {
			head.dir = 3;
		}
		else if (wParam == VK_LEFT) {
			head.dir = 4;
		}
		else if (wParam == VK_SPACE) {
			SetTimer(hWnd, 4, 100, NULL); // 점프
		}
		break;
	}
	case WM_CHAR:
	{
		char input = wParam;
		switch (input) {
		case 'H':
		case 'h':
			if (!isOn) {
				isOn = true;
				SetTimer(hWnd, 5, 100, NULL);
			}
			else {
				isOn = false;
				KillTimer(hWnd, 5);
			}
			break;
		case '+':
		case '=':
			if (Speed > 20) {
				Speed -= 10;
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, Speed, NULL);
			}
			break;
		case '-':
		case '_':
			if (Speed < 200) {
				Speed += 10;
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, Speed, NULL);
			}
			break;
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			break;
		}
		break;
	}
	case WM_TIMER:
	{
		switch (wParam) {
		case 1:
			for (int i = count; i > 0; --i) {
				body[i].x = body[i - 1].x;
				body[i].y = body[i - 1].y;
				body[i].x2 = body[i - 1].x2;
				body[i].y2 = body[i - 1].y2;
				//body[i].dir = body[i - 1].dir;
			}
			if (body[0].isReal) {
				body[0].x = head.x;
				body[0].y = head.y;
				body[0].x2 = head.x2;
				body[0].y2 = head.y2;
				//body[0].dir = head.dir;
			}
			switch (head.dir) {
			case 1:
				if (head.y > 0) {
					--head.y;
					--head.y2;
				}
				else head.dir = 1 + rand() % 4;
				break;
			case 2:
				if (head.x2 < mapSize) {
					++head.x;
					++head.x2;
				}
				else head.dir = 1 + rand() % 4;
				break;
			case 3:
				if (head.y2 < mapSize) {
					++head.y;
					++head.y2;
				}
				else head.dir = 1 + rand() % 4;
				break;
			case 4:
				if (head.x > 0) {
					--head.x;
					--head.x2;
				}
				else head.dir = 1 + rand() % 4;
				break;
			}
			for (int i = 0; i < food_count; ++i) {
				if (Food[i].isReal) {
					if (head.x == Food[i].x && head.y == Food[i].y) {
						Food[i].isReal = false;
						++count;
						body[0].isReal = true;
					}
				}
			}

			break;
		case 2:
			if (food_count < MAX) {
				++food_count;
				Food[food_count].x = 5 + rand() % (mapSize - 10);
				Food[food_count].y = 5 + rand() % (mapSize - 10);
				Food[food_count].x2 = Food[food_count].x + 1;
				Food[food_count].y2 = Food[food_count].y + 1;
				Food[food_count].dir = 1 + rand() % 4;
				Food[food_count].isReal = true;
			}
			break;
		case 3:
			for (int i = 0; i < food_count; ++i) {
				switch (Food[i].dir) {
				case 1:
					if (Food[i].y > 0) {
						--Food[i].y;
						--Food[i].y2;
					}
					else Food[i].dir = 1 + rand() % 4;
					break;
				case 2:
					if (Food[i].x2 < mapSize) {
						++Food[i].x;
						++Food[i].x2;
					}
					else Food[i].dir = 1 + rand() % 4;
					break;
				case 3:
					if (Food[i].y2 < mapSize) {
						++Food[i].y;
						++Food[i].y2;
					}
					else Food[i].dir = 1 + rand() % 4;
					break;
				case 4:
					if (Food[i].x > 0) {
						--Food[i].x;
						--Food[i].x2;
					}
					else Food[i].dir = 1 + rand() % 4;
					break;
				}
			}
			break;
		case 4:
		{
			switch (head.dir) {
			case 1:
			case 3:
				if (!isStart) {
					isStart = true;
					isUp = true;
				}

				if (isStart) {
					if (isUp) {
						if (jump < 3) {
							++jump;
							++head.x;
							++head.x2;
							isUp = false;
						}
					}
					else {
						if (jump == 0) {
							isStart = false;
							isUp = false;
							KillTimer(hWnd, 4);
						}
						else {
							--jump;
							--head.x;
							--head.x2;
						}
					}
				}
				break;
			case 2:
			case 4:
				if (!isStart) {
					isStart = true;
					isUp = true;
				}

				if (isStart) {
					if (isUp) {
						if (jump < 3) {
							++jump;
							--head.y;
							--head.y2;
							isUp = false;
						}
					}
					else {
						if (jump == 0) {
							isStart = false;
							isUp = false;
							KillTimer(hWnd, 4);
						}
						else {
							--jump;
							++head.y;
							++head.y2;
						}
					}
				}
				break;
			}
		}
		break;
		case 5:
			if (!isOn) {
				isBig = true;
				isOn = true;

			}
			else {
				if (isBig) {
					if (boom < 1) {
						++boom;
					}
					else {
						isBig = false;
					}
				}
				else {
					if (boom == 0) {
						isBig = true;
					}
					else {
						--boom;
					}
				}
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		KillTimer(hWnd, 4);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}