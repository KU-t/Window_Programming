//스킬추가
#include <windows.h>
#include <tchar.h>
#include <atlimage.h>
#include <vector>
#include <ctime>

using namespace std;

#define GRAY RGB(100, 100, 100)
#define WHITE RGB(255, 255, 255)
#define BLACK RGB(0, 0, 0)
#define PURPLE RGB(145, 0, 200)

#define WD_WIDTH 800
#define WD_HEIGHT 600

int background_wsize = 0;
int startpoint = 0;

enum MENU { e_start, e_stage, e_ingame, e_gameover, e_victory };
enum STAGE { nul, stage1, stage2 };
enum ATTACK_COUNT { A_0, A_1, A_2, A_3 };
enum My { m_mouse, m_bear };
enum Enemy { e_mouse, e_bear };

CImage start, startbutton, stage[4], controlUI, mouse[3], bear[3], mt[8], bt[8], timer[12], back[2], food_cnt_png[11], boss[2], pause[5], zombie1[3], zombie2[3];
CImage paladog[14], skill, mouse_attack[4], gameover[2], skill_fire[3], bear_attack[4], victory[2], z1_attack[4], z2_attack[4], level, Next[2];
HDC hdc, savedc;

class PALADOG
{
public:
	int x, y;
	float hp;   //방어력(체력)
	float maxhp;   //max방어력(체력)
	bool tf;      //충돌 체크
	int cnt_an;
	int level;

	PALADOG(int x = startpoint + 70, int y = 200, float hp = 50, float ma = 50, bool tf = FALSE, int cnt_an = 0) :x(x), y(y), hp(hp), maxhp(ma), tf(tf), cnt_an(cnt_an) {}
	void MY_Animation(PALADOG m, HDC savedc)
	{
		paladog[cnt_an].TransparentBlt(savedc, m.x, m.y, 120, 110, PURPLE);
	}
};

PALADOG p;

class SKILL_1
{
public:
	int x, y;
	float attack;
	int life;

	SKILL_1(int x = p.x + 100, int y = p.y + 20, int attack = 6, int life = 3) : x(x), y(y), attack(attack), life(life) {}
};

class BOSS
{
public:
	int x, y;
	float hp;   //방어력(체력)
	float maxhp;   //max방어력(체력)
	bool tf;      //충돌 체크
	BOSS(int x = startpoint + 1250, int y = 0, float hp = 100, float ma = 100, bool tf = FALSE) :x(x), y(y), hp(hp), maxhp(ma), tf(tf) {}

};

class ENEMY
{
public:
	int shape;
	int x, y;
	float hp;   //방어력(체력)
	float maxhp;   //max방어력(체력)
	float attack;   //공격력
	bool tf;      //충돌 체크
	int cnt_an;
	int cnt_at;
	int attack_count;
	bool skill_check;
	bool paladog_tf;

	void enemy_Animation(ENEMY m, HDC savedc)
	{
		if (m.shape == e_mouse)
			zombie1[cnt_an].TransparentBlt(savedc, m.x, m.y, 70, 70, PURPLE);
		else if (m.shape == e_bear)
			zombie2[cnt_an].TransparentBlt(savedc, m.x, m.y, 70, 70, PURPLE);
	}
	void enemy_Attack_Animation(ENEMY m, HDC savedc)
	{
		if (m.shape == e_mouse)
			z1_attack[cnt_at].TransparentBlt(savedc, m.x, m.y, 70, 70, PURPLE);
		else if (m.shape == e_bear)
			z2_attack[cnt_at].TransparentBlt(savedc, m.x, m.y, 70, 70, PURPLE);
	}

	void m_make() {
		shape = e_mouse;
		x = startpoint + 1500;
		y = rand() % 25 + 230;
		hp = 16;
		maxhp = 16;
		attack = 2;
		cnt_an = 0;
		cnt_at = 0;
		tf = FALSE;
		attack_count = 0;
		skill_check = FALSE;
		paladog_tf = FALSE;
	}

	void b_make() {
		shape = e_bear;
		x = startpoint + 1500;
		y = rand() % 25 + 230;
		hp = 20;
		maxhp = 20;
		attack = 3;
		cnt_an = 0;
		cnt_at = 0;
		tf = FALSE;
		attack_count = 0;
		paladog_tf = FALSE;
		skill_check = FALSE;
	}
};

class MY
{
public:
	int shape;
	int x, y;
	float hp;   //방어력(체력)
	float maxhp;   //max방어력(체력)
	float attack;   //공격력
	bool tf;      //충돌 체크
	int cnt_an;
	int attack_count;
	int cnt_at;

	void MY_Animation(MY m, HDC savedc)
	{
		if (m.shape == m_mouse)
			mouse[cnt_an].TransparentBlt(savedc, m.x, m.y, 70, 70, PURPLE);
		else if (m.shape == m_bear)
			bear[cnt_an].TransparentBlt(savedc, m.x, m.y, 70, 70, PURPLE);
	}
	void MY_Attack_Animation(MY m, HDC savedc)
	{
		if (m.shape == m_mouse)
			mouse_attack[cnt_at].TransparentBlt(savedc, m.x, m.y, 70, 70, PURPLE);
		else if (m.shape == m_bear)
			bear_attack[cnt_at].TransparentBlt(savedc, m.x, m.y, 70, 70, PURPLE);
	}

	void m_make() {
		shape = m_mouse;
		x = startpoint - 70;
		y = rand() % 25 + 230;
		hp = 9;
		maxhp = 9;
		attack = 3;
		cnt_an = 0;
		cnt_at = 0;
		tf = FALSE;
		attack_count = 0;
	}

	void b_make() {
		shape = m_bear;
		x = startpoint - 70;
		y = rand() % 25 + 230;
		hp = 15;
		maxhp = 15;
		attack = 4;
		cnt_an = 0;
		cnt_at = 0;
		tf = FALSE;
		attack_count = 0;
	}
};

vector<MY> m;

vector<ENEMY> z;

vector<SKILL_1> s_1;

BOSS b;

void sec_Animation(HDC savedc);
void sec2_Animation(HDC savedc);
void min_Animation(HDC savedc);
void min2_Animation(HDC savedc);
void food_cnt1_Animation(HDC savedc);
void food_cnt2_Animation(HDC savedc);
void skill_cnt1_Animation(HDC savedc);
void skill_cnt2_Animation(HDC savedc);

void check_mz1(MY&, ENEMY&);
void check_bz1(MY&, ENEMY&);
void check_mboss(MY &my, BOSS &en);
void check_pala_z(PALADOG &my, ENEMY &en);

void t_at(MY&, ENEMY&);
void t_at1(MY &my, ENEMY &en);
void t_at2(MY &my, BOSS &en);
void t_at3(PALADOG &my, ENEMY &en);

void reset();

HINSTANCE g_hInst;

LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow)
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

int screen;

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
bool fire_attack;

bool skill_buf;
bool FS_count; // UI 카운트
int z1_buf, z2_buf;
int s1_count;
int fire_cnt;
bool left_sw;
int Stage;

bool stage_buf;

bool stage_1, stage_2, stage_exit;

bool gameover_sw;
bool victory_sw;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FILE *fp;
	static PAINTSTRUCT ps;
	static HBITMAP hbitmap;

	static HBRUSH hBrush, oldBrush;

	static int mx, my;

	switch (uMsg) {
	case WM_CREATE:


		fp = fopen("data.txt", "r");
		fscanf(fp, "%d", &p.level);
		fclose(fp);

		skill_cnt = 0;
		skill_cnt1 = 0;
		skill_cnt2 = 0;
		fire_cnt = 0;
		skill_buf = TRUE;
		fire_attack = FALSE;

		stage_buf = FALSE;

		gameover_sw = FALSE;
		victory_sw = FALSE;

		z1_buf = 0;
		z2_buf = 0;

		left_sw = FALSE;

		s1_count = 0;
		FS_count = true;

		screen = 0;
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

		start.Load(TEXT("image/paladogstart.jpg"));
		startbutton.Load(TEXT("image/playbutton.bmp"));

		pause[1].Load(TEXT("image/pause.png"));
		pause[0].Load(TEXT("image/pausedown.png"));
		pause[2].Load(TEXT("image/pausemenu.png"));
		pause[3].Load(TEXT("image/pausemenu_restart.png"));
		pause[4].Load(TEXT("image/pausemenu_esc.png"));

		stage[0].Load(TEXT("image/stage.png"));
		stage[1].Load(TEXT("image/stage1.png"));
		stage[2].Load(TEXT("image/stage2.png"));
		stage[3].Load(TEXT("image/stage_exit.png"));

		controlUI.Load(TEXT("image/controlUI.png"));
		back[0].Load(TEXT("image/background1.png"));
		back[1].Load(TEXT("image/background2.png"));

		boss[0].Load(TEXT("image/boss.png"));
		boss[1].Load(TEXT("image/boss1.png"));

		paladog[0].Load(TEXT("image/paladog.png"));
		paladog[1].Load(TEXT("image/paladog1.png"));
		paladog[2].Load(TEXT("image/paladog2.png"));
		paladog[3].Load(TEXT("image/paladog3.png"));
		paladog[4].Load(TEXT("image/paladog4.png"));
		paladog[5].Load(TEXT("image/paladog5.png"));
		paladog[6].Load(TEXT("image/paladog6.png"));
		paladog[7].Load(TEXT("image/paladog7.png"));
		paladog[8].Load(TEXT("image/paladog8.png"));
		paladog[9].Load(TEXT("image/paladog9.png"));
		paladog[10].Load(TEXT("image/paladog10.png"));
		paladog[11].Load(TEXT("image/paladog11.png"));
		paladog[12].Load(TEXT("image/paladog12.png"));


		skill_fire[0].Load(TEXT("image/skill_fire0.png"));
		skill_fire[1].Load(TEXT("image/skill_fire1.png"));
		skill_fire[2].Load(TEXT("image/skill_fire2.png"));

		skill.Load(TEXT("image/skill1.png"));

		mouse[0].Load(TEXT("image/mouse_1.png"));
		mouse[1].Load(TEXT("image/mouse_2.png"));
		mouse[2].Load(TEXT("image/mouse_3.png"));

		mt[0].Load(TEXT("image/mt1.png"));
		mt[1].Load(TEXT("image/mt2.png"));
		mt[2].Load(TEXT("image/mt3.png"));
		mt[3].Load(TEXT("image/mt4.png"));
		mt[4].Load(TEXT("image/mt5.png"));
		mt[5].Load(TEXT("image/mt6.png"));
		mt[6].Load(TEXT("image/mt7.png"));
		mt[7].Load(TEXT("image/mt8.png"));
		mouse_attack[0].Load(TEXT("image/mouse_at1.png"));
		mouse_attack[1].Load(TEXT("image/mouse_at2.png"));
		mouse_attack[2].Load(TEXT("image/mouse_at3.png"));
		mouse_attack[3].Load(TEXT("image/mouse_at4.png"));

		bear[0].Load(TEXT("image/bear_1.png"));
		bear[1].Load(TEXT("image/bear_2.png"));
		bear[2].Load(TEXT("image/bear_3.png"));
		bear_attack[0].Load(TEXT("image/b_at1.png"));
		bear_attack[1].Load(TEXT("image/b_at2.png"));
		bear_attack[2].Load(TEXT("image/b_at3.png"));
		bear_attack[3].Load(TEXT("image/b_at4.png"));

		bt[0].Load(TEXT("image/bt1.png"));
		bt[1].Load(TEXT("image/bt2.png"));
		bt[2].Load(TEXT("image/bt3.png"));
		bt[3].Load(TEXT("image/bt4.png"));
		bt[4].Load(TEXT("image/bt5.png"));
		bt[5].Load(TEXT("image/bt6.png"));
		bt[6].Load(TEXT("image/bt7.png"));
		bt[7].Load(TEXT("image/bt8.png"));

		timer[0].Load(TEXT("image/time_0.png"));
		timer[1].Load(TEXT("image/time_1.png"));
		timer[2].Load(TEXT("image/time_2.png"));
		timer[3].Load(TEXT("image/time_3.png"));
		timer[4].Load(TEXT("image/time_4.png"));
		timer[5].Load(TEXT("image/time_5.png"));
		timer[6].Load(TEXT("image/time_6.png"));
		timer[7].Load(TEXT("image/time_7.png"));
		timer[8].Load(TEXT("image/time_8.png"));
		timer[9].Load(TEXT("image/time_9.png"));
		timer[10].Load(TEXT("image/time_10.png"));      // : 모양
		timer[11].Load(TEXT("image/time_11.png"));      // / 모양

		food_cnt_png[0].Load(TEXT("image/food_0.png"));
		food_cnt_png[1].Load(TEXT("image/food_1.png"));
		food_cnt_png[2].Load(TEXT("image/food_2.png"));
		food_cnt_png[3].Load(TEXT("image/food_3.png"));
		food_cnt_png[4].Load(TEXT("image/food_4.png"));
		food_cnt_png[5].Load(TEXT("image/food_5.png"));
		food_cnt_png[6].Load(TEXT("image/food_6.png"));
		food_cnt_png[7].Load(TEXT("image/food_7.png"));
		food_cnt_png[8].Load(TEXT("image/food_8.png"));
		food_cnt_png[9].Load(TEXT("image/food_9.png"));
		food_cnt_png[10].Load(TEXT("image/food_10.png"));      // / 모양

		zombie1[0].Load(TEXT("image/z1_1.png"));
		zombie1[1].Load(TEXT("image/z1_2.png"));
		zombie1[2].Load(TEXT("image/z1_3.png"));
		z1_attack[0].Load(TEXT("image/z1_at1.png"));
		z1_attack[1].Load(TEXT("image/z1_at2.png"));
		z1_attack[2].Load(TEXT("image/z1_at3.png"));
		z1_attack[3].Load(TEXT("image/z1_at4.png"));

		zombie2[0].Load(TEXT("image/z2_1.png"));
		zombie2[1].Load(TEXT("image/z2_2.png"));
		zombie2[2].Load(TEXT("image/z2_3.png"));
		z2_attack[0].Load(TEXT("image/z2_at1.png"));
		z2_attack[1].Load(TEXT("image/z2_at2.png"));
		z2_attack[2].Load(TEXT("image/z2_at3.png"));
		z2_attack[3].Load(TEXT("image/z2_at4.png"));

		gameover[0].Load(TEXT("image/mission_failed.png"));
		gameover[1].Load(TEXT("image/mission_failed2.png"));

		victory[0].Load(TEXT("image/stage1_clear.png"));
		victory[1].Load(TEXT("image/stage2_clear.png"));

		level.Load(TEXT("image/level.png"));

		Next[0].Load(TEXT("image/next.png"));
		Next[1].Load(TEXT("image/next_ck.png"));

		mct = 0;
		bct = 0;

		mcttf = FALSE;
		bcttf = FALSE;

		stage_1 = FALSE;
		stage_2 = FALSE;
		stage_exit = FALSE;
		break;

	case WM_PAINT:
		// 3분 30초가 넘어가면 ps가 NULL이 되어서 hdc를 못만들어내
		// 결국 그리기를 못하는건데
		// 메모리는 왜이렇게 쓰는거지??

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

		case e_gameover:

			if (!gameover_sw)
				gameover[0].StretchBlt(savedc, 0, 0, 785, 560, 0, 0, 785, 560, SRCCOPY);
			else if (gameover_sw)
				gameover[1].StretchBlt(savedc, 0, 0, 785, 560, 0, 0, 785, 560, SRCCOPY);
			break;

		case e_victory:
			victory[0].StretchBlt(savedc, 0, 0, 800, 600, 0, 0, 800, 600, SRCCOPY);
			// 플레이시간 출력
			timer[0].TransparentBlt(savedc, 180, 305, 15, 20, BLACK);
			timer[m2_cnt].TransparentBlt(savedc, 280, 305, 15, 20, BLACK);
			timer[m_cnt].TransparentBlt(savedc, 300, 305, 15, 20, BLACK);
			timer[s2_cnt].TransparentBlt(savedc, 380, 305, 15, 20, BLACK);
			timer[s_cnt].TransparentBlt(savedc, 400, 305, 15, 20, BLACK);

			if (!victory_sw)
				Next[0].TransparentBlt(savedc, 520, 450, 240, 100, PURPLE);
			else if (victory_sw)
				Next[1].TransparentBlt(savedc, 520, 450, 240, 100, PURPLE);
			break;

		case e_stage:

			if (stage_1)
				stage[1].StretchBlt(savedc, 0, 0, 785, 562, 0, 0, 785, 562, SRCCOPY);   //시작화면 출력
			else if (stage_2)
				stage[2].StretchBlt(savedc, 0, 0, 785, 562, 0, 0, 785, 562, SRCCOPY);   //시작화면 출력
			else if (stage_exit)
				stage[3].StretchBlt(savedc, 0, 0, 785, 562, 0, 0, 785, 562, SRCCOPY);   //시작화면 출력
			else
				stage[0].StretchBlt(savedc, 0, 0, 785, 562, 0, 0, 785, 562, SRCCOPY);   //시작화면 출력

			break;

		case e_ingame:


			if (Stage == stage1)
				back[0].StretchBlt(savedc, 0, 0, 800, 400, background_wsize, 0, 800, 400, SRCCOPY);
			else if (Stage == stage2)
				back[1].StretchBlt(savedc, 0, 0, 800, 400, background_wsize, 0, 800, 400, SRCCOPY);

			if (b.hp > 0)
			{
				boss[0].TransparentBlt(savedc, b.x, 0, 315, 400, RGB(255, 0, 255));
			}
			else
			{
				b.hp = 0;
			}

			if (p.hp > 0)
			{
				hBrush = CreateSolidBrush(WHITE);
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				Rectangle(savedc, p.x + 15, p.y - 15, p.x + 67, p.y - 10);
				DeleteObject(hBrush);

				p.MY_Animation(p, savedc);

				hBrush = CreateSolidBrush(RGB(0, 100, 255));
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				if (p.hp >= 0)
					Rectangle(savedc, p.x + 15, p.y - 15, p.x + 15 + p.hp * (52 / p.maxhp), p.y - 10);
				DeleteObject(hBrush);
			}
			else
			{
				select_screen = e_gameover;
			}

			for (auto p = m.begin(); p < m.end(); p++)   // 쥐새끼 체력 및 애니메이션 구현
			{
				if (!p->tf)
				{
					hBrush = CreateSolidBrush(WHITE);
					oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
					Rectangle(savedc, p->x + 22, p->y - 5, p->x + 52, p->y);
					DeleteObject(hBrush);

					p->MY_Animation(*p, savedc);

					hBrush = CreateSolidBrush(RGB(0, 255, 0));
					oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
					if (p->hp >= 0)
						Rectangle(savedc, p->x + 22, p->y - 5, p->x + 22 + p->hp * (30 / p->maxhp), p->y);
					DeleteObject(hBrush);
				}
				else
				{
					hBrush = CreateSolidBrush(WHITE);
					oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
					Rectangle(savedc, p->x + 22, p->y - 5, p->x + 52, p->y);
					DeleteObject(hBrush);

					p->MY_Attack_Animation(*p, savedc);

					hBrush = CreateSolidBrush(RGB(0, 255, 0));
					oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
					if (p->hp >= 0)
						Rectangle(savedc, p->x + 22, p->y - 5, p->x + 22 + p->hp * (30 / p->maxhp), p->y);
					DeleteObject(hBrush);
				}

			}

			for (auto p = z.begin(); p < z.end(); p++)                // 적2 체력 및 애니메이션 구현
			{
				if (!p->tf &&   p->paladog_tf == FALSE)
				{
					hBrush = CreateSolidBrush(WHITE);
					oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
					Rectangle(savedc, p->x + 30, p->y - 10, p->x + 60, p->y - 5);
					DeleteObject(hBrush);

					p->enemy_Animation(*p, savedc);

					hBrush = CreateSolidBrush(RGB(255, 0, 0));
					oldBrush = (HBRUSH)SelectObject(savedc, hBrush);

					Rectangle(savedc, p->x + 30, p->y - 10, p->x + 30 + p->hp * (30 / p->maxhp), p->y - 5);
					DeleteObject(hBrush);
				}
				else
				{
					hBrush = CreateSolidBrush(WHITE);
					oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
					Rectangle(savedc, p->x + 22, p->y - 5, p->x + 52, p->y);
					DeleteObject(hBrush);

					p->enemy_Attack_Animation(*p, savedc);

					hBrush = CreateSolidBrush(RGB(255, 0, 0));
					oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
					if (p->hp >= 0)
						Rectangle(savedc, p->x + 22, p->y - 5, p->x + 22 + p->hp * (30 / p->maxhp), p->y);
					DeleteObject(hBrush);
				}
			}

			for (auto p = s_1.begin(); p < s_1.end(); p++)
			{
				skill.TransparentBlt(savedc, p->x, p->y, 70, 60, PURPLE);
			}

			if (fire_attack == TRUE)
			{
				skill_fire[fire_cnt].TransparentBlt(savedc, p.x + 115, p.y, 130, 104, PURPLE);
			}



			if (b.hp > 0)
			{
				boss[1].TransparentBlt(savedc, b.x, 0, 315, 400, RGB(255, 0, 255));
			}
			else
			{
				b.hp <= 0;
				select_screen = e_victory;
				p.level++;
			}

			hBrush = CreateSolidBrush(BLACK);               // 먹이 검정색 배경 그리기
			oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
			Rectangle(savedc, 55, 390, 285, 410);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(BLACK);;               // 스킬 검정색 배경 그리기
			oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
			Rectangle(savedc, 550, 390, 730, 410);
			DeleteObject(hBrush);


			hBrush = CreateSolidBrush(RGB(229, 188, 0));            //먹이 노란색 카운트 그리기
			oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
			Rectangle(savedc, 55, 390, 55 + food_cnt * 3.3, 410);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(0, 198, 190));            //스킬 하늘색 카운트 그리기
			oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
			Rectangle(savedc, 730, 390, 730 - skill_cnt * 2, 410);
			DeleteObject(hBrush);

			min2_Animation(savedc);                                    // 플레이시간 출력
			timer[10].TransparentBlt(savedc, 400, 0, 15, 20, BLACK);
			sec_Animation(savedc);
			sec2_Animation(savedc);
			min_Animation(savedc);

			controlUI.TransparentBlt(savedc, 0, 360, 785, 200, PURPLE);   //ui출력

			food_cnt1_Animation(savedc);                                 // 먹이 카운트 출력 애니메이션
			food_cnt2_Animation(savedc);
			food_cnt_png[10].TransparentBlt(savedc, 284, 392, 12, 15, WHITE);
			food_cnt_png[6].TransparentBlt(savedc, 296, 392, 12, 15, WHITE);
			food_cnt_png[0].TransparentBlt(savedc, 309, 392, 12, 15, WHITE);

			skill_cnt1_Animation(savedc);                                 // 먹이 카운트 출력 애니메이션
			skill_cnt2_Animation(savedc);
			food_cnt_png[10].TransparentBlt(savedc, 510, 392, 12, 15, WHITE);
			food_cnt_png[9].TransparentBlt(savedc, 522, 392, 12, 15, WHITE);
			food_cnt_png[0].TransparentBlt(savedc, 534, 392, 12, 15, WHITE);

			if (mcttf == TRUE)mt[mct].StretchBlt(savedc, 445, 416, 81, 64, SRCCOPY);// 쿨타임 애니메이션 구현
			if (bcttf == TRUE)bt[bct].StretchBlt(savedc, 612, 416, 81, 64, SRCCOPY);

			if (pause_button == FALSE)pause[1].BitBlt(savedc, 721, 0);         // pause 버튼 + pause 메뉴 draw
			else
			{
				pause[0].BitBlt(savedc, 721, 0);
				pause[2].TransparentBlt(savedc, 200, 75, 349, 309, GRAY);

			}
			if (restart_button == TRUE)pause[3].TransparentBlt(savedc, 200, 75, 349, 309, GRAY);
			if (esc_button == TRUE)pause[4].TransparentBlt(savedc, 200, 75, 349, 309, GRAY);
			level.TransparentBlt(savedc, 10, 10, 30, 30, RGB(145, 0, 200));
			if (!(p.level / 10 == 0))
				food_cnt_png[p.level / 10].TransparentBlt(savedc, 50, 10, 15, 30, WHITE);
			food_cnt_png[p.level % 10].TransparentBlt(savedc, 70, 10, 15, 30, WHITE);

			if (b.hp > 0)
			{
				hBrush = CreateSolidBrush(WHITE);          // BOSS 체력바 배경흰색
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				Rectangle(savedc, startpoint + 1270, 70, startpoint + 1470, 80);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush(RGB(255, 0, 0));          // BOSS 체력바 
				oldBrush = (HBRUSH)SelectObject(savedc, hBrush);
				Rectangle(savedc, startpoint + 1270, 70, startpoint + 1270 + b.hp*(200 / b.maxhp), 80);
				DeleteObject(hBrush);
			}
			break;
		}



		BitBlt(hdc, 0, 0, WD_WIDTH, WD_HEIGHT, savedc, 0, 0, SRCCOPY);

		DeleteDC(savedc);
		DeleteObject(hbitmap);

		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			switch (select_screen)
			{
			case e_victory:
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				KillTimer(hWnd, 5);
				KillTimer(hWnd, 6);
				KillTimer(hWnd, 7);
				KillTimer(hWnd, 8);
				KillTimer(hWnd, 9);
				KillTimer(hWnd, 10);
				break;
			case e_gameover:
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				KillTimer(hWnd, 5);
				KillTimer(hWnd, 6);
				KillTimer(hWnd, 7);
				KillTimer(hWnd, 8);
				KillTimer(hWnd, 9);
				KillTimer(hWnd, 10);
				break;
			}
			for (auto q = z.begin(); q < z.end(); q++)
			{
				check_pala_z(p, *q);

			}
			for (auto q = z.begin(); q < z.end(); q++)
			{
				for (auto p = m.begin(); p < m.end(); p++)  check_mz1(*p, *q);
			}

			for (auto p = m.begin(); p < m.end(); p++)
			{
				check_mboss(*p, b);
			}

			for (auto p = m.begin(); p < m.end(); p++) {   // 캐릭터 이동
				if (!(p->tf)) {
					p->x += 5;
					p->cnt_an++;
					if (p->cnt_an == 3)   p->cnt_an = 0;
				}


			}


			for (auto p = z.begin(); p < z.end(); p++) {
				if (!(p->tf) && !(p->paladog_tf))   p->x -= 5;
				p->cnt_an++;
				if (p->cnt_an == 3)   p->cnt_an = 0;
			}

			for (auto q = z.begin(); q < z.end(); ) {
				if (q->hp <= 0 || q->x < startpoint) {
					q = z.erase(q);

					for (auto p = m.begin(); p < m.end(); p++)
						p->tf = FALSE;
				}
				else q++;
			}

			for (auto q = m.begin(); q < m.end(); ) {
				if (q->hp <= 0) {
					q = m.erase(q);

					for (auto p = z.begin(); p < z.end(); p++)
						p->tf = FALSE;
				}
				else q++;
			}
			for (auto p = s_1.begin(); p < s_1.end(); )
			{
				p->x += 15;

				for (auto q = z.begin(); q < z.end(); q++) {
					if (q->x - p->x <= 40 && q->x - p->x > 20) {
						q->hp -= p->attack;
						p->life -= 1;
					}

					if (p->life == 0) break;

				}

				if (p->life == 0)
					p = s_1.erase(p);
				else if (p->x >= 1500)
					p = s_1.erase(p);
				else p++;
			}


			if (FS_count) {
				if (food_cnt < 60)food_cnt += 1;         // 먹이 카운트 조절
				{
					food_cnt2 = food_cnt / 10;
					food_cnt1 = food_cnt % 10;
				}

				if (skill_buf) {
					if (skill_cnt < 90)skill_cnt += 1;         // 스킬 카운트 조절
					skill_cnt2 = skill_cnt / 10;
					skill_cnt1 = skill_cnt % 10;
					skill_buf = FALSE;
				}
				else if (!skill_buf)
					skill_buf = TRUE;

				FS_count = FALSE;
			}
			 
			else if (!FS_count)
				FS_count = TRUE;

			break;

		case 2:
			if (fire_attack == TRUE)
			{
				if (fire_cnt < 2)
				{
					fire_cnt++;
				}
				else if (fire_cnt == 2)
				{
					fire_cnt = 0;
					fire_attack = FALSE;
				}
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


		case 7:
			z1_buf++;
			if (z1_buf == 7) {
				z.emplace_back();
				if (TRUE) {
					auto p = z.end();
					(p - 1)->m_make();
				}
				z1_buf = 0;
			}

			z2_buf++;
			if (z2_buf == 24) {
				z.emplace_back();
				if (TRUE) {
					auto p = z.end();
					(p - 1)->b_make();
				}
				z2_buf = 0;
			}
			break;

		case 9:
			for (auto p = m.begin(); p < m.end(); p++)
			{
				for (auto q = z.begin(); q < z.end(); q++)
				{
					t_at(*p, *q);
					p->cnt_at++;
					if (p->cnt_at == 4) p->cnt_at = 0;

					break;
				}
			}

			for (auto p = m.begin(); p < m.end(); p++)
			{
				t_at2(*p, b);
				break;
			}
			break;

		case 10:
			for (auto q = z.begin(); q < z.end(); q++)
			{
				for (auto p = m.begin(); p < m.end(); p++)
				{
					t_at1(*p, *q);
					q->cnt_at++;
					if (q->cnt_at == 4)q->cnt_at = 0;
					break;
				}
			}

			for (auto q = z.begin(); q < z.end(); q++)
			{
				t_at3(p, *q);
				q->cnt_at++;
				if (q->cnt_at == 4)q->cnt_at = 0;

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

				stage_buf = TRUE;

				break;

			case e_ingame:
				if (mx > 445 && mx < 526 && my < 480 && my>416 && food_cnt >= 10 && mcttf == FALSE && !pause_button)      // 쥐새끼 마우스 클릭 생성
				{
					m.emplace_back();
					auto p = m.end();
					(p - 1)->m_make();
					food_cnt -= 10;
					food_cnt2 -= 1;
					SetTimer(hWnd, 3, 200, NULL);
					mcttf = TRUE;

				}
				else if (mx > 612 && mx < 693 && my < 480 && my>416 && food_cnt >= 30 && bcttf == FALSE && !pause_button)   // 곰탱이 마우스 클릭 생성
				{
					m.emplace_back();
					auto p = m.end();
					(p - 1)->b_make();
					food_cnt -= 30;
					food_cnt2 -= 3;
					SetTimer(hWnd, 4, 400, NULL);
					bcttf = TRUE;
				}

				if (mx > 70 && mx < 175 && my < 530 && my>425 && !pause_button) {
					if (background_wsize > 0 && background_wsize <= 770 && screen == 0 && !fire_attack)
					{
						if (!left_sw) {
							left_sw = TRUE;
							p.cnt_an = 7;
						}

						background_wsize -= 5;
						startpoint += 5;
						for (auto p = m.begin(); p < m.end(); p++)p->x += 5;

						for (auto p = z.begin(); p < z.end(); p++)p->x += 5;

						for (auto p = s_1.begin(); p < s_1.end(); p++)p->x += 5;
						b.x += 5;
						if (p.cnt_an <12)
							p.cnt_an++;
						else if (p.cnt_an == 12)
							p.cnt_an = 7;
					}                                                        // 맵 좌측 이동
					else if (screen>0)
					{
						screen -= 5;
						if (!left_sw) {
							left_sw = TRUE;
							p.cnt_an = 7;
						}

						p.x -= 5;

						if (p.cnt_an <12)
							p.cnt_an++;
						else if (p.cnt_an == 12)
							p.cnt_an = 7;

					}
				}

				if (mx > 225 && mx < 330 && my < 530 && my>425 && !pause_button)
				{
					if (background_wsize < 765 && !fire_attack)
					{

						background_wsize += 5;
						startpoint -= 5;

						for (auto p = m.begin(); p < m.end(); p++)p->x -= 5;

						for (auto p = z.begin(); p < z.end(); p++)p->x -= 5;

						for (auto p = s_1.begin(); p < s_1.end(); p++)p->x -= 5;

						b.x -= 5;
						if (p.cnt_an <6)
							p.cnt_an++;
						else if (p.cnt_an == 6)
							p.cnt_an = 1;

					}                                            //맵 우측 이동
					else if (screen >= 0 && screen<350)
					{

						screen += 5;
						p.x += 5;

						if (p.cnt_an <6)
							p.cnt_an++;
						else if (p.cnt_an == 6)
							p.cnt_an = 1;

					}
				}

				if (mx > 452 && mx < 530 && my < 545 && my>480 && !pause_button)
				{
					if (skill_cnt >= 9)
					{
						s1_count++;
						s_1.emplace_back();
						auto p = s_1.end() - 1;
						skill_cnt -= 9;
						skill_cnt2 = skill_cnt / 10;
						skill_cnt1 = skill_cnt % 10;
					}
				}
				if (mx > 618 && mx < 696 && my < 545 && my>480 && !pause_button)
				{
					if (skill_cnt >= 13)
					{
						skill_cnt -= 13;
						skill_cnt2 = skill_cnt / 10;
						skill_cnt1 = skill_cnt % 10;

						for (auto q = z.begin(); q < z.end(); q++) {
							if (p.x + 100 <= q->x && p.x + 230 >= q->x)
								q->hp -= 3;
						}

						fire_attack = TRUE;

					}
				}
				if (mx > 721 && mx < 800 && my>0 && my < 63 && pause_button == FALSE)      // pause 기능 구현 + 종료 + 재시작 기능 구현
				{
					pause_button = TRUE;

				}
				else if (mx > 250 && mx < 500 && my>180 && my < 255 && pause_button == TRUE) { pause_button = FALSE; }
				else if (mx > 250 && mx < 500 && my>275 && my < 350 && pause_button == TRUE)
				{
					select_screen = e_stage;
					Stage = nul;
					reset();
					//PostQuitMessage(0);
					//pause[4].TransparentBlt(savedc, 200, 75, 349, 309, GRAY);


				}
				break;
			}
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_MOUSEMOVE:
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			switch (select_screen) {
			case e_victory:
				if (520 <= mx && mx <= 760 && 450 <= my && my <= 550) victory_sw = TRUE;
				else victory_sw = FALSE;
				break;

			case e_gameover:
				if (560 <= mx && mx <= 800 && 490 <= my && my <= 590) gameover_sw = TRUE;
				else gameover_sw = FALSE;
				break;

			case e_start:
				if (15 <= mx && mx <= 245 && 383 <= my && my <= 553) START = TRUE;
				else START = FALSE;
				break;

			case e_stage:
				if (75 <= mx && mx <= WD_WIDTH / 2 && 140 <= my && my <= 460) stage_1 = TRUE;
				else stage_1 = FALSE;

				if (WD_WIDTH / 2 <= mx && mx <= 710 && 140 <= my && my <= 460) stage_2 = TRUE;
				else stage_2 = FALSE;

				if (700 <= mx && mx <= 765 && 20 <= my && my <= 80) stage_exit = TRUE;
				else stage_exit = FALSE;
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
				if (stage_buf) {
					stage_buf = FALSE;
					if (stage_1) {
						select_screen = e_ingame;
						Stage = stage1;
					}

					else if (stage_2) {
						select_screen = e_ingame;
						Stage = stage2;
					}

					else if (stage_exit) {
						exit(1);
					}
				}


				SetTimer(hWnd, 1, 100, NULL);   // 아군,적군 이동
				SetTimer(hWnd, 2, 150, NULL);
				SetTimer(hWnd, 5, 1000, NULL);   // 타이머

				if (Stage == stage1)
					SetTimer(hWnd, 7, 500, NULL);   //적군1 생성 + 적군2 생성
				else if (Stage == stage2)
					SetTimer(hWnd, 7, 200, NULL);   //적군1 생성 + 적군2 생성
													//SetTimer(hWnd, 8, 15000, NULL);   //적군2 생성
				SetTimer(hWnd, 9, 350, NULL);   // 쥐-적쥐 공격시간
				SetTimer(hWnd, 10, 500, NULL);   // 쥐-적쥐 공격시간
												 //SetTimer(hWnd, 11, 100, NULL);
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
					KillTimer(hWnd, 8);
					KillTimer(hWnd, 9);
					KillTimer(hWnd, 10);
				}
				else if (mx > 250 && mx < 500 && my>180 && my < 255 && pause_button == FALSE)
				{
					SetTimer(hWnd, 1, 100, NULL);
					SetTimer(hWnd, 2, 200, NULL);
					SetTimer(hWnd, 3, 200, NULL);
					SetTimer(hWnd, 4, 400, NULL);
					SetTimer(hWnd, 5, 1000, NULL);
					if (Stage == stage1)
						SetTimer(hWnd, 7, 500, NULL);   //적군1 생성 + 적군2 생성
					else if (Stage == stage2)
						SetTimer(hWnd, 7, 200, NULL);   //적군1 생성 + 적군2 생성
					SetTimer(hWnd, 8, 20000, NULL);   //적군2 생성
					SetTimer(hWnd, 9, 350, NULL);
					SetTimer(hWnd, 10, 500, NULL);
				}
				break;

			case e_gameover:
				if (gameover_sw) {
					select_screen = e_stage;
					Stage = nul;
					reset();
				}

				break;

			case e_victory:
				if (victory_sw) {
					select_screen = e_stage;
					Stage = nul;
					reset();
				}

				break;
			}
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_KEYDOWN:


			if (select_screen == e_ingame) {



				switch (wParam)
				{
				case 'u': case 'U':
					skill_cnt = 90;
					break;

				case 'i': case 'I':
					food_cnt = 60;
					break;

				case 'P':   case 'p':
					b.hp = 0;
					break;

				case 'o':   case 'O':
					p.hp = 0;
					break;

				case '1':
					if (food_cnt >= 10 && mcttf == FALSE && !pause_button)      // 쥐새끼 키다운 생성
					{
						m.emplace_back();
						auto p = m.end();
						(p - 1)->m_make();
						food_cnt -= 10;
						food_cnt2 -= 1;
						SetTimer(hWnd, 3, 200, NULL);
						mcttf = TRUE;
					}
					break;

				case '2':                              // 곰탱이 키다운 생성
					if (food_cnt >= 30 && bcttf == FALSE && !pause_button)
					{
						m.emplace_back();
						auto p = m.end();
						(p - 1)->b_make();
						food_cnt -= 30;
						food_cnt2 -= 3;
						SetTimer(hWnd, 4, 400, NULL);
						bcttf = TRUE;
					}
					break;
				case 'k':
				case 'K':
					if (skill_cnt >= 13)
					{
						skill_cnt -= 13;
						skill_cnt2 = skill_cnt / 10;
						skill_cnt1 = skill_cnt % 10;

						for (auto q = z.begin(); q < z.end(); q++) {
							if (p.x + 100 <= q->x && p.x + 230 >= q->x)
								q->hp -= 3;
						}

						fire_attack = TRUE;

					}
					break;
				case 'j':
				case 'J':
					if (skill_cnt >= 9)
					{
						s1_count++;
						s_1.emplace_back();
						auto p = s_1.end() - 1;
						skill_cnt -= 9;
						skill_cnt2 = skill_cnt / 10;
						skill_cnt1 = skill_cnt % 10;
					}
					break;


					break;



				case VK_RIGHT:      case 'd':   case 'D':
					if (background_wsize < 765 && !fire_attack)
					{

						background_wsize += 5;
						startpoint -= 5;

						for (auto p = m.begin(); p < m.end(); p++)p->x -= 5;

						for (auto p = z.begin(); p < z.end(); p++)p->x -= 5;

						for (auto p = s_1.begin(); p < s_1.end(); p++)p->x -= 5;

						b.x -= 5;
						if (p.cnt_an < 6)
							p.cnt_an++;
						else if (p.cnt_an == 6)
							p.cnt_an = 1;

					}                                            //맵 우측 이동
					else if (screen >= 0 && screen < 350)
					{

						screen += 5;
						p.x += 5;

						if (p.cnt_an < 6)
							p.cnt_an++;
						else if (p.cnt_an == 6)
							p.cnt_an = 1;

					}
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				case VK_LEFT:   case 'a':   case 'A':
					if (background_wsize > 0 && background_wsize <= 770 && screen == 0 && !fire_attack)
					{
						if (!left_sw) {
							left_sw = TRUE;
							p.cnt_an = 7;
						}

						background_wsize -= 5;
						startpoint += 5;
						for (auto p = m.begin(); p < m.end(); p++)p->x += 5;

						for (auto p = z.begin(); p < z.end(); p++)p->x += 5;

						for (auto p = s_1.begin(); p < s_1.end(); p++)p->x += 5;
						b.x += 5;
						if (p.cnt_an < 12)
							p.cnt_an++;
						else if (p.cnt_an == 12)
							p.cnt_an = 7;
					}                                                        // 맵 좌측 이동
					else if (screen > 0)
					{
						screen -= 5;
						if (!left_sw) {
							left_sw = TRUE;
							p.cnt_an = 7;
						}

						p.x -= 5;

						if (p.cnt_an < 12)
							p.cnt_an++;
						else if (p.cnt_an == 12)
							p.cnt_an = 7;

					}
				}
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			}
			break;

		case WM_KEYUP:
			switch (wParam) {

			case VK_RIGHT:   case 'd':   case 'D':
			case VK_LEFT: case 'a': case 'A':
				p.cnt_an = 0;
				left_sw = FALSE;
				break;

		
			}
			break;


		case WM_DESTROY:
			fp = fopen("data.txt", "w");
			fprintf(fp, "%d\n", p.level);
			fclose(fp);
			PostQuitMessage(0);
			break;
   }
   return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


// 플레이타임 카운트
void sec_Animation(HDC savedc)
{
	timer[s_cnt].TransparentBlt(savedc, 440, 0, 15, 20, BLACK);
}
void sec2_Animation(HDC savedc)
{
	timer[s2_cnt].TransparentBlt(savedc, 420, 0, 15, 20, BLACK);
}
void min_Animation(HDC savedc)
{
	timer[m_cnt].TransparentBlt(savedc, 380, 0, 15, 20, BLACK);
}
void min2_Animation(HDC savedc)
{
	timer[m2_cnt].TransparentBlt(savedc, 360, 0, 15, 20, BLACK);
}

// 먹이 카운트
void food_cnt1_Animation(HDC savedc)
{
	food_cnt_png[food_cnt1].TransparentBlt(savedc, 272, 392, 12, 15, WHITE);
}
void food_cnt2_Animation(HDC savedc)
{
	food_cnt_png[food_cnt2].TransparentBlt(savedc, 260, 392, 12, 15, WHITE);
}

//스킬 카운트
void skill_cnt1_Animation(HDC savedc)
{
	food_cnt_png[skill_cnt1].TransparentBlt(savedc, 498, 392, 12, 15, WHITE);
}
void skill_cnt2_Animation(HDC savedc)
{
	food_cnt_png[skill_cnt2].TransparentBlt(savedc, 486, 392, 12, 15, WHITE);
}

void check_mz1(MY &my, ENEMY &en) {

	if (en.x - my.x <= 65) {
		my.tf = TRUE;
		en.tf = TRUE;
	}
}

void check_pala_z(PALADOG &my, ENEMY &en) {

	if (en.x - my.x <= 120 && en.x - my.x >= 0) {
		en.paladog_tf = TRUE;
	}

	else
	{
		en.paladog_tf = FALSE;
	}
}

void check_mboss(MY &my, BOSS &en) {
	if (en.x - my.x <= 45) {
		my.tf = TRUE;
		en.tf = TRUE;
	}
}

// 어택
void t_at(MY &my, ENEMY &en) {
	if (my.tf)
		if (en.tf && en.hp > 0) {


			if (my.attack_count == 0) {
				if (en.hp - my.attack >= 0)   en.hp -= my.attack;
				else if (en.hp - my.attack < 0) en.hp = 0;
			}
			my.attack_count++;
			if (my.attack_count > 3)   my.attack_count = 0;
		}
}

void t_at1(MY &my, ENEMY &en) {
	if (en.tf)
		if (my.tf && my.hp > 0) {
			if (en.attack_count == 0) {
				if (my.hp - en.attack >= 0)   my.hp -= en.attack;
				else if (my.hp - en.attack < 0) my.hp = 0;
			}
			en.attack_count++;
			if (en.attack_count > 3)   en.attack_count = 0;


		}
}

void t_at2(MY &my, BOSS &en) {
	if (my.tf)
		if (en.tf && en.hp > 0) {
			my.attack_count++;
			if (my.attack_count > 3)   my.attack_count = 0;

			if (my.attack_count == 0)   en.hp -= my.attack;
		}
}

void t_at3(PALADOG &pa, ENEMY &en) {
	if (en.paladog_tf)
		if (pa.hp > 0) {
			if (en.attack_count == 0) {
				if (pa.hp - en.attack >= 0)   pa.hp -= en.attack;
				else if (pa.hp - en.attack < 0) pa.hp = 0;
			}
			en.attack_count++;
			if (en.attack_count > 3)   en.attack_count = 0;


		}
}

void reset() {

	background_wsize = 0;
	startpoint = 0;

	skill_cnt = 0;
	skill_cnt1 = 0;
	skill_cnt2 = 0;
	fire_cnt = 0;
	skill_buf = TRUE;
	fire_attack = FALSE;

	z1_buf = 0;
	z2_buf = 0;

	left_sw = FALSE;

	s1_count = 0;
	FS_count = true;

	m_cnt = 0;
	m2_cnt = 0;
	s_cnt = 0;
	s2_cnt = 0;

	screen = 0;
	pause_button = FALSE;
	restart_button = FALSE;
	esc_button = FALSE;

	Stage = NULL;
	START = FALSE;

	food_cnt = 0;
	food_cnt1 = 0;
	food_cnt2 = 0;

	dir_button = FALSE;

	s_cnt = 0;
	s2_cnt = 0;
	m2_cnt = 0;

	mct = 0;
	bct = 0;

	mcttf = FALSE;
	bcttf = FALSE;
	for (auto q = z.begin(); q < z.end(); ) {
		q = z.erase(q);
	}

	for (auto p = m.begin(); p < m.end(); ) {
		p = m.erase(p);
	}

	for (auto p = s_1.begin(); p < s_1.end(); ) {
		p = s_1.erase(p);
	}

	p.x = startpoint + 70;
	p.hp = p.maxhp;
	p.tf = FALSE;
	p.cnt_an = 0;

	b.x = startpoint + 1250;
	b.hp = b.maxhp;
	b.tf = FALSE;
}