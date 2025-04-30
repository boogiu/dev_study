#include "pch.h"
#include "CkeyMgr.h"

CkeyMgr* CkeyMgr::m_pInstance = nullptr;

CkeyMgr::CkeyMgr()
{
	ZeroMemory(m_keyArr, sizeof(m_keyArr));
}

CkeyMgr::~CkeyMgr()
{
}

void CkeyMgr::Initialize()
{
}

void CkeyMgr::Update()
{
	for (int i = 0; i < 256; ++i) {
		if (GetAsyncKeyState(i) & 0x8000) {
			if ((m_keyArr[i] == TAP)||(m_keyArr[i]==HOLD)) 
			{
				m_keyArr[i] = HOLD;
			}
			else
			{
				m_keyArr[i] = TAP;
			}
		}
		else 
		{
			if ((m_keyArr[i] == AWAY))
			{
				m_keyArr[i] = NONE;
			}
			else if((m_keyArr[i] == TAP)|| (m_keyArr[i] == HOLD))
			{
				m_keyArr[i] = AWAY;
			}
		}
	}
}

void CkeyMgr::Late_Update()
{
}

void CkeyMgr::Render(HDC _hDC)
{
}

void CkeyMgr::Release()
{
}

bool CkeyMgr::isKeyDown(int _vkKey)
{
	return 	(m_keyArr[_vkKey] == HOLD)|| (m_keyArr[_vkKey] == TAP) ;
}

bool CkeyMgr::isKeyTap(int _vkKey)
{
	return (m_keyArr[_vkKey] == TAP);
}

bool CkeyMgr::isKeyHold(int _vkKey)
{
	return (m_keyArr[_vkKey] == HOLD);
}

bool CkeyMgr::isKeyAway(int _vkKey)
{
	return (m_keyArr[_vkKey] == AWAY);
}
