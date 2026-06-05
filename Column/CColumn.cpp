#include <stdio.h>
#include <stdlib.h>
#include "CColumn.h"
#include "../GameConfig/GameConfig.h"
#include "../BirdAPP/CBirdAPP.h"

static const int PIPE_GAP = 149;
static IMAGE s_up[5], s_upF[5], s_low[5], s_lowF[5];
static int s_upH[5];
static bool s_loaded = false;

CColumn::CColumn()
{
	m_x = 0; m_y = 0; m_style = 0; m_pipeH = 0; m_IfPass = 0;
}

CColumn::~CColumn()
{
}

void CColumn::InitColumn()
{
	if (!s_loaded) {
		for (int i = 0; i < 5; i++) {
			char p[64];
			sprintf_s(p, "./res/column_up_%d.png", i + 1);
			loadimage(&s_up[i], p);
			sprintf_s(p, "./res/column_up_%d_front.png", i + 1);
			loadimage(&s_upF[i], p);
			sprintf_s(p, "./res/column_low_%d.png", i + 1);
			loadimage(&s_low[i], p);
			sprintf_s(p, "./res/column_low_%d_front.png", i + 1);
			loadimage(&s_lowF[i], p);
			s_upH[i] = s_up[i].getheight();
		}
		s_loaded = true;
	}

	m_style = rand() % 5;
	m_pipeH = s_upH[m_style];
	m_x = WINDOW_WIDTH + 5;
	m_y = (-CBirdAPP::m_rand % (340)) - 120;
	m_IfPass = 0;
}

void CColumn::ShowColumn()
{
	int s = m_style;
	int h = m_pipeH;
	int gap = PIPE_GAP;

	// Upper: SRCAND then SRCPAINT (direct from static, no copy)
	putimage(m_x, m_y, IMG_COLUMN_WIDTH, h, &s_upF[s], 0, 0, SRCAND);
	putimage(m_x, m_y, IMG_COLUMN_WIDTH, h, &s_up[s], 0, 0, SRCPAINT);

	// Lower: SRCAND then SRCPAINT
	int lh = s_low[s].getheight();
	putimage(m_x, m_y + h + gap, IMG_COLUMN_WIDTH, lh, &s_lowF[s], 0, 0, SRCAND);
	putimage(m_x, m_y + h + gap, IMG_COLUMN_WIDTH, lh, &s_low[s], 0, 0, SRCPAINT);
}

void CColumn::MoveColumn(int step)
{
	m_x -= step;
}

bool CColumn::IfTouchBird(CPlayerBird* player)
{
	int h = m_pipeH;
	int gap = PIPE_GAP;

	int birdL = player->m_x;
	int birdR = player->m_x + IMG_BIRD_WIDTH * 3 / 4;
	int birdT = player->m_y + IMG_BIRD_HEIGHT * 1 / 4;
	int birdB = player->m_y + IMG_BIRD_HEIGHT * 3 / 4;

	int colL = m_x;
	int colR = m_x + IMG_COLUMN_WIDTH;

	if (colL <= birdR && colR >= birdL &&
		(birdT <= m_y + h || birdB >= m_y + h + gap))
	{
		return true;
	}
	return false;
}
