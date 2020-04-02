#include "Mouse.h"

ScreenPoint::ScreenPoint(int _x, int _y) 
	: x(_x), y(_y) {}

Mouse::Mouse()
	: m_lButton(false), m_rButton(false)
{
	POINT pt;
	GetCursorPos(&pt);
	m_curPos.x = m_prevPos.x = pt.x;
	m_curPos.y = m_prevPos.y = pt.y;
}

bool Mouse::isLButtonPressed() const
{
	return m_lButton;
}

bool Mouse::isRButtonPressed() const
{
	return m_rButton;
}

ScreenPoint  Mouse::getCurrentPos() const
{
	return m_curPos;
}

ScreenPoint  Mouse::getPrevPos() const
{
	return m_prevPos;
}