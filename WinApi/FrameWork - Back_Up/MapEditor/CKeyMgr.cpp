#include "pch.h"
#include "CKeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
    ZeroMemory(m_KeyArr, sizeof(m_KeyArr));
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::Initialize()
{

}

void CKeyMgr::Update()
{
    for (int i = 0; i < VK_MAX; ++i) {
        if (GetAsyncKeyState(i) & 0x8000) {
            if (m_KeyArr[i] == KEY_STATE::TAP) {
                m_KeyArr[i] = KEY_STATE::HOLD;
            }
            else if (m_KeyArr[i] == KEY_STATE::HOLD) {
                m_KeyArr[i] = KEY_STATE::HOLD;
            }
            else if (m_KeyArr[i] == KEY_STATE::AWAY) {
                m_KeyArr[i] = KEY_STATE::TAP;
            }
            else if (m_KeyArr[i] == KEY_STATE::NONE) {
                m_KeyArr[i] = KEY_STATE::TAP;
            }
        }
        else {
            if (m_KeyArr[i] == KEY_STATE::TAP) {
                m_KeyArr[i] = KEY_STATE::AWAY;
            }
            else if (m_KeyArr[i] == KEY_STATE::HOLD) {
                m_KeyArr[i] = KEY_STATE::AWAY;
            }
            else if (m_KeyArr[i] == KEY_STATE::AWAY) {
                m_KeyArr[i] = KEY_STATE::NONE;
            }
            else {
                m_KeyArr[i] = KEY_STATE::NONE;
            }
        }
    }
}

void CKeyMgr::Late_Update()
{
}

void CKeyMgr::Render(HDC _hDC)
{
}

void CKeyMgr::Release()
{
}

bool CKeyMgr::GetKeyDown(int vk_Key)
{
    return ((m_KeyArr[vk_Key] == KEY_STATE::HOLD) || (m_KeyArr[vk_Key] == KEY_STATE::TAP));
}

bool CKeyMgr::GetKeyTap(int vk_Key)
{
    return (m_KeyArr[vk_Key] == KEY_STATE::TAP);
}

bool CKeyMgr::GetKeyHold(int vk_Key)
{
    return (m_KeyArr[vk_Key] == KEY_STATE::HOLD);
}

bool CKeyMgr::GetKeyAway(int vk_Key)
{
    return (m_KeyArr[vk_Key] == KEY_STATE::AWAY);
}
