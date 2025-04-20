#include "pch.h"
#include "CInputMgr.h"
CInputMgr* CInputMgr::m_InputMgr = nullptr;

CInputMgr::CInputMgr()
	:m_vecKey(256,KEY_STATE::NONE)
{
}

CInputMgr::~CInputMgr()
{
	Release();
}

void CInputMgr::Initialize()
{
}

void CInputMgr::Update()
{
	for (int i = 0; i < m_vecKey.size(); ++i) {
		if (GetAsyncKeyState(i) & 0x8000) { //해당 키를 지금 프레임에 눌렀고
			switch (m_vecKey[i])
			{
			case KEY_STATE::NONE: //전에 상태가 아무것도 아니었다면
				m_vecKey[i] = KEY_STATE::TAP;
				break;

			case KEY_STATE::TAP: //전에 상태가 한번 눌렀다면
				m_vecKey[i] = KEY_STATE::HOLD;
				break;

			case KEY_STATE::HOLD: //이미 눌러져 있는 상태였다면
				m_vecKey[i] = KEY_STATE::HOLD;
				break;

			case KEY_STATE::AWAY: // 전에 상태가 방금 키를 뗀 상태였다면
				m_vecKey[i] = KEY_STATE::TAP;
				break;
			default:
				break;
			}

		}
		else {//해당 키를 지금 프레임에 누르지 않았고
			switch (m_vecKey[i])
			{
			case KEY_STATE::NONE: //전에 상태가 아무것도 아니었다면
				m_vecKey[i] = KEY_STATE::NONE;
				break;
			case KEY_STATE::TAP: //전에 상태가 한번 눌렀다면
				m_vecKey[i] = KEY_STATE::AWAY;
				break;
			case KEY_STATE::HOLD: //이미 눌러져 있는 상태였다면
				m_vecKey[i] = KEY_STATE::AWAY;
				break;
			case KEY_STATE::AWAY: // 전에 상태가 방금 키를 뗀 상태였다면
				m_vecKey[i] = KEY_STATE::NONE;
				break;
			default:
				break;
			}
		}
	}
}

void CInputMgr::Late_Update()
{
}

void CInputMgr::Render(HDC _hDC)
{
}

void CInputMgr::Release()
{
	m_vecKey.clear();
}

bool CInputMgr::KeyDown(int _vKey)
{

	return (m_vecKey[_vKey] == KEY_STATE::HOLD) || (m_vecKey[_vKey] == KEY_STATE::TAP);
}

bool CInputMgr::KeyTap(int _vKey)
{
	return (m_vecKey[_vKey] == KEY_STATE::TAP);
}

bool CInputMgr::KeyHold(int _vKey)
{
	return (m_vecKey[_vKey] == KEY_STATE::HOLD);
}

bool CInputMgr::KeyAway(int _vKey)
{
	return (m_vecKey[_vKey] == KEY_STATE::AWAY);
}
