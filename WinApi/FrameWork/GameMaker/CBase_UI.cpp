#include "pch.h"
#include "CBase_UI.h"

CBase_UI::CBase_UI()
{
}

CBase_UI::~CBase_UI()
{
}

void CBase_UI::Set_Scale(float xSize, float ySize)
{
	m_tInfo.Set_Size(xSize, ySize);
}

void CBase_UI::AlignToPointX(float x, AL_DIR _dir)
{
	m_tInfo.fX = x;
	switch (_dir)
	{
	case CBase_UI::AL_DIR::LEFT:
		m_tInfo.offsetRight = m_tInfo.X_Size();
		m_tInfo.offsetLeft = 0;
		break;
	case CBase_UI::AL_DIR::RIGHT:
		m_tInfo.offsetLeft = m_tInfo.X_Size();
		m_tInfo.offsetRight = 0;
		break;
	case CBase_UI::AL_DIR::UP:
	case CBase_UI::AL_DIR::DOWN:
	default:
		break;
	}
}

void CBase_UI::AlignToPointY(float y, AL_DIR _dir)
{
	m_tInfo.fY = y;

	switch (_dir)
	{

	case CBase_UI::AL_DIR::UP:
		m_tInfo.offsetBottom = m_tInfo.Y_Size();
		m_tInfo.offsetTop = 0;
		break;
	case CBase_UI::AL_DIR::DOWN:
		m_tInfo.offsetTop = m_tInfo.Y_Size();
		m_tInfo.offsetBottom = 0;
		break;
	case CBase_UI::AL_DIR::LEFT:
	case CBase_UI::AL_DIR::RIGHT:
	default:
		break;
	}
}
void CBase_UI::Make_GapX(float fromX, float distance, AL_DIR move_dir)
{
}

void CBase_UI::Make_GapY(float fromY, float distance, AL_DIR move_dir)
{

}

