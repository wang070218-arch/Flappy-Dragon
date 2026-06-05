#pragma once
#include<graphics.h>
#include"../PlayerBird/CPlayerBird.h"

class CColumn
{
public:
	int m_x;
	int m_y;
	int m_style;
	int m_pipeH;
	bool m_IfPass;

	CColumn();
	~CColumn();
	void InitColumn();
	void ShowColumn();
	void MoveColumn(int step);
	bool IfTouchBird(CPlayerBird* player);
};
