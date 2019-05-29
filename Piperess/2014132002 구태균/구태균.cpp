#pragma comment(lib, "msimg32.lib")
#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <ctime>
#include <atlimage.h>

#define WD_WIDTH 720
#define WD_HEIGHT 720
#define CellSize 30

//보드 사이즈
#define Small_size 10 
#define Mid_size 20
#define Big_size 30

//총 블록개수
#define Block_Total 200

enum SHAPE { lt, tr, rb, bl, tb, lr };
enum SPEED { L1 = 2, L2 = 3, L3 = 4 };

class Block {
public:
	//좌표
	int x, y;

	//그리기
	bool T;

	// 파이프 완성
	bool p;

	//연결
	bool t, b, l, r;

	int check;

	//모형
	int Shape;
	Block() {
		T = FALSE; x = WD_WIDTH / 2; y = 0;
		t = TRUE; b = TRUE; l = TRUE; r = TRUE;
	}
};

//전역 변수
Block block[Block_Total];	//블록
int count;					//블록 수 변수

int Board_num;	//Board 개수 변수



HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow) {

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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "2014132002 구태균", WS_OVERLAPPEDWINDOW, 0, 0, WD_WIDTH + 216, WD_HEIGHT + 39, NULL, (HMENU)NULL, hInstance, NULL);

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
	static HDC hdc, savedc, oldsavedc, savedc1, oldsavedc1, bitdc[6];
	PAINTSTRUCT ps;


	//배경
	static HBITMAP Hbitmap, oldhbitmap, Hbitmap1, oldhbitmap1;

	//그리기
	static HBRUSH hbrush, oldbrush;
	static HPEN hpen, oldpen;

	//시작
	static bool start;
	static int size;

	static bool sw;

	static int GO;
	static int speed;

	static int st_check;

	static float mx, my;
	HFONT hfont, oldfont;

	//새로운 블록
	static bool New_block;

	//Cimage
	static CImage img[18];
	static CImage b[5];
	

	//출동체크 변수
	static bool boom;

	switch (uMsg) {
	case WM_CREATE:
		start = TRUE;
		size = 0;

		GO = 0;
		speed = L1;
		//비트맵 넣기
		img[0].Load(TEXT("0'.bmp"));
		img[1].Load(TEXT("90'.bmp"));
		img[2].Load(TEXT("180'.bmp"));
		img[3].Load(TEXT("270'.bmp"));
		img[4].Load(TEXT("I.bmp"));
		img[5].Load(TEXT("ㅡ.bmp"));

		img[6].Load(TEXT("0'1.bmp"));
		img[7].Load(TEXT("90'1.bmp"));
		img[8].Load(TEXT("180'1.bmp"));
		img[9].Load(TEXT("270'1.bmp"));
		img[10].Load(TEXT("I1.bmp"));
		img[11].Load(TEXT("ㅡ1.bmp"));

		img[12].Load(TEXT("0'2.bmp"));
		img[13].Load(TEXT("90'2.bmp"));
		img[14].Load(TEXT("180'2.bmp"));
		img[15].Load(TEXT("270'2.bmp"));
		img[16].Load(TEXT("I2.bmp"));
		img[17].Load(TEXT("ㅡ2.bmp"));

		b[0].Load(TEXT("b1.bmp"));
		b[1].Load(TEXT("b2.bmp"));
		b[2].Load(TEXT("b3.bmp"));
		b[3].Load(TEXT("b4.bmp"));
		b[4].Load(TEXT("b5.bmp"));
		b[5].Load(TEXT("b6.bmp"));

		//처음 보드 크기
		Board_num = Mid_size;

		//처음 블록 
		count = 0;
		New_block = TRUE;
		block[0].Shape = rand() % 6;

		
		//SetTimer(hWnd, 2, 10, NULL);

		//충돌체크
		boom = FALSE;
		break;


		// 타이머
	case WM_TIMER:
		switch (wParam) {
		case 1:

			//내려오면서 
			if (block[count].y < (WD_HEIGHT * (Board_num - 1) / Board_num)) {

				if (count == 0)
					boom = FALSE;

				else if (count > 0) {
					for (int i = 0; i < count; i++) {
						if ((block[i].x == block[count].x && block[i].y == block[count].y + WD_HEIGHT / Board_num))
							boom = TRUE;
					}
				}
			}

			//맨아래
			else if (block[count].y == (WD_HEIGHT * (Board_num - 1) / Board_num))
				boom = TRUE;


			{	//다음 블럭
				if (boom == FALSE) {
					if (count < 3)		speed = L1;	
					else if (count < 6) speed = L2;
					else				speed = L3;

					block[count].y += speed;
				}
					
				else if (boom == TRUE) {
					New_block = TRUE;
					boom = FALSE;
					

					if (block[count].Shape == lt) {
						for (int i = 0; i < count; i++) { // l
							if (block[count].y == block[i].y && block[count].x - WD_WIDTH / Board_num == block[i].x) {
								block[count].l = TRUE;
								block[i].r = TRUE;
							}	
						}
					}

					else if (block[count].Shape == tr) {
						for (int i = 0; i < count; i++) { //r
							if (block[count].y == block[i].y && block[count].x + WD_WIDTH / Board_num == block[i].x) {
								block[count].r = TRUE;
								block[i].l = TRUE;
							}
						}
					}

					else if (block[count].Shape == rb) { //
						for (int i = 0; i < count; i++) { //r
							if (block[count].y == block[i].y && block[count].x + WD_WIDTH / Board_num == block[i].x) {
								block[count].r = TRUE;
								block[i].l = TRUE;
							}
						}

						for (int i = 0; i < count; i++) { // b
							if (block[count].x == block[i].x && block[count].y + WD_WIDTH / Board_num == block[i].y) {
								if (block[i].t == FALSE) {
									block[count].b = TRUE;
									block[i].t = TRUE;
								}
							}
						}
					}

					else if (block[count].Shape == bl) { //
						for (int i = 0; i < count; i++) { // b
							if (block[count].x == block[i].x && block[count].y + WD_WIDTH / Board_num == block[i].y) {
								if (block[i].t == FALSE) {
									block[count].b = TRUE;
									block[i].t = TRUE;
								}
							}
						}
						for (int i = 0; i < count; i++) { // l
							if (block[count].y == block[i].y && block[count].x - WD_WIDTH / Board_num == block[i].x) {
								block[count].l = TRUE;
								block[i].r = TRUE;
							}
						}
					}

					else if (block[count].Shape == lr) { //
						for (int i = 0; i < count; i++) { // l
							if (block[count].y == block[i].y && block[count].x - WD_WIDTH / Board_num == block[i].x) {
								block[count].l = TRUE;
								block[i].r = TRUE;
							}
						}
						for (int i = 0; i < count; i++) { //r
							if (block[count].y == block[i].y && block[count].x + WD_WIDTH / Board_num == block[i].x) {
								block[count].r = TRUE;
								block[i].l = TRUE;
								
							}
						}
					}

					else if (block[count].Shape == tb) { 
						for (int i = 0; i < count; i++) { // b
							if (block[count].x == block[i].x && block[count].y + WD_WIDTH / Board_num == block[i].y) {
								if (block[i].t == FALSE) {
									block[count].b = TRUE;
									block[i].t = TRUE;
								}
							}
						}
					}

					if (block[count].x == 0 / Board_num)	block[count].l = TRUE;
					if (block[count].x == WD_WIDTH * (Board_num - 1) / Board_num)	block[count].r = TRUE;
					if (block[count].y == WD_HEIGHT * (Board_num - 1) / Board_num)	block[count].b = TRUE;

					for (int i = 0; i <= count; i++) {
						if (block[i].p == FALSE) {
							if ( block[i].t == TRUE && block[i].r == TRUE && block[i].b == TRUE) {
								block[i].p = TRUE;
							}
							else if (block[i].l == TRUE && block[i].r == TRUE && block[i].b == TRUE) {
								block[i].p = TRUE;
							}
							else if (block[i].l == TRUE && block[i].t == TRUE && block[i].b == TRUE) {
								block[i].p = TRUE;
							}
							else if (block[i].l == TRUE && block[i].t == TRUE && block[i].r == TRUE ) {
								block[i].p = TRUE;
							}
						}	
					}

					for (int i = 0; i <= count; i++) {
						if (block[i].l == TRUE && block[i].t == TRUE && block[i].r == TRUE && block[i].b == TRUE)
							block[i].T = FALSE;
					}
					count++;
				}

			}
			break;

			case 2:

				//내려오면서 
				if (block[count].y < (WD_HEIGHT * (Board_num - 1) / Board_num)) {

					if (count == 0)
						boom = FALSE;

					else if (count > 0) {
						for (int i = 0; i < count; i++) {
							if ((block[i].x == block[count].x && block[i].y == block[count].y + WD_HEIGHT / Board_num))
								boom = TRUE;
						}
					}
				}

				//맨아래
				else if (block[count].y == (WD_HEIGHT * (Board_num - 1) / Board_num))
					boom = TRUE;


				{	//다음 블럭
					if (boom == FALSE) {
						if (count < 1)
							block[count].y += L1;
						else if (count < 2)
							block[count].y += L2;
						else
							block[count].y += L3;
					}

					else if (boom == TRUE) {
						New_block = TRUE;
						boom = FALSE;


						if (block[count].Shape == lt) {
							for (int i = 0; i < count; i++) { // l
								if (block[count].y == block[i].y && block[count].x - WD_WIDTH / Board_num == block[i].x) {
									block[count].l = TRUE;
									block[i].r = TRUE;
								}
							}
						}

						else if (block[count].Shape == tr) {
							for (int i = 0; i < count; i++) { //r
								if (block[count].y == block[i].y && block[count].x + WD_WIDTH / Board_num == block[i].x) {
									block[count].r = TRUE;
									block[i].l = TRUE;
								}
							}
						}

						else if (block[count].Shape == rb) { //
							for (int i = 0; i < count; i++) { //r
								if (block[count].y == block[i].y && block[count].x + WD_WIDTH / Board_num == block[i].x) {
									block[count].r = TRUE;
									block[i].l = TRUE;
								}
							}

							for (int i = 0; i < count; i++) { // b
								if (block[count].x == block[i].x && block[count].y + WD_WIDTH / Board_num == block[i].y) {
									if (block[i].t == FALSE) {
										block[count].b = TRUE;
										block[i].t = TRUE;
									}
								}
							}
						}

						else if (block[count].Shape == bl) { //
							for (int i = 0; i < count; i++) { // b
								if (block[count].x == block[i].x && block[count].y + WD_WIDTH / Board_num == block[i].y) {
									if (block[i].t == FALSE) {
										block[count].b = TRUE;
										block[i].t = TRUE;
									}
								}
							}
							for (int i = 0; i < count; i++) { // l
								if (block[count].y == block[i].y && block[count].x - WD_WIDTH / Board_num == block[i].x) {
									block[count].l = TRUE;
									block[i].r = TRUE;
								}
							}
						}

						else if (block[count].Shape == lr) { //
							for (int i = 0; i < count; i++) { // l
								if (block[count].y == block[i].y && block[count].x - WD_WIDTH / Board_num == block[i].x) {
									block[count].l = TRUE;
									block[i].r = TRUE;
								}
							}
							for (int i = 0; i < count; i++) { //r
								if (block[count].y == block[i].y && block[count].x + WD_WIDTH / Board_num == block[i].x) {
									block[count].r = TRUE;
									block[i].l = TRUE;

								}
							}
						}

						else if (block[count].Shape == tb) {
							for (int i = 0; i < count; i++) { // b
								if (block[count].x == block[i].x && block[count].y + WD_WIDTH / Board_num == block[i].y) {
									if (block[i].t == FALSE) {
										block[count].b = TRUE;
										block[i].t = TRUE;
									}
								}
							}
						}

						if (block[count].x == 0 / Board_num)	block[count].l = TRUE;
						if (block[count].x == WD_WIDTH * (Board_num - 1) / Board_num)	block[count].r = TRUE;
						if (block[count].y == WD_HEIGHT * (Board_num - 1) / Board_num)	block[count].b = TRUE;

						for (int i = 0; i <= count; i++) {
							if (block[i].p == FALSE) {
								if (block[i].t == TRUE && block[i].r == TRUE && block[i].b == TRUE) {
									block[i].p = TRUE;
								}
								else if (block[i].l == TRUE && block[i].r == TRUE && block[i].b == TRUE) {
									block[i].p = TRUE;
								}
								else if (block[i].l == TRUE && block[i].t == TRUE && block[i].b == TRUE) {
									block[i].p = TRUE;
								}
								else if (block[i].l == TRUE && block[i].t == TRUE && block[i].r == TRUE) {
									block[i].p = TRUE;
								}
							}
						}

						for (int i = 0; i <= count; i++) {
							if (block[i].l == TRUE && block[i].t == TRUE && block[i].r == TRUE && block[i].b == TRUE)
								block[i].T = FALSE;
						}
						count++;
					}

				}
				break;
		}



		InvalidateRect(hWnd, NULL, FALSE);
		break;


		// 페인트
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Hbitmap = CreateCompatibleBitmap(hdc, WD_WIDTH + 200, WD_WIDTH);
		Hbitmap1 = CreateCompatibleBitmap(hdc, WD_WIDTH, WD_WIDTH);

		savedc = CreateCompatibleDC(hdc);
		savedc1 = CreateCompatibleDC(hdc);

		oldhbitmap = (HBITMAP)SelectObject(savedc, Hbitmap);
		oldhbitmap1 = (HBITMAP)SelectObject(savedc1, Hbitmap1);
		
		if (start == TRUE) {
			hpen = CreatePen(PS_SOLID, 1, RGB(155, 200, 155));
			SelectObject(savedc1, hpen);
			hbrush = CreateSolidBrush(RGB(155, 200, 155));
			SelectObject(savedc1, hbrush);

			Rectangle(savedc1, 0, 0, 200, WD_WIDTH);

			SelectObject(savedc1, oldpen);
			SelectObject(savedc1, oldbrush);
			DeleteObject(hpen);
			DeleteObject(hbrush);
			
			hpen = CreatePen(PS_SOLID, 1, RGB(155, 200, 155));
			SelectObject(savedc, hpen);
			hbrush = CreateSolidBrush(RGB(250, 250, 200));
			SelectObject(savedc, hbrush);
			for (int i = 1; i <= Board_num; i++) {
				for (int j = 1; j <= Board_num; j++) {
					Rectangle(savedc, WD_WIDTH * (i - 1) / Board_num, WD_HEIGHT * (j - 1) / Board_num, WD_WIDTH * i / Board_num, WD_HEIGHT * j / Board_num);
				}
			}
			SelectObject(savedc, oldpen);
			SelectObject(savedc, oldbrush);
			DeleteObject(hpen);
			DeleteObject(hbrush);

			img[tb].TransparentBlt(savedc, WD_WIDTH * (10 - 1) / Board_num, WD_HEIGHT * (20 - 1) / Board_num, WD_WIDTH / Board_num, WD_HEIGHT / Board_num, RGB(255, 255, 255));
			img[tb].TransparentBlt(savedc, WD_WIDTH * (10 - 1) / Board_num, WD_HEIGHT * (19 - 1) / Board_num, WD_WIDTH / Board_num, WD_HEIGHT / Board_num, RGB(255, 255, 255));
			img[rb].TransparentBlt(savedc, WD_WIDTH * (10 - 1) / Board_num, WD_HEIGHT * (18 - 1) / Board_num, WD_WIDTH / Board_num, WD_HEIGHT / Board_num, RGB(255, 255, 255));
			img[lr].TransparentBlt(savedc, WD_WIDTH * (11 - 1) / Board_num, WD_HEIGHT * (20 - 1) / Board_num, WD_WIDTH / Board_num, WD_HEIGHT / Board_num, RGB(255, 255, 255));
			img[tr].TransparentBlt(savedc, WD_WIDTH * (11 - 1) / Board_num, WD_HEIGHT * (19 - 1) / Board_num, WD_WIDTH / Board_num, WD_HEIGHT / Board_num, RGB(255, 255, 255));

			hpen = CreatePen(PS_SOLID, 10, RGB(0, 0, 0)); //		//선
			oldpen = (HPEN)SelectObject(savedc, hpen);	//

			hbrush = CreateSolidBrush(RGB(150, 150, 150));	//			// 면
			oldbrush = (HBRUSH)SelectObject(savedc, hbrush);	//

			hfont = CreateFont(200, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Fixedsys"));
			oldfont = (HFONT)SelectObject(savedc, hfont);	//

			SetBkColor(savedc, RGB(155, 200, 155));
			SetTextColor(savedc, RGB(250, 250, 200));

			TextOut(savedc, WD_WIDTH * 1 / 5, WD_HEIGHT * 1 / 8, "Piperess", 8);

			SelectObject(savedc, oldpen);	//
			SelectObject(savedc, oldbrush);	//
			SelectObject(savedc, oldfont);	//
			
			DeleteObject(oldbrush);
			DeleteObject(oldpen);
			DeleteObject(oldfont);

			
			hbrush = CreateSolidBrush(RGB(250, 250, 200));
			SelectObject(savedc1, hbrush);

			if (size == Small_size)
				hpen = CreatePen(PS_SOLID, 5, RGB(200, 155, 155));
			else
				hpen = CreatePen(PS_SOLID, 5, RGB(155, 155, 200));
			SelectObject(savedc1, hpen);
			Rectangle(savedc1, 50, 100, 150, 200);

			if (size == Mid_size)
				hpen = CreatePen(PS_SOLID, 5, RGB(200, 155, 155));
			else
				hpen = CreatePen(PS_SOLID, 5, RGB(155, 155, 200));
			SelectObject(savedc1, hpen);
			Rectangle(savedc1, 50, 300, 150, 400);

			if (size == Big_size)
				hpen = CreatePen(PS_SOLID, 5, RGB(200, 155, 155));
			else
				hpen = CreatePen(PS_SOLID, 5, RGB(155, 155, 200));
			SelectObject(savedc1, hpen);
			Rectangle(savedc1, 50, 500, 150, 600);

			

			SelectObject(savedc1, oldpen);
			SelectObject(savedc1, oldbrush);
			DeleteObject(hpen);
			DeleteObject(hbrush);

			hpen = CreatePen(PS_SOLID, 10, RGB(0, 0, 0)); //		//선
			oldpen = (HPEN)SelectObject(savedc1, hpen);	//

			hbrush = CreateSolidBrush(RGB(150, 150, 150));	//			// 면
			oldbrush = (HBRUSH)SelectObject(savedc1, hbrush);	//

			hfont = CreateFont(90, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Fixedsys"));
			oldfont = (HFONT)SelectObject(savedc1, hfont);	//

			SetBkColor(savedc1, RGB(250, 250, 200));
			SetTextColor(savedc1, RGB(155, 200, 155));

			TextOut(savedc1, 20, 10, "SIZE", 4);
			
			TextOut(savedc1,60,110, "10", 2);
			TextOut(savedc1,60,310, "20", 2);
			TextOut(savedc1,60,510, "30", 2);

			SelectObject(savedc1, oldpen);	//
			SelectObject(savedc1, oldbrush);	//
			SelectObject(savedc1, oldfont);	//

			DeleteObject(oldbrush);
			DeleteObject(oldpen);
			DeleteObject(oldfont);

			
		}
		
		else if (start == FALSE) {
			// savedc1 - 배경 그리기
			hpen = CreatePen(PS_SOLID, 1, RGB(155, 200, 155));
			SelectObject(savedc1, hpen);
			hbrush = CreateSolidBrush(RGB(155, 200, 155));
			SelectObject(savedc1, hbrush);

			Rectangle(savedc1, 0, 0, 200, WD_WIDTH);

			SelectObject(savedc1, oldpen);
			SelectObject(savedc1, oldbrush);
			DeleteObject(hpen);
			DeleteObject(hbrush);

			hpen = CreatePen(PS_SOLID, 1, RGB(155, 155, 200));
			SelectObject(savedc1, hpen);
			hbrush = CreateSolidBrush(RGB(250, 250, 200));
			SelectObject(savedc1, hbrush);

			Rectangle(savedc1, 99 - WD_WIDTH / Board_num, 49, (101 - WD_WIDTH / Board_num) + 2 * WD_WIDTH / Board_num, 51 + 2 * WD_HEIGHT / Board_num);

			SelectObject(savedc1, oldpen);
			SelectObject(savedc1, oldbrush);
			DeleteObject(hpen);
			DeleteObject(hbrush);

			img[block[count + 1].Shape].TransparentBlt(savedc1, 100 - WD_WIDTH / Board_num, 50, 2 * WD_WIDTH / Board_num, 2 * WD_HEIGHT / Board_num, RGB(255, 255, 255));



			// savedc - Board 그리기
			hpen = CreatePen(PS_SOLID, 1, RGB(155, 200, 155));
			SelectObject(savedc, hpen);
			hbrush = CreateSolidBrush(RGB(250, 250, 200));
			SelectObject(savedc, hbrush);
			for (int i = 1; i <= Board_num; i++) {
				for (int j = 1; j <= Board_num; j++) {
					Rectangle(savedc, WD_WIDTH * (i - 1) / Board_num, WD_HEIGHT * (j - 1) / Board_num, WD_WIDTH * i / Board_num, WD_HEIGHT * j / Board_num);
				}
			}
			SelectObject(savedc, oldpen);
			SelectObject(savedc, oldbrush);
			DeleteObject(hpen);
			DeleteObject(hbrush);


			//새로운 블록 
			if (New_block) {
				block[count].T = TRUE;
				block[count + 1].Shape = rand() % 6;
				New_block = FALSE;

				if (block[count].Shape == lt) {
					block[count].l = FALSE;
					block[count].t = FALSE;
				}
				else if (block[count].Shape == tr) {
					block[count].r = FALSE;
					block[count].t = FALSE;
				}
				else if (block[count].Shape == rb) {
					block[count].r = FALSE;
					block[count].b = FALSE;
				}
				else if (block[count].Shape == bl) {
					block[count].l = FALSE;
					block[count].b = FALSE;
				}
				else if (block[count].Shape == lr) {
					block[count].l = FALSE;
					block[count].r = FALSE;
				}
				else if (block[count].Shape == tb) {
					block[count].b = FALSE;
					block[count].t = FALSE;
				}
				block[count].p = FALSE;
			}

			//모든 블록 그리기
			for (int i = 0; i <= count; i++) {
				if (block[i].T == TRUE && block[i].p == FALSE) {
					img[block[i].Shape].TransparentBlt(savedc, block[i].x, block[i].y, WD_WIDTH / Board_num, WD_HEIGHT / Board_num, RGB(255, 255, 255));
				}
			}

			// 연결 블록 
			for (int i = 0; i <= count; i++) {
				if (block[i].T == TRUE && block[i].p == TRUE) {
					img[block[i].Shape + 6].TransparentBlt(savedc, block[i].x, block[i].y, WD_WIDTH / Board_num, WD_HEIGHT / Board_num, RGB(255, 255, 255));
				}
			}

			//터진 블록
			for (int i = 0; i <= count; i++) {
				if (block[i].T == FALSE && block[i].p == TRUE) {
					img[block[i].Shape + 12].TransparentBlt(savedc, block[i].x, block[i].y, WD_WIDTH / Board_num, WD_HEIGHT / Board_num, RGB(255, 255, 255));
				}
			}

		}
		
		//savedc 가져오기
		BitBlt(hdc, 0, 0, WD_WIDTH, WD_HEIGHT, savedc, 0, 0, SRCCOPY);
		BitBlt(hdc, WD_WIDTH, 0, 200, WD_HEIGHT, savedc1, 0, 0, SRCCOPY);

		SelectObject(savedc, oldhbitmap);
		SelectObject(savedc1, oldhbitmap1);

		DeleteObject(Hbitmap);
		DeleteObject(Hbitmap1);

		DeleteObject(oldhbitmap);
		DeleteObject(oldhbitmap1);

		DeleteObject(oldpen);
		DeleteObject(oldbrush);
		
		DeleteDC(savedc);
		DeleteDC(savedc1);
		DeleteDC(hdc);
		
		EndPaint(hWnd, &ps);
		break;

		//키조작
	case WM_KEYDOWN:


		switch (wParam) {

		case VK_LEFT: {
			bool check = TRUE;
			if (block[count].x > 0) {
				for (int i = 0; i < count; i++) {
					if (block[count].x - WD_WIDTH / Board_num == block[i].x && block[count].y + WD_WIDTH / Board_num >= block[i].y) {
						check = FALSE;
					}
				}

				if (check)
					block[count].x -= WD_WIDTH / Board_num;
			}
		}
					  break;



		case VK_RIGHT: {
			bool check = TRUE;
			if (block[count].x < WD_WIDTH - WD_WIDTH / Board_num) {
				for (int i = 0; i < count; i++) {
					if (block[count].x + WD_WIDTH / Board_num == block[i].x && block[count].y + WD_WIDTH / Board_num >= block[i].y) {
						check = FALSE;
					}
				}

				if (check)
					block[count].x += WD_WIDTH / Board_num;
			}
		}
					   break;






		case VK_SPACE:
			if (block[count].Shape == lt) {
				block[count].Shape = tr;
				block[count].l = TRUE;
				block[count].t = FALSE;
				block[count].r = FALSE;
				block[count].b = TRUE;
			}

			else if (block[count].Shape == tr) {
				block[count].Shape = rb;
				block[count].l = TRUE;
				block[count].t = TRUE;
				block[count].r = FALSE;
				block[count].b = FALSE;
			}

			else if (block[count].Shape == rb) {
				block[count].Shape = bl;
				block[count].l = FALSE;
				block[count].t = TRUE;
				block[count].r = TRUE;
				block[count].b = FALSE;
			}

			else if (block[count].Shape == bl) {
				block[count].Shape = lt;
				block[count].l = FALSE;
				block[count].t = FALSE;
				block[count].r = TRUE;
				block[count].b = TRUE;
			}

			else if (block[count].Shape == tb) {
				block[count].Shape = lr;
				block[count].l = FALSE;
				block[count].t = TRUE;
				block[count].r = FALSE;
				block[count].b = TRUE;
			}

			else if (block[count].Shape == lr) {
				block[count].Shape = tb;
				block[count].l = TRUE;
				block[count].t = FALSE;
				block[count].r = TRUE;
				block[count].b = FALSE;
			}
			break;

		case 'q':	case 'Q':
			exit(1);
			break;

		case 's':	case 'S':
			if (sw == FALSE) {
				KillTimer(hWnd, 1);
				sw = TRUE;
			}
			break;
		
		

		}


		InvalidateRect(hWnd, NULL, FALSE);
		break;

		case WM_KEYUP:
			switch (wParam) {
			
			case VK_DOWN:
				if(st_check == FALSE){
					st_check = TRUE;

					if (block[count].y < WD_HEIGHT - 3 * (WD_HEIGHT / Board_num)) {

						if (count == 0) {
							GO = 1;
						}

						else if (count > 0) {
							for (int i = count - 1; i >= 0; i--) {
								if (block[i].x == block[count].x) {
									if (block[i].y - 5 * (WD_HEIGHT / Board_num) < block[count].y) {
										GO = 2;
									}
								}
							}
							if (GO == 0)
								GO = 1;
						}

						if (GO == 1) 
							block[count].y += 10;
		
						GO = 0;
					}
					st_check = FALSE;
				}
				
				break;
			}
			break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (770 <= mx && mx <= 870 && 100 <= my && my <= 200)	size = Small_size;
		else if (770 <= mx && mx <= 870 && 300 <= my && my <= 400)	size = Mid_size;
		else if (770 <= mx && mx <= 870 && 500 <= my && my <= 600)	size = Big_size;
		else size = 0;
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP:
		if (size != 0) {
			Board_num = size;
			start = FALSE;
			// SET 타이머
			SetTimer(hWnd, 1, 10, NULL);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		//KillTimer(hWnd, 2);

		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//enum SHAPE { lt, tr, rb, bl, tb, lr };
void Check(Block b1,Block b2) {
	if (b1.p == TRUE && b2.p == TRUE) {

	}
}