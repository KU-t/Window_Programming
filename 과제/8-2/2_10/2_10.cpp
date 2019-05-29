#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <stdio.h>

#define WD_left 1200
#define WD_top 1200
int Cell_size = 30; // 한칸 크기

int Board_size = 600; // 보드 총 크기

int figure_num ;

static bool show;

int before;

int ch;

HDC hdc;
HBRUSH hbrush, oldbrush;

int choose ;
void draw();

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";


LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

class Figure {
public:
	POINT point[3];
	int x1, y1, x2, y2, z1, z2,z3,z4;
	int num, figure;
	COLORREF color;

	void make() {
		x1 = (rand() % ((Board_size - Cell_size) / Cell_size) * Cell_size) ;
		y1 = (rand() % ((Board_size - Cell_size) / Cell_size) * Cell_size);
		x2 = x1 + Cell_size;
		y2 = y1 + Cell_size;
		z1 = (rand() % ((Board_size - Cell_size) / Cell_size) * Cell_size);
		z2 = (rand() % ((Board_size - Cell_size) / Cell_size) * Cell_size);
		z3 = z1 + Cell_size;
		z4 = z2;
		POINT tri_point[3] = { { z1,z2 },{z3 ,z4 },{ (z1 + z3) / 2 , z2+(z3 - z1) } };
		//POINT tri_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
		for (int i = 0; i < 3; i++)
			point[i] = tri_point[i];
		color = RGB(rand() % 255, rand() % 255, rand() % 255);
	}

	void change() {
		
			x1 = x1 / before * Cell_size;
			y1 = y1 / before * Cell_size;
			x2 = x1 + Cell_size;
			y2 = y1 + Cell_size;
			
			z1 = z1 / before * Cell_size;
			z2 = z2 / before * Cell_size;
			z3 = z1 + Cell_size;
			z4 = z2;
			POINT tri1_point[3] = { { z1,z2 },{ z3 ,z4 },{ (z1 + z3) / 2 ,z2 + (z3 - z1) } };
			//POINT tri1_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
			for (int i = 0; i < 3; i++)
				point[i] = tri1_point[i];
		
	}

	void size(int n) {
		switch (n) {
		case 1:
			if( z1 > 0 && z2 > 0) {
				z1 -= Cell_size;
				z2 -= Cell_size;
				z4 -= Cell_size;
				POINT tri1_point[3] = { { z1,z2 },{ z3 ,z4 },{ (z1 + z3) / 2 ,z2 + (z3 - z1) } };
				//POINT tri1_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
				for (int i = 0; i < 3; i++)
					point[i] = tri1_point[i];
			}

			else if (z1 == 0 && z2 > 0) {
				z2 -= Cell_size;
				z3 += Cell_size;
				z4 -= Cell_size;
				POINT tri2_point[3] = { { z1,z2 },{ z3 ,z4 },{ (z1 + z3) / 2 ,z2 + (z3 - z1) } };
				//POINT tri1_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
				for (int i = 0; i < 3; i++)
						point[i] = tri2_point[i];
			}

			

			else if (z1 > 0 && z2 == 0) {
				z1 -= Cell_size;
				POINT tri3_point[3] = { { z1,z2 },{ z3 ,z4 },{ (z1 + z3) / 2 ,z2 + (z3 - z1) } };
				//POINT tri1_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
				for (int i = 0; i < 3; i++)
					point[i] = tri3_point[i];
			}
			else if (z1==0 && z2==0) {
				if (z3 < Board_size - Cell_size) {
					z3 += Cell_size;
					POINT tri2_point[3] = { { z1,z2 },{ z3 ,z4 },{ (z1 + z3) / 2 ,z2 + (z3 - z1) } };
					//POINT tri1_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
					for (int i = 0; i < 3; i++)
						point[i] = tri2_point[i];
				}
				
			}
			
			break;
			
		case 2:
			if (z3 - z1 != Cell_size) {
				z3 -= Cell_size;
				POINT tri5_point[3] = { { z1,z2 },{ z3 ,z4 },{ (z1 + z3) / 2 ,z2 + (z3 - z1) } };
				//POINT tri1_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
				for (int i = 0; i < 3; i++)
					point[i] = tri5_point[i];
			}
			break;
		}
	}

	void move(int n) {
		switch (n) {
		case 1:
			if (z2 > 0) {
				z2 -= Cell_size;
				z4 -= Cell_size;
				POINT tri1_point[3] = { { z1,z2 },{ z3 ,z4 },{ (z1 + z3) / 2 ,z2 + (z3 - z1) } };
				//POINT tri1_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
				for (int i = 0; i < 3; i++)
					point[i] = tri1_point[i];
			}
			break;

		case 2:
			if ( z2 + (z3 - z1) < Board_size-Cell_size) {
				z2 += Cell_size;
				z4 += Cell_size;
				POINT tri2_point[3] = { { z1,z2 },{ z3 ,z4 },{ (z1 + z3) / 2 ,z2 + (z3 - z1) } };
				//POINT tri2_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
				for (int i = 0; i < 3; i++)
					point[i] = tri2_point[i];
			}
			break;

		case 3:
			if (z1 > 0) {
				z1 -= Cell_size;
				z3 -= Cell_size;
				POINT tri3_point[3] = { { z1,z2 },{ z3 ,z4 },{ (z1 + z3) / 2 ,z2 + (z3 - z1) } };

				//POINT tri3_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
				for (int i = 0; i < 3; i++)
					point[i] = tri3_point[i];
			}
			break;

		case 4:
			if (z3 < Board_size-Cell_size) {
				z1 += Cell_size;
				z3 += Cell_size;
				POINT tri4_point[3] = { { z1,z2 },{ z3 ,z4 },{ (z1 + z3) / 2 ,z2 + (z3 - z1) } };

				//POINT tri4_point[3] = { { z1 + (Cell_size / 2),z2 },{ z1,z2 + Cell_size },{ z1 + Cell_size,z2 + Cell_size } };
				for (int i = 0; i < 3; i++)
					point[i] = tri4_point[i];
			}
			break;
		}
	}

	void draw() {

		hbrush = CreateSolidBrush(color);
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

		switch (figure) {
		case 1:
			Ellipse(hdc, x1, y1, x2, y2);
			break;

		case 2:
			Polygon(hdc, point, 3);
			break;

		case 3:
			Rectangle(hdc, x1, y1, x2, y2);
			break;
		}

		SelectObject(hdc, oldbrush);
		DeleteObject(hbrush);
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow) {

	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;




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
	static Figure F[5];
	PAINTSTRUCT ps;
	HPEN hpen, oldpen;

	FILE* fp;

	switch (uMsg) {
	case WM_CREATE:
		show = FALSE;
		figure_num = 0;
		choose = 0;
		break;

	case WM_CHAR:
		switch (wParam) {
		case 's':		case 'S':
			before = Cell_size;
			Cell_size = 20;			
			for (int i = 0; i < figure_num; i++)
				F[i].change();
			break;

		case 'm':		case 'M':
			before = Cell_size;
			Cell_size = 15;
			for (int i = 0; i < figure_num; i++)
				F[i].change();
			break;

		case 'l':		case 'L':
			before = Cell_size;
			Cell_size = 12;
			for (int i = 0; i < figure_num; i++)
				F[i].change();
			break;

		case 'e':		case 'E':
			if (figure_num < 5 && figure_num>=0) {
				F[figure_num].make();
				F[figure_num].figure = 1;
				figure_num++;
			}

			break;

		case 'T':		case 't':
			if (figure_num < 5 && figure_num >= 0) {
				F[figure_num].make();
				F[figure_num].figure = 2;
				figure_num++;
			}
			break;


		case 'R':		case 'r':
			if (figure_num < 5 && figure_num >= 0) {
				F[figure_num].make();
				F[figure_num].figure = 3;
				figure_num++;
			}
			break;

		case '1':	
			if(figure_num >= 1)
			choose = 0;
			break;

		case '2':	
			if (figure_num >= 2)
			choose = 1;
			break;

		case '3':	
			if (figure_num >= 3)
				choose = 2;
			break;

		case '4':	
			if (figure_num >= 4)
				choose = 3;
			break;

		case '5':	
			if (figure_num >= 5)
				choose = 4;
			break;

		case '+':
		case '=':
			if (F[choose].figure == 2) {
				F[choose].size(1);
			}

			else if (F[choose].figure != 2) {
				if (F[choose].x1 > 0) {
					F[choose].x1 -= Cell_size;
					F[choose].y1 -= Cell_size;
				}
				else if (F[choose].y2 < Board_size - Cell_size) {
					F[choose].x2 += Cell_size;
					F[choose].y2 += Cell_size;
				}
				else if (F[choose].y1 > 0) {
					F[choose].y1 -= Cell_size;
					F[choose].x2 += Cell_size;
				}
				else if (F[choose].x1 > 0) {
					F[choose].x1 -= Cell_size;
					F[choose].y2 += Cell_size;
				}
			}
			
			
			break;

		case '-':
		case '_':
			if (F[choose].figure == 2) {
				F[choose].size(2);
			
			}
			else if (F[choose].figure != 2) {
				if (F[choose].x2 - F[choose].x1 != 30) {
					F[choose].x2 -= Cell_size;
					F[choose].y2 -= Cell_size;
				}
			}
			break;


		case 'q':
		case 'Q':
			exit(1);

		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_UP:
			if (F[choose].figure == 2) {
				F[choose].move(1);
			}
			else if (F[choose].figure != 2) {
				if (F[choose].y1 > 0) {
					F[choose].y1 -= Cell_size;
					F[choose].y2 -= Cell_size;
				}
			}
			break;

		case VK_DOWN:
			if (F[choose].figure == 2) {
				F[choose].move(2);
			}
			else if (F[choose].figure != 2) {
				if (F[choose].y2 < Board_size-30) {
					F[choose].y1 += Cell_size;
					F[choose].y2 += Cell_size;
				}
			}
			break;

		case VK_LEFT:
			if (F[choose].figure == 2) {
				F[choose].move(3);
			}
			else if (F[choose].figure != 2) {
				if (F[choose].x1 > 0) {
					F[choose].x1 -= Cell_size;
					F[choose].x2 -= Cell_size;
				}
			}
			break;

		case VK_RIGHT:
			if (F[choose].figure == 2) {
				F[choose].move(4);
			}
			else if (F[choose].figure != 2) {
				if (F[choose].x2 < Board_size-30) {
					F[choose].x1 += Cell_size;
					F[choose].x2 += Cell_size;
				}
			}
			break;
			
		case VK_DELETE:
			if (figure_num > 0) {
				
					for (int i = choose; i < 5; i++)
						F[i] = F[i + 1];
					figure_num--;
					if (figure_num <= choose)
						choose = figure_num-1;
				
			}
			break;

		case VK_F1:
			
			fp = fopen("data.txt", "r");
			fscanf(fp, "%d", &choose);
			fscanf(fp, "%d", &before);
			fscanf(fp, "%d", &figure_num);
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 3; j++) {
					fscanf(fp, "%d", &F[i].point[j].x);
					fscanf(fp, "%d", &F[i].point[j].y);
				}
				fscanf(fp, "%d", &F[i].x1);
				fscanf(fp, "%d", &F[i].y1);
				fscanf(fp, "%d", &F[i].x2);
				fscanf(fp, "%d", &F[i].y2);
				fscanf(fp, "%d", &F[i].z1);
				fscanf(fp, "%d", &F[i].z2);
				fscanf(fp, "%d", &F[i].z3);
				fscanf(fp, "%d", &F[i].z4);
				fscanf(fp, "%d", &F[i].num);
				fscanf(fp, "%d", &F[i].figure);
				fscanf(fp, "%d", &F[i].color);
			}
			

			fclose(fp);
			break;

			/*POINT point[3];
			int x1, y1, x2, y2, z1, z2, z3, z4;
			int num, figure;
			COLORREF color;*/

		case VK_F2:
			
			fp = fopen("data.txt", "w");
			fprintf(fp, "%d\n", choose);
			fprintf(fp, "%d\n", before);
			fprintf(fp, "%d\n", figure_num);
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 3; j++) {
					fprintf(fp, "%d\n", F[i].point[j].x);
					fprintf(fp, "%d\n", F[i].point[j].y);
				}
				fprintf(fp, "%d\n", F[i].x1);
				fprintf(fp, "%d\n", F[i].y1);
				fprintf(fp, "%d\n", F[i].x2);
				fprintf(fp, "%d\n", F[i].y2);
				fprintf(fp, "%d\n", F[i].z1);
				fprintf(fp, "%d\n", F[i].z2);
				fprintf(fp, "%d\n", F[i].z3);
				fprintf(fp, "%d\n", F[i].z4);
				fprintf(fp, "%d\n", F[i].num);
				fprintf(fp, "%d\n", F[i].figure);
				fprintf(fp, "%d\n", F[i].color);
			}
			

			fclose(fp);

			show = TRUE;
			break;

		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		draw();

		for (int i = 0; i < figure_num; i++) {
			if (choose == i) {
				ch = i;
			}
			else F[i].draw();
		}
		hpen = CreatePen(PS_SOLID, 10, RGB(255, 0, 0));
		oldpen = (HPEN)SelectObject(hdc, hpen);
		F[ch].draw();
		SelectObject(hdc, oldpen);
		DeleteObject(hpen);
		F[choose].draw();


		if (figure_num == 0) {
			draw();
			F[0] = F[1];
			choose = 0;
		}
		if (show) {
			TextOut(hdc, 600, 600, "저장 완료", 9);
			show = FALSE;
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
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