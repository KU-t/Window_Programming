// 객체 1개씩 따로 때리기
#include <windows.h>
#include <tchar.h>
#include <atlimage.h>
#include <vector>
#include <ctime>

using namespace std;

#define WD_WIDTH 800
#define WD_HEIGHT 600


int background_wsize = 0;
int startpoint = 0;

enum MENU { e_start, e_stage, e_ingame };
enum ATTACK_COUNT { A_0, A_1, A_2, A_3 };

CImage start, startbutton, stage, controlUI, mouse[3], bear[3], mt[8], bt[8], timer[12], back, food_cnt_png[11], boss[2], pause[5], zombie1[3], zombie2[3];
HDC hdc, savedc;

class ZOMBIE_1
{
public:
	int x, y;
	float hp;   //방어력(체력)
	float maxhp;   //max방어력(체력)
	float attack;   //공격력
	bool tf;      //충돌 체크
	int cnt_an;
	ZOMBIE_1(int x = startpoint + 1500, int y = rand() % 15 + 230, float hp = 10, float ma = 10, float at = 2, bool tf = FALSE, int cnt_an = 0) :x(x), y(y), hp(hp), maxhp(ma), attack(at), tf(tf), cnt_an(cnt_an) {}

	void zombie1_Animation(ZOMBIE_1 m, HDC savedc)
	{
		zombie1[cnt_an].TransparentBlt(savedc, m.x, m.y, 70, 70, RGB(145, 0, 200));
	}
};

class ZOMBIE_2
{
public:
	int x, y;
	float hp;   //방어력(체력)
	float maxhp;   //max방어력(체력)
	float attack;   //공격력
	bool tf;      //충돌 체크
	int cnt_an;
	ZOMBIE_2(int x = startpoint + 1500, int y = rand() % 15 + 230, float hp = 15, float ma = 15, float at = 3, bool tf = FALSE, int cnt_an = 0) :x(x), y(y), hp(hp), maxhp(ma), attack(at), tf(tf), cnt_an(cnt_an) {}

	void zombie2_Animation(ZOMBIE_2 m, HDC savedc)
	{
		zombie2[cnt_an].TransparentBlt(savedc, m.x, m.y, 70, 70, RGB(145, 0, 200));
	}
};

class MOUSE
{
public:
	int x, y;
	float hp;   //방어력(체력)
	float maxhp;   //max방어력(체력)
	float attack;   //공격력
	bool tf;      //충돌 체크
	int cnt_an;

	int attack_count;

	MOUSE(int x = startpoint - 70, int y = rand() % 15 + 230, float hp = 9, float ma = 9, float at = 3, bool tf = FALSE, int cnt_an = 0, int attack_count = 0) :x(x), y(y), hp(hp), maxhp(ma), attack(at), tf(tf), cnt_an(cnt_an), attack_count(attack_count) {}

	void mouse_Animation(MOUSE m, HDC savedc)
	{
		mouse[cnt_an].TransparentBlt(savedc, m.x, m.y, 70, 70, RGB(145, 0, 200));
	}

	void t_at(ZOMBIE_1 &en) {
		if (tf)
			if (en.tf && en.hp > 0) {
				attack_count++;
				if (attack_count > 3)   attack_count = 0;

				if (attack_count == 0)   en.hp -= attack;
			}
	}
};


class BEAR
{
public:
	int x, y;
	float hp;   //방어력(체력)
	float maxhp;   //max방어력(체력)
	float attack;   //공격력
	bool tf;      //충돌 체크
	int cnt_an;
	BEAR(int x = startpoint - 70, int y = rand() % 15 + 230, float hp = 15, float ma = 15, float at = 4, bool tf = FALSE, int cnt_an = 0) :x(x), y(y), hp(hp), maxhp(ma), attack(at), tf(tf), cnt_an(cnt_an) {}

	void bear_Animation(BEAR b, HDC savedc)
	{
		bear[cnt_an].TransparentBlt(savedc, b.x, b.y, 70, 70, RGB(145, 0, 200));
	}
};



vector<MOUSE> m;
vector<BEAR> b;
vector<ZOMBIE_1> z1;
vector<ZOMBIE_2> z2;


void sec_Animation(HDC savedc);
void sec2_Animation(HDC savedc);
void min_Animation(HDC savedc);
void min2_Animation(HDC savedc);
void food_cnt1_Animation(HDC savedc);
void food_cnt2_Animation(HDC savedc);
void skill_cnt1_Animation(HDC savedc);
void skill_cnt2_Animation(HDC savedc);

void check_mz1(MOUSE&, ZOMBIE_1&);
void check_bz1(BEAR&, ZOMBIE_1&);
void check_mz2(MOUSE&, ZOMBIE_2&);
void check_bz2(BEAR&, ZOMBIE_2&);


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

	hWnd = CreateWindow(lpszClass, "window API", WS_OVERLAPPEDWINDOW, 0, 0, WD_WIDTH, WD_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;

}

int select_screen;

int mct, bct;

int s_cnt;
int s2_cnt;
int m_cnt;
int m2_cnt;

int food_cnt;
int food_cnt1;
int food_cnt2;

int skill_cnt;
int skill_cnt1;
int skill_cnt2;

bool mcttf, bcttf;
bool START;

bool dir_button;
bool pause_button;
bool restart_button;
bool esc_button;



LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned)time(NULL));

	PAINTSTRUCT ps;
	static HBITMAP hbitmap;

	HBRUSH hBrush, oldBrush;
	switch (uMsg) {
	case WM_CREATE:

		skill_cnt = 0;
		skill_cnt1 = 0;
		skill_cnt2 = 0;

		pause_button = FALSE;
		restart_button = FALSE;
		esc_button = FALSE;

		select_screen = 0;
		START = FALSE;

		food_cnt = 0;
		food_cnt1 = 0;
		food_cnt2 = 0;

		dir_button = FALSE;

		s_cnt = 0;
		s2_cnt = 0;
		m2_cnt = 0;
		static int mx, my;

		start.Load(TEXT("paladogstart.jpg"));
		startbutton.Load(TEXT("playbutton.bmp"));

		pause[1].Load(TEXT("pause.png"));
		pause[0].Load(TEXT("pausedown.png"));
		pause[2].Load(TEXT("pausemenu.png"));
		pause[3].Load(TEXT("pausemenu_restart.png"));
		pause[4].Load(TEXT("pausemenu_esc.png"));

		stage.Load(TEXT("stage.png"));

		controlUI.Load(TEXT("controlUI.png"));
		back.Load(TEXT("background.png"));

		boss[0].Load(TEXT("boss.png"));
		boss[1].Load(TEXT("boss1.png"));

		mouse[0].Load(TEXT("mouse_1.png"));
		mouse[1].Load(TEXT("mouse_2.png"));
		mouse[2].Load(TEXT("mouse_3.png"));

		mt[0].Load(TEXT("mt1.png"));
		mt[1].Load(TEXT("mt2.png"));
		mt[2].Load(TEXT("mt3.png"));
		mt[3].Load(TEXT("mt4.png"));
		mt[4].Load(TEXT("mt5.png"));
		mt[5].Load(TEXT("mt6.png"));
		mt[6].Load(TEXT("mt7.png"));
		mt[7].Load(TEXT("mt8.png"));


		bear[0].Load(TEXT("bear_1.png"));
		bear[1].Load(TEXT("bear_2.png"));
		bear[2].Load(TEXT("bear_3.png"));

		bt[0].Load(TEXT("bt1.png"));
		bt[1].Load(TEXT("bt2.png"));
		bt[2].Load(TEXT("bt3.png"));
		bt[3].Load(TEXT("bt4.png"));
		bt[4].Load(TEXT("bt5.png"));
		bt[5].Load(TEXT("bt6.png"));
		bt[6].Load(TEXT("bt7.png"));
		bt[7].Load(TEXT("bt8.png"));

		timer[0].Load(TEXT("time_0.png"));
		timer[1].Load(TEXT("time_1.png"));
		timer[2].Load(TEXT("time_2.png"));
		timer[3].Load(TEXT("time_3.png"));
		timer[4].Load(TEXT("time_4.png"));
		timer[5].Load(TEXT("time_5.png"));
		timer[6].Load(TEXT("time_6.png"));
		timer[7].Load(TEXT("time_7.png"));
		timer[8].Load(TEXT("time_8.png"));
		timer[9].Load(TEXT("time_9.png"));
		timer[10].Load(TEXT("time_10.png"));      // : 모양
		timer[11].Load(TEXT("time_11.png"));      // / 모양

		food_cnt_png[0].Load(TEXT("food_0.png"));
		food_cnt_png[1].Load(TEXT("food_1.png"));
		food_cnt_png[2].Load(TEXT("food_2.png"));
		food_cnt_png[3].Load(TEXT("food_3.png"));
		food_cnt_png[4].Load(TEXT("food_4.png"));
		food_cnt_png[5].Load(TEXT("food_5.png"));
		food_cnt_png[6].Load(TEXT("food_6.png"));
		food_cnt_png[7].Load(TEXT("food_7.png"));
		food_cnt_png[8].Load(TEXT("food_8.png"));
		food_cnt_png[9].Load(TEXT("food_9.png"));
		food_cnt_png[10].Load(TEXT("food_10.png"));      // / 모양

		zombie1[0].Load(TEXT("z1_1.png"));
		zombie1[1].Load(TEXT("z1_2.png"));
		zombie1[2].Load(TEXT("z1_3.png"));


		zombie2[0].Load(TEXT("z2_1.png"));
		zombie2[1].Load(TEXT("z2_2.png"));
		zombie2[2].Load(TEXT("z2_3.png"));

		mct = 0;
		bct = 0;

		mcttf = FALSE;
		bcttf = FALSE;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		savedc = CreateCompatibleDC(hdc);

		hbitmap = CreateCompatibleBitmap(hdc, WD_WIDTH, WD_HEIGHT);

		SelectObject(savedc, hbitmap);



		switch (select_screen)
		{
		case e_start:
			start.StretchBlt(savedc, 0, 0, 785, 562, 0, 0, 785, 562, SRCCOPY);   //시작화면 출력
			if (START)
				startbutton.TransparentBlt(savedc, 15, 383, 230, 170, RGB(6, 33, 54));   //시작버튼 출력
			break;

		case e_stage:
			stage.StretchBlt(savedc, 0, 0, 785, 562, 0, 0, 785, 562, SRCCOPY);   //시작화면 출력
			break;

		case e_ingame:
			back.StretchBlt(savedc, 0, 0, 800, 400, background_wsize, 0, 800, 400, SRCCOPY);

			boss[0].TransparentBlt(savedc, startpoint + 1250, 0, 315, 400, RGB(255, 0, 255));

			for (auto p = m.begin(); p < m.end(); p++)   // 쥐새끼 체력 및 애니메이션 구현
			{
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				Rectangle(savedc, p->x + 22, p->y, p->x + 52, p->y + 5);
				DeleteObject(hBrush);

				p->mouse_Animation(*p, savedc);

				hBrush = CreateSolidBrush(RGB(0, 255, 0));
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				if (p->hp > 0)
					Rectangle(savedc, p->x + 22, p->y, p->x + 22 + p->hp * (30 / p->maxhp), p->y + 5);
				DeleteObject(hBrush);

			}
			for (auto p = b.begin(); p < b.end(); p++)                // 곰탱이 체력 및 애니메이션 구현
			{
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				Rectangle(savedc, p->x + 15, p->y - 10, p->x + 45, p->y - 5);
				DeleteObject(hBrush);

				p->bear_Animation(*p, savedc);

				hBrush = CreateSolidBrush(RGB(0, 255, 0));
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				if (p->hp > 0)
					Rectangle(savedc, p->x + 15, p->y - 10, p->x + 15 + p->hp * (30 / p->maxhp), p->y - 5);
				DeleteObject(hBrush);
			}

			for (auto p = z1.begin(); p < z1.end(); p++)                // 적1 체력 및 애니메이션 구현
			{
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				Rectangle(savedc, p->x + 15, p->y - 10, p->x + 45, p->y - 5);
				DeleteObject(hBrush);

				p->zombie1_Animation(*p, savedc);

				hBrush = CreateSolidBrush(RGB(255, 0, 0));
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				if (p->hp > 0)
					Rectangle(savedc, p->x + 15, p->y - 10, p->x + 15 + p->hp * (30 / p->maxhp), p->y - 5);
				DeleteObject(hBrush);
			}

			for (auto p = z2.begin(); p < z2.end(); p++)                // 적2 체력 및 애니메이션 구현
			{
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				Rectangle(savedc, p->x + 15, p->y - 10, p->x + 45, p->y - 5);
				DeleteObject(hBrush);

				p->zombie2_Animation(*p, savedc);

				hBrush = CreateSolidBrush(RGB(255, 0, 0));
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				if (p->hp > 0)
					Rectangle(savedc, p->x + 15, p->y - 10, p->x + 15 + p->hp * (30 / p->maxhp), p->y - 5);
				DeleteObject(hBrush);
			}

			boss[1].TransparentBlt(savedc, startpoint + 1250, 0, 315, 400, RGB(255, 0, 255));

			hBrush = CreateSolidBrush(RGB(0, 0, 0));               // 먹이 검정색 배경 그리기
			oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
			Rectangle(savedc, 55, 390, 285, 410);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(0, 0, 0));;               // 스킬 검정색 배경 그리기
			oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
			Rectangle(savedc, 550, 390, 730, 410);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(229, 188, 0));            //먹이 노란색 카운트 그리기
			oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
			Rectangle(savedc, 55, 390, 75 + food_cnt * 3, 410);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(0, 198, 190));            //스킬 하늘색 카운트 그리기
			oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
			Rectangle(savedc, 730, 390, 730 - skill_cnt * 2, 410);
			DeleteObject(hBrush);

			min2_Animation(savedc);                                    // 플레이시간 출력
			timer[10].TransparentBlt(savedc, 400, 0, 15, 20, RGB(0, 0, 0));
			sec_Animation(savedc);
			sec2_Animation(savedc);
			min_Animation(savedc);

			controlUI.TransparentBlt(savedc, 0, 360, 785, 200, RGB(145, 0, 200));   //ui출력

			food_cnt1_Animation(savedc);                                 // 먹이 카운트 출력 애니메이션
			food_cnt2_Animation(savedc);
			food_cnt_png[10].TransparentBlt(savedc, 284, 392, 12, 15, RGB(255, 255, 255));
			food_cnt_png[6].TransparentBlt(savedc, 296, 392, 12, 15, RGB(255, 255, 255));
			food_cnt_png[0].TransparentBlt(savedc, 309, 392, 12, 15, RGB(255, 255, 255));

			skill_cnt1_Animation(savedc);                                 // 먹이 카운트 출력 애니메이션
			skill_cnt2_Animation(savedc);
			food_cnt_png[10].TransparentBlt(savedc, 510, 392, 12, 15, RGB(255, 255, 255));
			food_cnt_png[9].TransparentBlt(savedc, 522, 392, 12, 15, RGB(255, 255, 255));
			food_cnt_png[0].TransparentBlt(savedc, 534, 392, 12, 15, RGB(255, 255, 255));

			if (mcttf == TRUE)mt[mct].StretchBlt(savedc, 445, 416, 81, 64, SRCCOPY);// 쿨타임 애니메이션 구현
			if (bcttf == TRUE)bt[bct].StretchBlt(savedc, 612, 416, 81, 64, SRCCOPY);

			if (pause_button == FALSE)pause[1].BitBlt(savedc, 721, 0);         // pause 버튼 + pause 메뉴 draw
			else
			{
				pause[0].BitBlt(savedc, 721, 0);
				pause[2].TransparentBlt(savedc, 200, 75, 349, 309, RGB(100, 100, 100));

			}
			if (restart_button == TRUE)pause[3].TransparentBlt(savedc, 200, 75, 349, 309, RGB(100, 100, 100));
			if (esc_button == TRUE)pause[4].TransparentBlt(savedc, 200, 75, 349, 309, RGB(100, 100, 100));
			break;
		}

		BitBlt(hdc, 0, 0, WD_WIDTH, WD_HEIGHT, savedc, 0, 0, SRCCOPY);

		DeleteDC(savedc);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			for (auto q = z1.begin(); q < z1.end(); q++) {

				for (auto p = m.begin(); p < m.end(); p++)  check_mz1(*p, *q);

				for (auto p = b.begin(); p < b.end(); p++)  check_bz1(*p, *q);

			}

			for (auto q = z1.begin(); q < z1.end(); ) {
				if (q->hp <= 0) {
					q = z1.erase(q);
					for (auto p = m.begin(); p < m.end(); p++)    p->tf = FALSE;
				}
				else q++;
			}

			for (auto q = z2.begin(); q < z2.end(); q++) {
				for (auto p = m.begin(); p < m.end(); p++)  check_mz2(*p, *q);

				for (auto p = b.begin(); p < b.end(); p++)  check_bz2(*p, *q);
			}

			for (auto p = m.begin(); p < m.end(); p++) {   // 캐릭터 이동
				if (!(p->tf))   p->x += 5;
				p->cnt_an++;
				if (p->cnt_an == 3)   p->cnt_an = 0;
			}

			for (auto p = b.begin(); p < b.end(); p++) {
				if (!p->tf)   p->x += 5;
				p->cnt_an++;
				if (p->cnt_an == 3)   p->cnt_an = 0;
			}

			for (auto p = z1.begin(); p < z1.end(); p++) {
				if (!(p->tf))   p->x -= 5;
				p->cnt_an++;
				if (p->cnt_an == 3)   p->cnt_an = 0;
			}

			for (auto p = z2.begin(); p < z2.end(); p++) {
				if (!p->tf)   p->x -= 5;
				p->cnt_an++;
				if (p->cnt_an == 3)   p->cnt_an = 0;
			}
			break;

		case 2:
			if (food_cnt < 60)food_cnt += 1;         // 먹이 카운트 조절
			if (food_cnt2 != 6)food_cnt1++;
			if (food_cnt1 > 9)
			{
				if (food_cnt2 < 6)food_cnt2++;
				food_cnt1 = 0;
			}
			break;

		case 3:                  // 쥐새끼 쿨타임 조절
			mct++;
			if (mct == 8) {
				mct = 0;
				KillTimer(hWnd, 3);
				mcttf = FALSE;
			}
			break;

		case 4:                  // 곰탱이 쿨타임 조절
			bct++;
			if (bct == 8) {
				bct = 0;
				KillTimer(hWnd, 4);
				bcttf = FALSE;
			}
			break;

		case 5:
			s_cnt++;                           // 플레이시간 조절
			if (s_cnt > 9) { s2_cnt++; s_cnt = 0; }
			if (s2_cnt > 5) { m_cnt++; s2_cnt = 0; }
			if (m_cnt > 9) { m2_cnt++; m_cnt = 0; }
			break;
		case 6:
			skill_cnt;
			if (skill_cnt < 90)skill_cnt += 1;         // 스킬 카운트 조절
			if (skill_cnt2 != 9)skill_cnt1++;
			if (skill_cnt1 > 9)
			{
				if (skill_cnt2 < 9)skill_cnt2++;
				skill_cnt1 = 0;
			}
			break;
		case 7:
			z1.emplace_back();
			break;

		case 8:
			z2.emplace_back();
			break;

		case 9:
			for (auto p = m.begin(); p < m.end(); p++) {
				for (auto q = z1.begin(); q < z1.end(); q++) {
					p->t_at(*q);
					break;
				}
			}
			break;
		}


		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		switch (select_screen) {
		case e_start:

			break;

		case e_stage:

			break;

		case e_ingame:
			if (mx > 445 && mx < 526 && my < 480 && my>416 && food_cnt >= 10 && mcttf == FALSE)      // 쥐새끼 마우스 클릭 생성
			{
				m.emplace_back();
				food_cnt -= 10;
				food_cnt2 -= 1;
				SetTimer(hWnd, 3, 200, NULL);
				mcttf = TRUE;

			}
			else if (mx > 612 && mx < 693 && my < 480 && my>416 && food_cnt >= 30 && bcttf == FALSE)   // 곰탱이 마우스 클릭 생성
			{
				b.emplace_back();
				food_cnt -= 30;
				food_cnt2 -= 3;
				SetTimer(hWnd, 4, 400, NULL);
				bcttf = TRUE;
			}

			if (mx > 70 && mx < 175 && my < 530 && my>425) {

				if (background_wsize > 0)
				{
					background_wsize -= 5;
					startpoint += 5;
					for (auto p = m.begin(); p < m.end(); p++)p->x += 5;
					for (auto p = b.begin(); p < b.end(); p++)p->x += 5;
					dir_button = TRUE;
				}
			}

			if (mx > 225 && mx < 330 && my < 530 && my>425) {
				if (background_wsize < 765)
				{
					background_wsize += 5;
					startpoint -= 5;
					for (auto p = m.begin(); p < m.end(); p++)p->x -= 5;
					for (auto p = b.begin(); p < b.end(); p++)p->x -= 5;
					dir_button = TRUE;
				}
			}
			if (mx > 721 && mx < 800 && my>0 && my < 63 && pause_button == FALSE)      // pause 기능 구현 + 종료 + 재시작 기능 구현
			{
				pause_button = TRUE;

			}
			else if (mx > 250 && mx < 500 && my>180 && my < 255 && pause_button == TRUE) { pause_button = FALSE; }
			else if (mx > 250 && mx < 500 && my>275 && my < 350 && pause_button == TRUE)
			{
				PostQuitMessage(0);
				pause[4].TransparentBlt(savedc, 200, 75, 349, 309, RGB(100, 100, 100));
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		switch (select_screen) {
		case e_start:
			if (15 <= mx && mx <= 245 && 383 <= my && my <= 553) START = TRUE;
			else START = FALSE;
			break;
		case e_ingame:
			if (mx > 250 && mx < 500 && my>180 && my < 255 && pause_button == TRUE) { restart_button = TRUE; }
			else restart_button = FALSE;
			if (mx > 250 && mx < 500 && my>275 && my < 350 && pause_button == TRUE) { esc_button = TRUE; }
			else esc_button = FALSE;
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;


	case WM_LBUTTONUP:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		switch (select_screen) {
		case e_start:
			if (15 <= mx && mx <= 245 && 383 <= my && my <= 553 && START) select_screen = e_stage;
			else START = FALSE;
			break;

		case e_stage:
			select_screen = e_ingame;
			SetTimer(hWnd, 1, 100, NULL);   // 아군,적군 이동
			SetTimer(hWnd, 2, 200, NULL);   // 먹이 생성 0.2초에 1씩증가 12초에 60 풀로참
			SetTimer(hWnd, 5, 1000, NULL);   // 타이머
			SetTimer(hWnd, 6, 500, NULL);   // 스킬 생성 0.5초에 1씩증가 45초에 90 풀로참
			SetTimer(hWnd, 7, 8000, NULL);   //적군1 생성
											 //SetTimer(hWnd, 8, 20000, NULL);   //적군2 생성
			SetTimer(hWnd, 9, 250, NULL);   // 쥐-적쥐 공격시간
			break;

		case e_ingame:
			if (dir_button == TRUE) { dir_button = FALSE; }                  // 마우스로 방향 버튼

			if (mx > 721 && mx < 800 && my>0 && my < 63 && pause_button == TRUE)   // pause 기능구현
			{
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				KillTimer(hWnd, 5);
				KillTimer(hWnd, 6);
				KillTimer(hWnd, 7);
				//   KillTimer(hWnd, 8);
			}
			else if (mx > 250 && mx < 500 && my>180 && my < 255 && pause_button == FALSE)
			{
				SetTimer(hWnd, 1, 100, NULL);
				SetTimer(hWnd, 2, 200, NULL);
				SetTimer(hWnd, 3, 200, NULL);
				SetTimer(hWnd, 4, 400, NULL);
				SetTimer(hWnd, 5, 1000, NULL);
				SetTimer(hWnd, 6, 500, NULL);
				SetTimer(hWnd, 7, 8000, NULL);   //적군1 생성
				SetTimer(hWnd, 8, 20000, NULL);   //적군2 생성
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
			if (food_cnt >= 10 && mcttf == FALSE)      // 쥐새끼 키다운 생성
			{
				m.emplace_back();
				food_cnt -= 10;
				food_cnt2 -= 1;
				SetTimer(hWnd, 3, 200, NULL);
				mcttf = TRUE;

			}
			break;

		case '2':                              // 곰탱이 키다운 생성
			if (food_cnt >= 30 && bcttf == FALSE)
			{
				b.emplace_back();
				food_cnt -= 30;
				food_cnt2 -= 3;
				SetTimer(hWnd, 4, 400, NULL);
				bcttf = TRUE;
			}
			break;

		case VK_RIGHT:      case 'd':   case 'D':
			if (background_wsize < 765)
			{
				background_wsize += 5;
				startpoint -= 5;
				for (auto p = m.begin(); p < m.end(); p++)p->x -= 5;
				for (auto p = b.begin(); p < b.end(); p++)p->x -= 5;
				for (auto p = z1.begin(); p < z1.end(); p++)p->x -= 5;
				for (auto p = z2.begin(); p < z2.end(); p++)p->x -= 5;
			}                                                        //맵 우측 이동
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case VK_LEFT:   case 'a':   case 'A':
			if (background_wsize > 0)
			{
				background_wsize -= 5;
				startpoint += 5;
				for (auto p = m.begin(); p < m.end(); p++)p->x += 5;
				for (auto p = b.begin(); p < b.end(); p++)p->x += 5;
				for (auto p = z1.begin(); p < z1.end(); p++)p->x += 5;
				for (auto p = z2.begin(); p < z2.end(); p++)p->x += 5;
			}                                                        // 맵 좌측 이동
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// * 애니메이션 함수

// 아군 애니메이션



// 적군 애니메이션



// 플레이타임 카운트
void sec_Animation(HDC savedc)
{
	timer[s_cnt].TransparentBlt(savedc, 440, 0, 15, 20, RGB(0, 0, 0));
}
void sec2_Animation(HDC savedc)
{
	timer[s2_cnt].TransparentBlt(savedc, 420, 0, 15, 20, RGB(0, 0, 0));
}
void min_Animation(HDC savedc)
{
	timer[m_cnt].TransparentBlt(savedc, 380, 0, 15, 20, RGB(0, 0, 0));
}
void min2_Animation(HDC savedc)
{
	timer[m2_cnt].TransparentBlt(savedc, 360, 0, 15, 20, RGB(0, 0, 0));
}
// 먹이 카운트
void food_cnt1_Animation(HDC savedc)
{
	food_cnt_png[food_cnt1].TransparentBlt(savedc, 272, 392, 12, 15, RGB(255, 255, 255));
}
void food_cnt2_Animation(HDC savedc)
{
	food_cnt_png[food_cnt2].TransparentBlt(savedc, 260, 392, 12, 15, RGB(255, 255, 255));
}

//스킬 카운트
void skill_cnt1_Animation(HDC savedc)
{
	food_cnt_png[skill_cnt1].TransparentBlt(savedc, 498, 392, 12, 15, RGB(255, 255, 255));
}
void skill_cnt2_Animation(HDC savedc)
{
	food_cnt_png[skill_cnt2].TransparentBlt(savedc, 486, 392, 12, 15, RGB(255, 255, 255));
}

void check_mz1(MOUSE &my, ZOMBIE_1 &en) {

	if (en.x - my.x <= 65) {
		my.tf = TRUE;
		en.tf = TRUE;
	}
}


void check_bz1(BEAR &my, ZOMBIE_1 &en) {

	if (en.x - my.x <= 65) {
		my.tf = TRUE;
		en.tf = TRUE;
	}
}

void check_mz2(MOUSE &my, ZOMBIE_2 &en) {

	if (en.x - my.x <= 65) {
		my.tf = TRUE;
		en.tf = TRUE;
	}
}


void check_bz2(BEAR &my, ZOMBIE_2 &en) {

	if (en.x - my.x <= 65) {
		my.tf = TRUE;
		en.tf = TRUE;
	}
}