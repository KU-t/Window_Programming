#pragma comment(lib, "msimg32.lib")
#include <Windows.h>
#include <iostream>
#include <string>
#include "resource.h"

#define WIDTH 1000
#define HEIGHT 800
HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

RECT win_size;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM IParam);
BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

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

	hWnd = CreateWindow(lpszClass, "4-3", WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, memDC;
	HBITMAP hBitmap;


	switch (iMessage)
	{

	case WM_CREATE:
		GetClientRect(hWnd, &win_size);
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
		return 0;

	//case WM_COMMAND:
	//	switch (LOWORD(wParam))
	//	{
	//		
	//	case IDD_DIALOG1:
	//		break;
	//	}
	//	break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, win_size.right, win_size.bottom);
		SelectObject(memDC, (HBITMAP)hBitmap);
		PatBlt(memDC, win_size.left, win_size.top, win_size.right, win_size.bottom, WHITENESS);
		//---------------------------------------------------------------------------------





		//---------------------------------------------------------------------------------
		BitBlt(hDC, 0, 0, win_size.right, win_size.bottom, memDC, 0, 0, SRCCOPY);
		DeleteDC(memDC);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		InvalidateRect(hWnd, NULL, FALSE);  // WM_PAINT 메시지 발생 
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hList;
	static HWND hCombo;

	char name[50];
	char num[50];
	char birth[5];
	static int radio = 0;
	static int selection = 0;
	char info[100]{};

	switch (iMsg) {
	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_LIST);
		hCombo = GetDlgItem(hDlg, IDC_COMBO_BIRTH);
		for (int i = 1930; i <= 2018; ++i)
		{
			sprintf_s(birth, "%d", i);
			SendMessage(hCombo, CB_ADDSTRING , 0, (LPARAM)birth);
		}
		//return true;
		break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_RADIO_FEMALE:
			radio = 0;
			break;

		case IDC_RADIO_MALE:
			radio = 1;
			break;
		case IDC_LIST:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = SendMessage(hList, LB_GETCURSEL, 0, 0);
			break;

		case IDC_BUTTON_EDIT:
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 50);
			GetDlgItemText(hDlg, IDC_EDIT_NUM, num, 50);
			GetDlgItemText(hDlg, IDC_COMBO_BIRTH, birth, 5);
			strcat_s(info, "이름: ");
			strcat_s(info, name);
			strcat_s(info, ", ");
			strcat_s(info, "전화번호: ");
			strcat_s(info, num);
			strcat_s(info, ", ");
			strcat_s(info, "성별: ");
			if (radio == 0)
				strcat_s(info, "여자");
			else if (radio == 1)
				strcat_s(info, "남자");
			strcat_s(info, ", ");
			strcat_s(info, "출생년도: ");
			strcat_s(info, birth);

			if (strcmp(name, ""))
				SendMessage(hList, LB_INSERTSTRING, selection, (LPARAM)(info));
			break;

		case IDC_BUTTON_CLEAR:
			SetDlgItemText(hDlg, IDC_EDIT_NAME, "");
			SetDlgItemText(hDlg, IDC_EDIT_NUM, "");
			break;
		
		case IDC_BUTTON_DELETE:
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			break;
		
		case IDC_BUTTON_JOIN:
			GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 50);
			GetDlgItemText(hDlg, IDC_EDIT_NUM, num, 50);
			GetDlgItemText(hDlg, IDC_COMBO_BIRTH, birth, 5);
			strcat_s(info, "이름: ");
			strcat_s(info, name);
			strcat_s(info, ", ");
			strcat_s(info, "전화번호: ");
			strcat_s(info, num);
			strcat_s(info, ", ");
			strcat_s(info, "성별: ");
			if(radio==0)
				strcat_s(info, "여자");
			else if(radio==1)
				strcat_s(info, "남자");
			strcat_s(info, ", ");
			strcat_s(info, "출생년도: ");
			strcat_s(info, birth);

			if (strcmp(name, ""))
				SendMessage(hList, LB_INSERTSTRING, 0, (LPARAM)(info));
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			break;
		}
	}
		break;

	}
	return 0;
}