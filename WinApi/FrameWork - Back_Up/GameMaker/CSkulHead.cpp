#include "pch.h"
#include "CSkulHead.h"
#include "CResourceMgr.h"
#include "CTimeMgr.h"
#include "CTile.h"
#include "CColider.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

CSkulHead::CSkulHead()
    : skullImg(nullptr), isHitted(false), m_fAngle(0.f)
{
#undef new
    skullImg = new Gdiplus::Image(L"../Image/Projectile/Skul_Skill.png");
#define new DBG_NEW

    m_Colider = new CColider(m_tInfo);
}

CSkulHead::~CSkulHead()
{
    Release();
}

void CSkulHead::Initialize()
{
 
    m_tInfo.Set_Size(30.f, 30.f);
    m_Dir = 1;
    m_Colider->Set_OffsetX(15.f, 15.f);
    m_Colider->Set_OffsetY(15.f, 15.f);

    m_tVector = { 500.f, 0.f };  // 초기 수평 속도
    isHitted = false;
    m_fAngle = 0.f;
}

void CSkulHead::Update()
{
    __super::Update_RECT();

    float delta = CTimeMgr::Get_Instance()->Get_Delta();

    if (!isHitted)
    {
        m_tInfo.fX += m_tVector.fX * m_Dir * delta;
        m_fAngle += delta * 360.f * 5 * m_Dir;
    }
    else
    {
        Add_Velocity(0.f, GRAVITY); // 중력 적용
        m_tInfo.fY += m_tVector.fY * delta;
    }
    if (m_Colider) {
        m_Colider->Update();
        m_Colider->Update_Center(m_tInfo.fX, m_tInfo.fY);
    }
}

void CSkulHead::Late_Update()
{
  
}
void CSkulHead::Render(HDC _hDC)
{
    Graphics graphics(_hDC);
    graphics.SetSmoothingMode(SmoothingModeHighQuality);

    INFO_EX screenInfo = CCamera::Get_Instance()->WorldToScreen(m_tInfo);

    float centerX = screenInfo.fX;
    float centerY = screenInfo.fY;
    float width = screenInfo.X_Size();
    float height = screenInfo.Y_Size();

    graphics.TranslateTransform(centerX, centerY);
    graphics.RotateTransform(m_fAngle);
    graphics.TranslateTransform(-centerX, -centerY);

    if (skullImg)
    {
        graphics.DrawImage(skullImg,
            (INT)(centerX - width / 2),
            (INT)(centerY - height / 2),
            (INT)width,
            (INT)height);
    }

    graphics.ResetTransform();
}


void CSkulHead::Release()
{
    if (m_Colider) {
        Safe_Delete<CColider*>(m_Colider);
    }
}

void CSkulHead::OnCollisionEnter(CObject* object, RECT collRECT)
{
    if (CTile* tile = dynamic_cast<CTile*>(object))
    {
        LONG distanceX = collRECT.right - collRECT.left;

        // 방향 반전
        m_Dir *= -1;

        // 살짝 튕기게 위치 보정
        m_tInfo.fX += (m_Dir > 0 ? distanceX : -distanceX);

        // 튕긴 후 위로 약간 점프
        m_tVector.fY = -200.f;

        // 중력 모드로 전환
        isHitted = true;
    }
}
