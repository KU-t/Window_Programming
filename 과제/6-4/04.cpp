#include <Windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DiagProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void getBinary(int num, char bin[], int* count);
int getDecimal(char bin[]);

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
	//WndClass.lpszMenuName = MAKEINTRESOURCE (IDR_MENU1);
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "Window API", WS_OVERLAPPEDWINDOW, 0, 0, 500, 300, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

bool exitCheck;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
	case WM_CREATE:
		exitCheck = FALSE;

		SetTimer(hWnd, 1, 100, NULL);
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DiagProc1);
		break;
	case WM_TIMER:
		if (exitCheck)
		{
			KillTimer(hWnd, 1);
			PostQuitMessage(0);
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
	static char str[50];
	static int count;
	static int number;
	static char opr;
	static bool resCheck;
	static int numCheck;
	static int result;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		count = 0;
		opr = 0;
		result = 0;
		resCheck = FALSE;
		numCheck = 10;
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_NUM0:
			if (numCheck == 2)
			{
				numCheck = 10;
				count = 0;
			}
			if (count < 50)
			{
				str[count++] = '0';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM1:
			if (numCheck == 2)
			{
				numCheck = 10;
				count = 0;
			}
			if (count < 50)
			{
				str[count++] = '1';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM2:
			if (numCheck == 2)
			{
				numCheck = 10;
				count = 0;
			}
			if (count < 50)
			{
				str[count++] = '2';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM3:
			if (numCheck == 2)
			{
				numCheck = 10;
				count = 0;
			}
			if (count < 50)
			{
				str[count++] = '3';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM4:
			if (numCheck == 2)
			{
				numCheck = 10;
				count = 0;
			}
			if (count < 50)
			{
				str[count++] = '4';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM5:
			if (numCheck == 2)
			{
				numCheck = 10;
				count = 0;
			}
			if (count < 50)
			{
				str[count++] = '5';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM6:
			if (numCheck == 2)
			{
				numCheck = 10;
				count = 0;
			}
			if (count < 50)
			{
				str[count++] = '6';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM7:
			if (numCheck == 2)
			{
				numCheck = 10;
				count = 0;
			}
			if (count < 50)
			{
				str[count++] = '7';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM8:
			if (numCheck == 2)
			{
				numCheck = 10;
				count = 0;
			}
			if (count < 50)
			{
				str[count++] = '8';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM9:
			if (numCheck == 2)
			{
				numCheck = 10;
				count = 0;
			}
			if (count < 50)
			{
				str[count++] = '9';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_EQUAL:
			if (!resCheck)
			{
				int i = 0;
				number = 0;
				opr = 0;

				while (str[i] != 0)
				{
					if ('0' <= str[i] && str[i] <= '9')
					{
						number = (number * 10) + (int)(str[i] - 48);
					}
					else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
					{
						if (opr == 0)
						{
							result = number;
							number = 0;
						}
						else if (opr == '+')
						{
							result += number;
							number = 0;
						}
						else if (opr == '-')
						{
							result -= number;
							number = 0;
						}
						else if (opr == '*')
						{
							result *= number;
							number = 0;
						}
						else if (opr == '/')
						{
							result /= number;
							number = 0;
						}

						opr = str[i];
					}

					i++;
				}

				if (opr == 0)
					result = number;
				else if (opr == '+')
					result += number;
				else if (opr == '-')
					result -= number;
				else if (opr == '*')
					result *= number;
				else if (opr == '/')
					result /= number;

				itoa(result, str, 10);
				count = strlen(str);

				resCheck = TRUE;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_ADD:
			if (count < 50)
			{
				str[count++] = '+';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_SUB:
			if (count < 50)
			{
				str[count++] = '-';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_DIV:
			if (count < 50)
			{
				str[count++] = '/';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_MUL:
			if (count < 50)
			{
				str[count++] = '*';
				str[count] = 0;
			}
			if (resCheck)
				resCheck = FALSE;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_REVERSE:
			if (count > 0)
			{
				int oper[3];
				int op_count = 0;
				char temp;

				oper[0] = count;
				oper[1] = count;
				oper[2] = count;

				for (int i = 0; i < count; i++)
				{
					if (str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '*') {
						oper[op_count++] = i;
					}
				}

				if(!(oper[0] == count)) {
					for (int i = 0; i < oper[0] / 2; i++) {
						temp = str[i];
						str[i] = str[oper[0] - 1 - i];
						str[oper[0] - 1 - i] = temp;
					}

					/*if (!(oper[1] == count)) {
						for (int i = 0; i < oper[0] / 2; i++) {
							temp = str[i];
							str[i] = str[oper[0] - 1 - i];
							str[oper[0] - 1 - i] = temp;
						}
					}*/
						
					if (oper[1] == count) {
						for (int i = oper[0]; i < count; i++) {
							temp = str[i];
							str[i] = str[count - 1 - i];
							str[count - 1 - i] = temp;
						}
					}
				}

				if (!(oper[0] == count)) {
					for (int i = 0; i < oper[0] / 2; i++) {
						temp = str[i];
						str[i] = str[oper[0] - 1 - i];
						str[oper[0] - 1 - i] = temp;
					}

					/*if (!(oper[1] == count)) {
					for (int i = 0; i < oper[0] / 2; i++) {
					temp = str[i];
					str[i] = str[oper[0] - 1 - i];
					str[oper[0] - 1 - i] = temp;
					}
					}*/

					if (oper[1] == count) {
						for (int i = oper[0]; i < count; i++) {
							temp = str[i];
							str[i] = str[count - 1 - i];
							str[count - 1 - i] = temp;
						}
					}
				}

				/*for (int j = 0; j <= op_count; j++) {
					if (!(oper[j] == count)) {
						for (int i = 0; i < oper[j]/2; i++) {
							temp = str[i];
							str[i] = str[oper[j] - 1 - i];
							str[oper[j] - 1 - i] = temp;
						}
						for (int i = oper[j]; i < count; i++) {
							temp = str[i];
							str[i] = str[count - 1 - i];
							str[count - 1 - i] = temp;
						}
					}
				}*/

				/*for (int j = 0; j <= count; j++) {
					if (!(oper[j] == count)) {
						for (int i = 0; i < oper[j] / 2; i++) {
							temp = str[i];
							str[i] = str[oper[j] - 1 - i];
							str[oper[j] - 1 - i] = temp;
						}
					}
				}*/
				
				if (oper[0] == count && oper[1] == count && oper[2] == count) {
					for (int i = 0; i < count/2; i++) {
						temp = str[i];
						str[i] = str[count - 1 - i];
						str[count - 1 - i] = temp;
					}
				}

				
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;

		case IDC_CE:
			if (count > 0)
			{
				int i = count;
				while (i>0)
				{
					if (str[i - 1] == 43 || str[i - 1] == 45 || str[i - 1] == 42 || str[i - 1] == 47)
						break;
					--i;
					str[i] = 0;
				}
				count = i;

			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_BINARY:
			if (numCheck == 10)
			{
				char bin[50];
				int num = atoi(str);
				int count = 0;
				getBinary(num, bin, &count);

				strcpy(str, bin);

				numCheck = 2;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_DECIMAL:
			if (count > 0)
			{
				count--;
				str[count] = 0;

				if (str[count - 1] == '+' || str[count - 1] == '-' || str[count - 1] == '*' || str[count - 1] == '/')
				{
					count--;
					str[count] = 0;
				}
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_TEN:
			if (count > 0)
			{
				str[count++] = '0';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_RESET:
			count = 0;
			str[count] = 0;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_EXIT:
			exitCheck = TRUE;
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}
void getBinary(int num, char bin[], int* count)
{
	if (num == 1)
		bin[(*count)++] = '1';
	else if (num == 0)
		bin[(*count)++] = '0';
	else
	{
		getBinary(num / 2, bin, count);
		bin[(*count)++] = num % 2 + 48;
	}

	bin[*count] = 0;
	return;
}
int getDecimal(char bin[])
{
	int i = strlen(bin) - 1;
	int result;

	if (bin[i--] == '1') result = 1;
	else result = 0;

	for (; i >= 0; i--)
	{
		if (bin[i] == '1')
		{
			int num = 1;
			for (int n = 0; n < strlen(bin) - 1 - i; n++)
				num *= 2;
			result += num;
		}
	}

	return result;
}