#include "Mouse.h"

ScreenPoint::ScreenPoint(int _x, int _y) 
	: x(_x), y(_y) {}

Mouse::Mouse()
	: m_lButton(false), m_rButton(false), m_visible(true)
{
	POINT pt;
	GetCursorPos(&pt);
	m_curPos.x = m_prevPos.x = pt.x;
	m_curPos.y = m_prevPos.y = pt.y;
}

void Mouse::setVisible(bool visible)
{
	if (m_visible == visible) return;
	ShowCursor(visible ? TRUE : FALSE);
	m_visible = visible;
}

void Mouse::setTo(int x, int y, HWND hwnd)
{
	POINT point{ x, y };
	ClientToScreen(hwnd, &point);
	SetCursorPos(point.x, point.y);
	m_curPos.x = x;
	m_curPos.y = y;
	m_prevPos = m_curPos;
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