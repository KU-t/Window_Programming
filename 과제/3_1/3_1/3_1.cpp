#include <windows.h>
#include <tchar.h>
#include <time.h>

#define WD_left 800
#define WD_top 800
#define Board_size 800
#define Cell_size 20
#define Random1_4 rand()%4+1

HDC hdc;
HWND hWnd;
RECT rectView;

void draw();

class Circle {
public:
	int x,y,way;
	bool check=false;

	void Random_way() {  //**************먹이 랜덤 움직임
		way = rand() % 4 + 1;
		switch (way) {
		case 1:
			y -= Cell_size;
			if (y - 10 < rectView.top) {
				y += Cell_size;
				way = Random1_4;
			}
			break;

		case 2:
			y += Cell_size;
			if (y + 10 > rectView.bottom) {
				y -= Cell_size;
				way = Random1_4;
			}
			break;

		case 3:
			x -= Cell_size;
			if (x - 10 < rectView.left) {
				x += Cell_size;
				way = Random1_4;
			}
			break;

		case 4:
			x += Cell_size;
			if (x + 10 > rectView.right) {
				x -= Cell_size;
				way = Random1_4;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}

	void Go_way() {//**********************머리 움직임
		
		switch (wParam) {
		case VK_UP:
			y -= Cell_size;
			if (y - 10 < rectView.top) {
				y += Cell_size;
				way = Random1_4;
			}
			break;

		case 2:
			y += Cell_size;
			if (y + 10 > rectView.bottom) {
				y -= Cell_size;
				way = Random1_4;
			}
			break;

		case 3:
			x -= Cell_size;
			if (x - 10 < rectView.left) {
				x += Cell_size;
				way = Random1_4;
			}
			break;

		case 4:
			x += Cell_size;
			if (x + 10 > rectView.right) {
				x -= Cell_size;
				way = Random1_4;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}

	void draw() {
		Ellipse(hdc, x - 10, y - 10, x + 10, y + 10);
	}
};

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";


LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow) {

	
	MSG Message;
	WNDCLASSEX WndClass;

	srand((unsigned)time(NULL));

	g_hInst = hInstance;

	srand((unsigned)time(NULL));

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
	PAINTSTRUCT ps;
	
	static int time_speed = 50;
	static int jump=0;
	static Circle cir[100];



	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);
		SetTimer(hWnd, 1, time_speed, NULL);
		SetTimer(hWnd, 2, 200, NULL);
		SetTimer(hWnd, 3, time_speed, NULL);
		cir[1].x = 350; cir[1].y = 350;
		cir[1].way =
		break;

	case WM_CHAR:
		switch (wParam) {

		case '+':
		case '=':
			if (time_speed > 20)
				time_speed -= 10;
			break;

		case '-':
		case '_':
			if (time_speed < 200)
				time_speed += 10;
			break;
		case 'q':
		case 'Q':
			exit(1);
		}
		break;

		

		case WM_KEYUP:
			switch (wParam) {
			case VK_UP:
				check = 1;
				break;
			case VK_DOWN:
				check = 2;
				break;
			case VK_LEFT:
				check = 3;
				break;
			case VK_RIGHT:
				check = 4;
				break;
			case VK_SPACE:
				jump = 4;
				break;

			}
			break;

		

//**************************paint**************************
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			draw();

			
			
			EndPaint(hWnd, &ps);
			break;


//*************************timer************************
		case WM_TIMER:
			
			switch (wParam) {
			case 1://타이머 2  머리이동
				switch (check) {
				case 1:
					y -= Cell_size;
					if (y - 10 < rectView.top) {
						y += Cell_size;
						check = Random1_4;
					}

					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 2:
					y += Cell_size;
					if (y + 10 > rectView.bottom) {
						y -= Cell_size;
						check = Random1_4;
					}

					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 3:
					x -= Cell_size;
					if (x - 10 < rectView.left) {
						x += Cell_size;
						check = Random1_4;
					}

					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 4:
					x += Cell_size;
					if (x + 10 > rectView.right) {
						x -= Cell_size;
						check = Random1_4;
					}

					InvalidateRect(hWnd, NULL, TRUE);
					break;
				}
				break;

			case 2:// 타이머 2 점프
				switch (check) {
				case 1:
				case 2:
					if (jump <= 4 && jump >= 3) {
						if (x > 340) {
							x -= Cell_size;	jump--;
							InvalidateRect(hWnd, NULL, TRUE);
						}
						else if (x <= 340) {
							x += Cell_size;	jump--;
							InvalidateRect(hWnd, NULL, TRUE);
						}
					}

					else if (jump <= 2 && jump >= 0) {
						if (x > 340) {
							x += Cell_size;	jump--;
							if (jump == 0) 	move = 1;
							InvalidateRect(hWnd, NULL, TRUE);
						}
						else if (x <= 340) {
							x -= Cell_size;	jump--;
							if (jump == 0) 	move = 1;
							InvalidateRect(hWnd, NULL, TRUE);
						}


					}
					break;

				case 3:
				case 4:
					if (jump <= 4 && jump >= 3) {
						if (y > 340) {
							y -= Cell_size;	jump--;
							InvalidateRect(hWnd, NULL, TRUE);
						}
						else if (y <= 340) {
							y += Cell_size;	jump--;
							InvalidateRect(hWnd, NULL, TRUE);
						}
					}

					else if (jump <= 2 && jump >= 0) {
						if (y > 340) {
							y += Cell_size;	jump--;
							if (jump == 0) 	move = 1;
							InvalidateRect(hWnd, NULL, TRUE);
						}
						else if (y <= 340) {
							y -= Cell_size;	jump--;
							if (jump == 0) 	move = 1;
							InvalidateRect(hWnd, NULL, TRUE);
						}

					}
					break;
				}
				break;

			case 3://타이머 3 
			



				break;
			}
	
		
			

			


		case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void draw() {
	for (int i = 0; i < Board_size; i += Cell_size) {
		MoveToEx(hdc, i, 0, NULL);
		LineTo(hdc, i, Board_size - Cell_size);
		MoveToEx(hdc, 0, i, NULL);
		LineTo(hdc, Board_size - Cell_size, i);
	}

}