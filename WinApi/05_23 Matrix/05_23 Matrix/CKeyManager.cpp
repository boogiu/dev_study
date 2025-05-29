#include "pch.h"
#include "CKeyManager.h"

CKeyManager::CKeyManager()
	:m_KeyContainer(256, KeyState::NONE), m_ptMouse{}
{
}

CKeyManager::~CKeyManager()
{
}

void CKeyManager::Initialize()
{
}

void CKeyManager::Update()
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);

	for (int i = 0; i < 256; ++i) {
		if (GetAsyncKeyState(i) & 0x8000) {
			if (m_KeyContainer[i] == KeyState::NONE || m_KeyContainer[i] == KeyState::AWAY) {
				m_KeyContainer[i] = KeyState::TAP;
			}
			else {
				m_KeyContainer[i] = KeyState::HOLD;
			}
		}
		else {
			if (m_KeyContainer[i] == KeyState::NONE || m_KeyContainer[i] == KeyState::AWAY) {
				m_KeyContainer[i] = KeyState::NONE;
			}
			else {
				m_KeyContainer[i] = KeyState::AWAY;
			}
		}
	}
}

void CKeyManager::Late_Update()
{
}

void CKeyManager::Release()
{
}

bool CKeyManager::Key_Down(int _vk)
{
	return m_KeyContainer[_vk] == KeyState::HOLD || m_KeyContainer[_vk] == KeyState::TAP;
}

bool CKeyManager::Key_Tap(int _vk)
{
	return m_KeyContainer[_vk] == KeyState::TAP;
}

bool CKeyManager::Key_Hold(int _vk)
{
	return m_KeyContainer[_vk] == KeyState::HOLD;
}

bool CKeyManager::Key_Away(int _vk)
{
	return m_KeyContainer[_vk] == KeyState::AWAY;
}

POINT CKeyManager::Get_Cursor()
{
	return m_ptMouse;
}
