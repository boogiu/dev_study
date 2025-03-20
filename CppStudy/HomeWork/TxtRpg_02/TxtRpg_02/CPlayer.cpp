#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
	: m_cName(nullptr), m_Class(NOCLASS)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
}

void CPlayer::Render()
{
}

void CPlayer::Release()
{
}

void CPlayer::SetClass(E_CLASS _class)
{
	switch (_class)
	{
	case NOCLASS:
		break;
	case WARRIOR:
		break;
	case MAGE:
		break;
	case ROGUE:
		break;
	case ARCHOR:
		break;
	default:
		break;
	}
}

void CPlayer::ShowStatus()
{
}

void CPlayer::GetExp(CObject* object)
{
}

void CPlayer::LevelUp()
{
}

void CPlayer::Purchase()
{
}

void CPlayer::GetBooty(CObject* object)
{
}

void CPlayer::SetName()
{
	m_cName = CFunction::SafeChar();
}
