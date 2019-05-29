#include <windows.h>
#include <tchar.h>

#define WD_left 1500
#define WD_top 300


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

	hWnd = CreateWindow(lpszClass, "window API", WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, 0, 0, WD_left, WD_top, NULL, (HMENU)NULL, hInstance, NULL);

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
	HDC hdc;
	PAINTSTRUCT ps;
	SIZE size;
	static int count;
	static int n = 0, m[10] = {}; // 배열자리
	static char array[10][101];
	static char array_2[10][110];
	static char array_3[10][110];
	static char array_nu[1][110];
	static char array_4[10][110];
	static int Switch = 0;
	static int Switch_1 = 0;
	static int Switch_2 = 0;
	static int Switch_3 = 0;
	static int Switch_4 = 0;
	static int Switch_pd = 0;
	static int Switch_pu = 0;
	static int Switch_P = 0;
	static int Switch_M = 0;
	int pu = 0;
	int be = 0, af = 0;
	static int N = 0;
	static int Nn = 0;
	static int BE = 0, AF = 0;

	switch (uMsg) {
	case WM_CREATE:
		count = 0;
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;

	case WM_CHAR:
		if (n < 10) {
			if ((wParam >= 32 && wParam <= 42) || (wParam >= 46 && wParam <= 126) || wParam == 44) {

				hdc = BeginPaint(hWnd, &ps);

				

				if (count <= 99) {
					for (int i = 1; i < count; i++) {
						if (array[n][i] == '\0') {
							array[n][i] = ' ';
						}
					}

					if (Switch_1 == 0) {
						
						if (Switch == 0) {
							array[n][count] = wParam;
							array[n][count+1] = '\0';
							array_2[n][count] = array[n][count];
							array_2[n][count + 1] = array[n][count + 1];
							array_3[n][count] = array[n][count];
							array_3[n][count + 1] = array[n][count + 1];
							count++;
						}

						else {

							if (wParam >= 97 && wParam <= 122) {
								array[n][count] = wParam;
								array[n][count] = array[n][count] - 32;
								array[n][count+1] = '\0';
								array_2[n][count] = array[n][count];
								array_2[n][count + 1] = array[n][count + 1];
								array_3[n][count] = array[n][count];
								array_3[n][count + 1] = array[n][count + 1];
								count++;
							}

							else {
								array[n][count] = wParam;
								array[n][count+1] = '\0';
								array_2[n][count] = array[n][count];
								array_2[n][count + 1] = array[n][count + 1];
								array_3[n][count] = array[n][count];
								array_3[n][count + 1] = array[n][count + 1];
								count++;
							}
						}
					}

					else {
						array[n][count] = '*';
						array[n][count+1] = '\0';
						array_2[n][count] = array[n][count];
						array_2[n][count + 1] = array[n][count + 1];
						array_3[n][count] = array[n][count];
						array_3[n][count + 1] = array[n][count + 1];
						count++;
					}
					
				}

				m[n] = count;

				InvalidateRect(hWnd, NULL, TRUE);
				EndPaint(hWnd, &ps);
			}

			if (wParam == '+') {
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 101; j++) {
						if (array_3[i][j] <= 56 && array_3[i][j] >= 48) {
							array_3[i][j] += 1;
						}
					}
				}
			}

			else if (wParam == '-') {
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 101; j++) {
						if (array_3[i][j] <= 57 && array_3[i][j] >= 49) {
							array_3[i][j] -= 1;
						}
					}
				}
			}
		}

		break;

	case WM_KEYDOWN:

		hdc = BeginPaint(hWnd, &ps);



		switch (wParam) {
		case VK_F1:
			if (Switch_1 == 0)
				Switch_1 = 1;
			else
				Switch_1 = 0;

			break;

		case VK_F2:
			
			if (Switch_2 == 0) {
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 101; j++) {
						if (array[i][j] <= 57 && array[i][j] >= 48) {
							array_2[i][j] = '0';
							array_3[i][j] = array_2[i][j];
						}
							
						else {
							if (!(array[i][j] == 'e' || array[i][j] == 'E')) {
								array_2[i][j] = array[i][j];
								array_3[i][j] = array_2[i][j];
							}
						}
					}
				}
			}

			else {
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 101; j++) {
						
						if (array_2[i][j] == 0) {
							array_2[i][j] = array[i][j];
							array_3[i][j] = array_2[i][j];
						}
						
						else {
							if (!(array[i][j] == 'e' || array[i][j] == 'E')) {
								array_2[i][j] = array[i][j];
								array_3[i][j] = array_2[i][j];
							}
							
						}

					}	
				}
			}
			

			if (Switch_2 == 0)
				Switch_2 = 1;
			else
				Switch_2 = 0;

			break;

		case VK_F3:
			if (Switch_3 == 0) {
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 101; j++) {
						if (array[i][j] == 'e' || array[i][j] == 'E') {
							array_2[i][j] = '#';
							array_3[i][j] = array_2[i][j];
						}
						else {
							if (!(array[i][j] <= 57 && array[i][j] >= 48)) {
								array_2[i][j] = array[i][j];
								array_3[i][j] = array_2[i][j];
							}
								

						}
					}
				}
			}
			else {
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 101; j++) 
						if (!(array[i][j] <= 57 && array[i][j] >= 48)) {
							array_2[i][j] = array[i][j];
							array_3[i][j] = array_2[i][j];
						}
				}
			}

			if (Switch_3 == 0)
				Switch_3 = 1;
			else
				Switch_3 = 0;
			break;

		case VK_F5:

			for (int i = 100; i < 110; i++) {
				for (int j = 0; j < 10; j++) {
					array_3[j][i] = '\0';
					array_4[j][i] = '\0';
				}
			}
				
			if (array_3[n][count] != ' ' || array_3[n][count] != '\0') {
				//복
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 100; j++) {
						array_4[i][j] = array_3[i][j];
					}
				}
				//널
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 100; j++) {
						array_3[i][j] = ' ';
					}
				}
		
				for (int i = 0; i < count; i++) {
					if (count - i != 0) {
						if (array_4[n][count - i] == ' ' || array_4[n][count - i] == '\0') {
							be = count - i;
							BE = 1;
							break;
						}
					}

					else {
						be = 0;
						BE = 0;
					}


					for (int i = 0; i < 100 - count; i++) {
						if (array_4[n][count + i] == ' ' || array_4[n][count + i] == '\0') {
							af = count + i;
							AF = 1;
							break;
						}
					}
				}

				if (BE != 0 || AF != 0) {
					for (int i = be; i < 99; i++) {

						array_4[n][i] = array_4[n][af - be + i];
						//array_3[n][i] = array_4[n][i];
						array[n][i] = array_4[n][i];
					}
					//
					for (int i = 0; i < 10; i++) {
						for (int j = 0; j < 100; j++) {
							array_3[i][j] = array_4[i][j];
						}
					}
				}
				else {
					for (int j = 0; j < 100; j++) {
						array_nu[n][j] = ' ';
					}
				}


				count -= (af - be - 1);
				if (count < 0) {
					count = 0;
				}
				else if (count > 100) {
					count = 100;
				}
				be = 0;
				af = 0;
				AF = 0;
				BE = 0;
			}
			break;

		case VK_PRIOR:

			if (Switch_pd == 0) {
				//맨앞 (
				for (int i = 0; i <= n; i++) {
					for (int j = 99; j >= 0; j--) {
						array_3[i][j + 1] = array_3[i][j];
					}
					array_3[i][0] = '(';
				}

				
				for (int i = 0; i <= n; i++) {
					for (int j = 1; j < 100; j++) {
						if (array_3[i][j] == ' ') {
							for (int k = 98; k > j; k--) {
								array_3[i][k + 1] = array_3[i][k];
							}

							array_3[i][j] = ')';
							array_3[i][j + 1] = '(';
						}

						else if (array_3[i][j] == '\0') {
							array_3[i][j + 1] = '\0';
							array_3[i][j] = ')';
							break;
						}
					}
				}
				for (int i = 0; i < 10; i++) {
					array_3[i][100] = '\0';
				}
				Switch_pd = 1;
			}

			else if (Switch_pd == 1) {
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 99; j++) {
						array_3[i][j] = array_2[i][j];
					}
				} 
				Switch_pd = 0;
			}
			


			break;

		case VK_NEXT:
			

			if (Switch_pd == 0) {
			
				for (int i = 0; i <= n; i++) {
					for (int j = 1; j < 100; j++) {
						if (array_3[i][j] == ' ') {
							for (int k = j; k < 100; k++) {
								array_3[i][k] = array_3[i][k+1];
							}
							
							for (int l = j+1; l < 100; l++) {
								if (array_3[i][l] == ' ')
									pu = l - j+1;
								else
									break;
							}
							for (int k = j; k < 100; k++) {
								array_3[i][k] = array_3[i][k + pu];
							}
									
						}

						else if (array_3[i][j] == '\0') {
							array_3[i][j + 1] = array_3[i][j];
							
							break;
						}
					}
				}

				Switch_pd = 1;
			}

			else if (Switch_pd == 1) {
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 100; j++) {
						array_3[i][j] = array_2[i][j];
					}
				}
				Switch_pd = 0;
			}



			break;
			break;

		case VK_RETURN: // 엔터

			if (n < 9) {
				n++;
				m[n] = count;
				count = 0;
				N++;
			}

			break;

		case VK_BACK: // 백스페이스
		    //앞에 \0
			if (count > 0) {

				if (array_3[n][count] == '\0') {
					array_3[n][--count] = '\0';
				}


				else {
					for (int i = count; i < 100; i++) {
						array_3[n][i - 1] = array_3[n][i];

					}
					count--;
				}
			}

			//첫글자에서 지울때
			else if (count == 0) {
				if (n > 0)
					count = m[--n];
			}

			break;

		case VK_ESCAPE: // ESC
			n = 0;	count = 0;
			for (int i = 0; i < 10; i++) {
				for (int j = 1; j < 99; j++) {
					array_3[i][0] = '\0';
					array_3[i][j] = ' ';
					array_2[i][0] = '\0';
					array_2[i][j] = ' ';
					array[i][0] = '\0';
					array[i][j] = ' ';
				}
			}

			break;

		case VK_TAB: // 탭
			for (int i = 0; i < 4; i++)
				if (count<100)
				array_3[n][count++] = ' ';
			break;

		case VK_HOME: // 홈
			count = 0;
			break;

		case VK_DELETE://Del
			count = 0;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 100; j++) {
					array_nu[0][j] = ' ';
				}
			}
			for (int i = n; i < 10; i++) {
				wsprintf(array_3[i], array_3[i + 1]);
			}
			for (int i = N; i < 10; i++) {
				for (int j = 0; j < 100; j++) {
					array_3[N][j] = ' ';
					array_2[N][j] = ' ';
					array[N][j] = ' ';
					Nn++;
				}
			}
			N -= Nn;
				
			break;

		case VK_LEFT: //왼쪽
			if (count>0)
				count--;
			break;

		case VK_UP: //위쪽
			if (n>0)
				n--;
			break;

		case VK_RIGHT: //오른쪽
			if (count<100)
				count++;
			break;

		case VK_DOWN:// 아래쪽
			if (n<10)
				n++;
			break;

		case VK_CAPITAL: // tab
			if (Switch == 0)
				Switch = 1;
			else
				Switch = 0;

			break;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		EndPaint(hWnd, &ps);

		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		
		//테두리
		/*
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 102; i++)
				TextOut(hdc, 13 * i, 156 * j, "-", 1);
			for (int i = 0; i < 10; i++)
				TextOut(hdc, 1313 * j, (15 * i) + 10, "|", 1);
		}
		*/

		//입력값
		for (int i = 0; i < 10; i++)
			TextOut(hdc, 15, 15 * (i + 1), array_3[i], strlen(array_3[i]));
		

		 
		//캐럿출력
		GetTextExtentPoint(hdc, array[n], count, &size);
		if (n < 10) {
			if (count != 0)
				SetCaretPos(size.cx + 15, size.cy + (15 * n));
			else
				SetCaretPos(size.cx + 15, size.cy + (15 * (n + 1)));
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}