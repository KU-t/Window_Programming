#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <vector>

#define  BOARD_SIZE 2
#define WINDOW_WIDTH   600
#define WINDOW_HEIGTH   600
#define C_BALL_R 10  
#define BIG_figure 100
#define Small_figure 10
#define speed 10 // 100의 약수
static int Rect_N = 0;




void Rectdraw(HDC hDC, int Rect_N);

class Figure {
public:
	int num;   // 도형 생김새
	int x, y;   // 도형 중심
	int ox, oy;
	POINT point[3] = { { x,y - BIG_figure },{ x - BIG_figure,y + BIG_figure },{ x + BIG_figure,y + BIG_figure } };

	Figure() {}

	Figure(int _x, int _y) {
		num = 0;
		x = _x;
		y = _y;
	}

	void figure_draw(HDC hdc) {
		switch (num) {
		case 0:   //원
			Ellipse(hdc, x - BIG_figure, y - BIG_figure, x + BIG_figure, y + BIG_figure);
			ox = x; oy = y - BIG_figure;
			break;
		case 1:   //사각형
			Rectangle(hdc, x - BIG_figure, y - BIG_figure, x + BIG_figure, y + BIG_figure);
			ox = x - BIG_figure; oy = y - BIG_figure;
			break;

		case 2:   //삼각형
			POINT tri[3] = { { x,y - BIG_figure },{ x - BIG_figure,y + BIG_figure },{ x + BIG_figure,y + BIG_figure } };
			Polygon(hdc, tri, 3);
			ox = x; oy = y - BIG_figure;
			break;
		}
	}
		
	void  One(HDC hdc) {
		switch (num) {
		case 0:
			Ellipse(hdc, ox -Small_figure, oy - Small_figure,ox + Small_figure, oy + Small_figure);
				break;
		case 1:
			Ellipse(hdc, ox - Small_figure, oy - Small_figure, ox + Small_figure, oy + Small_figure);
				break;

		case 2:
			Ellipse(hdc, ox - Small_figure, oy - Small_figure, ox + Small_figure, oy + Small_figure);
				break;
		}
	}
	
};

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

	hWnd = CreateWindow(lpszClass, "window API", WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH + 39, WINDOW_HEIGTH + 39, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;

}

static PAINTSTRUCT ps;
HDC hDC;
HPEN hPen, oldPen;
static HBRUSH hBrush, oldBrush;
int redball = 0;
static Figure figure[4];

void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)     //1번 아이디 타이머 함수 
{
	
	HDC hdc;
	hdc = GetDC(hWnd);
	srand((unsigned)time(NULL));
	if (redball == 0)
	{
		hBrush = CreateSolidBrush(RGB(255, 0, 0));

		++redball;
	}
	else
		hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));

	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	for (int i = 1; i < BOARD_SIZE * 2; i += BOARD_SIZE)
	{
		for (int j = 1; j < BOARD_SIZE * 2; j += BOARD_SIZE)
		{
			Ellipse(hDC, WINDOW_WIDTH / 4 * i - C_BALL_R, WINDOW_HEIGTH / 4 * j - C_BALL_R, WINDOW_WIDTH / 4 * i + C_BALL_R, WINDOW_HEIGTH / 4 * j + C_BALL_R);
		}
	}
	SelectObject(hdc, oldBrush);

	DeleteObject(hBrush);

	ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	

	figure[0].x = WINDOW_WIDTH / 4;
	figure[1].x = WINDOW_WIDTH / 4 * 3;
	figure[2].x = WINDOW_WIDTH / 4;
	figure[3].x = WINDOW_WIDTH / 4 * 3;

	figure[0].y = WINDOW_HEIGTH / 4;
	figure[1].y = WINDOW_HEIGTH / 4;
	figure[2].y = WINDOW_HEIGTH / 4 * 3;
	figure[3].y = WINDOW_HEIGTH / 4 * 3;


	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);
		SetTimer(hWnd, 2, 500, TimerProc);
		
		/*SetTimer(hWnd, 3, 1000, NULL);
		SetTimer(hWnd, 4, 1000, NULL);*/
		break;
		
	case WM_TIMER:
		switch (wParam) {
		case 1:

			for (int i = 0; i < 4; i++) {
				switch (figure[i].num) {
				case 0:
					
					break;
				
				case 1:
					if (figure[i].ox == figure[i].x - BIG_figure && figure[i].oy == figure[i].y - BIG_figure) {
						figure[i].ox += speed; 
					}
					else if (figure[i].ox == figure[i].x + BIG_figure && figure[i].oy == figure[i].y - BIG_figure) {
						figure[i].oy += speed;
					}
					else if (figure[i].ox == figure[i].x + BIG_figure && figure[i].oy == figure[i].y + BIG_figure) {
						figure[i].ox -= speed;
					}
					else if (figure[i].ox == figure[i].x - BIG_figure && figure[i].oy == figure[i].y + BIG_figure) {
						figure[i].oy -= speed;
					}

					else if (figure[i].ox > figure[i].x - BIG_figure && figure[i].ox < figure[i].x + BIG_figure && figure[i].oy == figure[i].y - BIG_figure) {
						figure[i].ox += speed;
					}
					else if (figure[i].ox == figure[i].x + BIG_figure && figure[i].oy > figure[i].y - BIG_figure && figure[i].oy < figure[i].y + BIG_figure) {
						figure[i].oy += speed;
					}
					else if (figure[i].ox < figure[i].x + BIG_figure && figure[i].ox > figure[i].x - BIG_figure && figure[i].oy == figure[i].y + BIG_figure) {
						figure[i].ox -= speed;
					}
					else if (figure[i].ox > figure[i].x - BIG_figure && figure[i].ox < figure[i].x + BIG_figure && figure[i].oy == figure[i].y + BIG_figure) {
						figure[i].oy -= speed;
					}
					break;
				
				case 2:

					break;
				}
			}
			
			
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
		
	case WM_CHAR:
		switch (wParam) {

		case '1':
			Rect_N = 1;         break;
		case '2':
			Rect_N = 2;         break;
		case '3':
			Rect_N = 3;     break;
		case '4':
			Rect_N = 4;    break;

		case 'r':
		case 'R':
			figure[Rect_N - 1].num = 1;
			break;

		case 't':
		case 'T':
			figure[Rect_N - 1].num = 2;
			break;
		}
		InvalidateRect(hWnd, NULL, true);// 1번 아이디의 타이머가 0.5초 마다 TimerProc 타이머 함수 실행      

		break;


	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		Rectdraw(hDC, Rect_N);
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hDC, hPen);

		int circle_Wsize = rand() % (WINDOW_WIDTH / 4 - 20) + 10;
		int circle_Hsize = rand() % (WINDOW_HEIGTH - C_BALL_R) + (C_BALL_R * 2);

	
		for (int i = 0; i < 4; i++) {
			figure[i].figure_draw(hDC);
			figure[i].One(hDC);
		}


		EndPaint(hWnd, &ps);

		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Rectdraw(HDC hDC, int Rect_N) {

	switch (Rect_N) {
	case 0:
		break;

	case 1:
		Rectangle(hDC, 0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGTH / 2);
		break;

	case 2:
		Rectangle(hDC, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH, WINDOW_HEIGTH / 2);
		break;

	case 3:
		Rectangle(hDC, 0, WINDOW_HEIGTH / 2, WINDOW_WIDTH / 2, WINDOW_HEIGTH);
		break;
	case 4:
		Rectangle(hDC, WINDOW_WIDTH / 2, WINDOW_HEIGTH / 2, WINDOW_WIDTH, WINDOW_HEIGTH);
		break;
	}
}