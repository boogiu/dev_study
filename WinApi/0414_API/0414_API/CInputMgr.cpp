#include "pch.h"
#include "CInputMgr.h"

CInputMgr::CInputMgr()
    :m_KeyArray{}
{
}

CInputMgr::~CInputMgr()
{
    Release();
}

void CInputMgr::Initialize()
{
    m_KeyArray.fill(KEY_STATE::NONE);
}

void CInputMgr::Update()
{
    for (int i = 0; i < m_KeyArray.size(); ++i) {
        if (GetAsyncKeyState(i) & 0x8000) {
            if (m_KeyArray[i] == KEY_STATE::TAP || m_KeyArray[i] == KEY_STATE::HOLD) {
                m_KeyArray[i] = KEY_STATE::HOLD;
            }
            else {
                m_KeyArray[i] = KEY_STATE::TAP;
            }
        }
        else {
            if (m_KeyArray[i] == KEY_STATE::TAP || m_KeyArray[i] == KEY_STATE::HOLD) {
                m_KeyArray[i] = KEY_STATE::AWAY;
            }
            else {
                m_KeyArray[i] = KEY_STATE::NONE;
            }
        }
  }
}

void CInputMgr::Late_Update()
{
}

void CInputMgr::Render()
{
}

void CInputMgr::Release()
{

}

bool CInputMgr::GetKeyDown(int v_key)
{
    return (m_KeyArray[v_key] == KEY_STATE::TAP) || (m_KeyArray[v_key]==KEY_STATE::HOLD);
}

bool CInputMgr::GetKeyTap(int v_key)
{
    return m_KeyArray[v_key] == KEY_STATE::TAP;
}

bool CInputMgr::GetKeyHold(int v_key)
{
    return m_KeyArray[v_key] == KEY_STATE::HOLD;
}

bool CInputMgr::GetKeyAway(int v_key)
{
    return m_KeyArray[v_key] == KEY_STATE::AWAY;
}
