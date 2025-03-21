#include "pch.h"
#include "CHealthPotion.h"
#include "CPlayer.h"

CHealthPotion::CHealthPotion()
	:m_HealPoint(0)
{

}

CHealthPotion::~CHealthPotion()
{
}

void CHealthPotion::Init()
{

}

void CHealthPotion::Release()
{
}

void CHealthPotion::Activate(CPlayer * _player)
{
	std::cout << m_ItemInfo.m_cName << "을 사용했습니다." << std::endl;
	_player->Heal(m_HealPoint);
}
