#pragma once
#include<graphics.h>

class CBackGround
{
public:
	IMAGE m_imgBack[8];
	int m_x;
	int m_y;
	int m_idx;

	CBackGround();
	~CBackGround();

	void InitBackGround();
	void ShowBackGround();
	void MoveBackGround(int step);
};
