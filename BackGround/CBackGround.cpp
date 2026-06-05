#include <stdio.h>
#include "CBackGround.h"
#include "../GameConfig/GameConfig.h"

CBackGround::CBackGround()
{
	m_x = 0;
	m_y = 0;
	m_idx = 0;
}

CBackGround::~CBackGround()
{
}

void CBackGround::InitBackGround()
{
	for (int i = 0; i < 8; i++) {
		char path[64];
		sprintf_s(path, "./res/bg_%d.png", i + 1);
		loadimage(&m_imgBack[i], path);
	}
	m_x = 0;
	m_y = 0;
	m_idx = 0;
}

void CBackGround::ShowBackGround()
{
	int imgW = m_imgBack[m_idx].getwidth();
	putimage(m_x, 0, &m_imgBack[m_idx]);
	putimage(m_x + imgW, 0, &m_imgBack[m_idx]);
}

void CBackGround::MoveBackGround(int step)
{
	int imgW = m_imgBack[m_idx].getwidth();
	m_x -= step;
	if (m_x <= -imgW) {
		m_x = 0;
		m_idx = (m_idx + 1) % 8;
	}
}
