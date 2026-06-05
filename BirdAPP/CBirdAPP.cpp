#include"CBirdAPP.h"
#include"../GameConfig/GameConfig.h"
#include<time.h>
#include <string>

WND_PARAM(200, 100, WINDOW_WIDTH, WINDOW_HEIGHT, "FlappyBird")
DYNAMIC_CREATE(CBirdAPP)

int CBirdAPP::m_rand = 0;

CBirdAPP::CBirdAPP()
{
	m_GameStatus = 0; // ��ʼ״̬Ϊ��δ��ʼ��Ϸ
	m_score = 0;
}
CBirdAPP::~CBirdAPP()
{

}


void CBirdAPP::On_SetTimer()
{
	if (m_GameStatus == 1) // �����Ϸ��ʼ��
	{
		// �趨��ʱ����������Ķ�ʱ�� ID 2
		SetTimer(hwnd, TIMER_RAND_NEW_TIMERID, TIMER_RAND_NEW_INTERVAL, nullptr);
		// �趨��ʱ�½����ӵĶ�ʱ�� ID 3
		SetTimer(hwnd, TIMER_NEW_COLUMN_TIMERID, TIMER_NEW_COLUMN_INTERVAL, nullptr);
		// �趨��ʱչʾ&�ı�С��Ķ�ʱ�� ID 5
		SetTimer(hwnd, TIMER_BIRD_POSITION_TIMERID, TIMER_BIRD_POSITION_INTERVAL, nullptr);
	}
	else if (m_GameStatus == 0)
	{
		//�趨��ʱ�ƶ������Ķ�ʱ��  ID 1
		SetTimer(hwnd, TIMER_GROUND_MOVE_TIMERID, TIMER_GROUND_MOVE_INTERVAL, nullptr);
		// �趨��ʱ�޸�С��IMG״̬�Ķ�ʱ�� ID 4
		SetTimer(hwnd, TIMER_BIRD_CHANGE_TIMERID, TIMER_BIRD_CHANGE_INTERVAL, nullptr);
		// �趨��ʱչʾ&�ı�С��Ķ�ʱ�� ID 5
		SetTimer(hwnd, TIMER_BIRD_SHOW_TIMERID, TIMER_BIRD_SHOW_INTERVAL, nullptr);
	}
}


void CBirdAPP::On_Init()
{
	if (m_GameStatus == 1) // �����Ϸ��ʼ��
	{
		
		srand((unsigned int)time(NULL));
		m_AfterGame.InitAfterGame();
		On_SetTimer();
	}
	else if (m_GameStatus == 0)
	{
		m_score = 0;
			m_ColumnBox.m_ColLst.clear();
		m_BackGround.InitBackGround();
		m_Ground.InitGround();
		m_PlayerBird.InitPlayerBird();
		m_BeforeGame.InitBeforeGame();
		On_SetTimer();
	}
}



void CBirdAPP::ShowLogo()
{
	settextcolor(WHITE);
	settextstyle(20, 0, "Arial");
	setbkmode(TRANSPARENT);
	outtextxy(10, IMG_GROUND_POS_Y + 10, "NUIST");
	outtextxy(10, IMG_GROUND_POS_Y + 40, "Flappy Dragon");
}

void CBirdAPP::On_Paint()
{
	if (m_GameStatus == 1)
	{
		m_BackGround.ShowBackGround();
		m_ColumnBox.ShowAllColumn();
		m_Ground.ShowGround();
		ShowLogo();
		ShowScore();
		m_PlayerBird.ShowPlayerBird();
	}
	else if (m_GameStatus == 0)
	{
		m_BackGround.ShowBackGround();
		m_Ground.ShowGround();
		ShowLogo();
		m_BeforeGame.ShowBeforeGame();
		m_PlayerBird.ShowPlayerBird();
	}
	else if (m_GameStatus == 2)
	{
		m_BackGround.ShowBackGround();
		m_ColumnBox.ShowAllColumn();
		m_Ground.ShowGround();
		ShowLogo();
		m_PlayerBird.ShowPlayerBird();
		m_AfterGame.ShowAfterGame();
		ShowScore();
	}
}


void CBirdAPP::On_Close()
{
	KillTimer(hwnd, 1);
	KillTimer(hwnd, 2);
	KillTimer(hwnd, 3);
	KillTimer(hwnd, 4);
	KillTimer(hwnd, 5);
	KillTimer(hwnd, 6);
	m_ColumnBox.m_ColLst.clear();
}


void CBirdAPP::On_WM_LBUTTONDOWN(POINT& po)
{
	//m_PlayerBird.m_direct = 1;
	if (m_GameStatus == 1) // �����Ϸ�Ѿ���ʼ
	{
		m_PlayerBird.m_speed = BIRD_STARTSPEED;
	}
	// �����Ϸû�п�ʼ�����������ڿ�ʼ��Χ��
	else if (m_GameStatus == 0 && po.x >= BUTTON_POS_X_1 && po.x <= BUTTON_POS_X_2 && po.y >= BUTTON_POS_Y_1 && po.y <= BUTTON_POS_Y_2) // �����Ϸû�п�ʼ�����������ڿ�ʼ��Χ��
	{
		m_GameStatus = 1; // ��Ϸ��ʼ
		On_Init();
	}
	// �����Ϸ�Ѿ���ʼ������������������Χ��
	else if (m_GameStatus == 2 && po.x >= BUTTON2_POS_X_1 && po.x <= BUTTON2_POS_X_2 && po.y >= BUTTON2_POS_Y_1 && po.y <= BUTTON2_POS_Y_2) 
	{
		m_GameStatus = 0; // ����
		On_Init();
	}
}


void CBirdAPP::On_WM_TIMER(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case TIMER_GROUND_MOVE_TIMERID:
	{
		m_BackGround.MoveBackGround(1);
		m_Ground.MoveGround(TIMER_GROUND_MOVE_STEP);
		m_ColumnBox.MoveAllColumn(TIMER_GROUND_MOVE_STEP);
		list<CColumn*>::iterator ite = m_ColumnBox.m_ColLst.begin();
		while (ite != m_ColumnBox.m_ColLst.end())
		{
			if (*ite)
			{
				if ((*ite)->IfTouchBird(&m_PlayerBird))
				{
					ReStartStop();
					m_GameStatus = 2;
				}
				if ((*ite)->m_x + IMG_COLUMN_WIDTH <= m_PlayerBird.m_x&&!(*ite)->m_IfPass)
				{
					(*ite)->m_IfPass = true;
					m_score++;
				}
			}
			ite++;
		}
	}
	break;
	case TIMER_RAND_NEW_TIMERID:
	{
		m_rand = rand();
	}
	break;
	case TIMER_NEW_COLUMN_TIMERID:
	{
		CColumn* newCol = new CColumn;
		newCol->InitColumn();
		m_ColumnBox.m_ColLst.push_back(newCol);
	}
	break;
	case TIMER_BIRD_CHANGE_TIMERID:
	{
		if (m_PlayerBird.m_mode + 1 <= 7)
		{
			m_PlayerBird.m_mode++;
		}
		else
		{
			m_PlayerBird.m_mode = 0;
		}
	}
	break;
	case TIMER_BIRD_SHOW_TIMERID:
	{

		m_PlayerBird.ShowPlayerBird();
	}
	break;
	case TIMER_BIRD_POSITION_TIMERID:
	{
		m_PlayerBird.MovePlayerBird();
		// ����Ƿ���������
		if (m_Ground.IfTouchGround(&m_PlayerBird))
		{
			ReStartStop();
			m_GameStatus = 2;
		}
	}
	break;
	}
}



void CBirdAPP::ReStartStop()
{
	KillTimer(hwnd, 1);
	KillTimer(hwnd, 2);
	KillTimer(hwnd, 3);
	KillTimer(hwnd, 4);
	KillTimer(hwnd, 5);
	KillTimer(hwnd, 6);
}

void CBirdAPP::ShowScore()
{
	string strScore("");
	char sc[5] = { 0 };
	_itoa_s(m_score, sc, 10);
	strScore += sc;

	RECT rect = { 0,0,WINDOW_HEIGHT,WINDOW_WIDTH };
	::settextcolor(RGB(150, 100, 99));  //����������ɫ
	settextstyle(60, 0, _T("����"));
	setbkmode(TRANSPARENT);
	::drawtext(strScore.c_str(), &rect, DT_CENTER | DT_SINGLELINE);  //��ʾ�ַ���
}
