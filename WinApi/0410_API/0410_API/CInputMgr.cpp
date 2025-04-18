#include "pch.h"
#include "CInputMgr.h"

//vk 할당이 256개 있음
CInputMgr::CInputMgr()
: m_vecKey(256, KEY_STATE::NONE)
{
	//ZeroMemory(m_vecKey, sizeof(m_vecKey));
}

CInputMgr::~CInputMgr()
{
	Release();
}

void CInputMgr::Initialize()
{
	m_vecKey.reserve(256);
}

void CInputMgr::Render()
{
}

void CInputMgr::Update()
{
	for (int i = 0; i < m_vecKey.size(); ++i) {
		if (GetAsyncKeyState(i) & 0x8000) { //키가 눌려졌다면 (이전에 없이)

			if (m_vecKey[i] == KEY_STATE::NONE || m_vecKey[i] == KEY_STATE::AWAY) {
				m_vecKey[i] = KEY_STATE::TAP;
			}
			else if(m_vecKey[i] == KEY_STATE::TAP) {
					m_vecKey[i] = KEY_STATE::HOLD;
			}
		}
		//눌려지지 않았다면
		else {
			if (m_vecKey[i] == KEY_STATE::HOLD || m_vecKey[i] == KEY_STATE::TAP) {
				m_vecKey[i] = KEY_STATE::AWAY;
			}
			else if(m_vecKey[i] == KEY_STATE::AWAY) {
				m_vecKey[i] = KEY_STATE::NONE;
			}
		}
	}
}

void CInputMgr::Release()
{
}

bool CInputMgr::KeyDown(int _vKey)
{
	return !(m_vecKey[_vKey] == KEY_STATE::NONE)||(m_vecKey[_vKey] == KEY_STATE::AWAY);
}

bool CInputMgr::KeyTap(int _vKey)
{
	return m_vecKey[_vKey] == KEY_STATE::TAP;
}

bool CInputMgr::KeyHold(int _vKey)
{
	return m_vecKey[_vKey] == KEY_STATE::HOLD;
}

bool CInputMgr::KeyAway(int _vKey)
{
	return m_vecKey[_vKey] == KEY_STATE::AWAY;
}
