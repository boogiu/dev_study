#include "pch.h"
#include "CSkulHead.h"
#include "CResourceMgr.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CTile.h"
#include "CAttackBox.h"
#include "CBaseEffect.h"
#include "CEffectMgr.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

CSkulHead::CSkulHead()
    : skullImg(nullptr), isHitted(false), m_fAngle(0.f), OnSound(false)
{
#undef new
    skullImg = new Gdiplus::Image(L"../Image/Projectile/Skul_Skill.png");
#define new DBG_NEW
    m_pCollider = new CCollider(this,m_tInfo);
}

CSkulHead::~CSkulHead()
{
    Release();
}

void CSkulHead::Initialize()
{
 
    m_tInfo.Set_Size(25.f, 25.f);
    m_Dir = 1;
    m_pCollider->Get_Info().Copy(m_tInfo);
      //    m_pCollider->Set_OffsetX(15.f, 15.f);
      //    m_pCollider->Set_OffsetY(15.f, 15.f);

    m_tVector = { 500.f, 0.f };  // 초기 수평 속도
    isHitted = false;
    m_fAngle = 0.f;


    if (!m_pAtkBox) {
        m_pAtkBox = new CAttackBox(this);
      
    }
    m_pAtkBox->Set_Target(MONSTER);
    m_pAtkBox->Set_Active();
    OnSound = false;
}

void CSkulHead::Update()
{
    __super::Update_RECT();

    float delta = CTimeMgr::Get_Instance()->Get_Delta();
    m_pAtkBox->Get_Info().Copy(m_tInfo);
    m_pAtkBox->Get_Info().Add_OffsetX(8.f, 8.f);
    m_pAtkBox->Get_Info().Add_OffsetY(8.f, 8.f);

    if (!isHitted)
    {
        m_fAngle += delta * 360.f * 5 * m_Dir;

    }
    else if(isHitted&&!m_bGround)
    {
        m_fAngle += delta * 360.f  *0.9* m_Dir;
        Add_Velocity(0.f, GRAVITY*2); // 중력 적용
    }
    else if (isHitted && m_bGround) {
        m_fAngle *= 0.15f;
    }
    if (m_pCollider) {
        m_pCollider->Update();
        m_pCollider->Update_Center(m_tInfo.fX, m_tInfo.fY);
    }

    if (!isOnGround()) {
        m_tInfo.fY += m_tVector.fY * delta*3;
    }

    m_tInfo.fX += m_tVector.fX * m_Dir * delta;

    if (!m_bActive) {
        m_pAtkBox->Set_DeActive();
    }
}

void CSkulHead::Late_Update()
{
    float delta = CTimeMgr::Get_Instance()->Get_Delta();

    if (isOnGround()) {
        m_tVector.fX *=0.85f;
    }
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
    if (m_pCollider) {
        Safe_Delete<CCollider*>(m_pCollider);
    }
    if (m_pAtkBox) {
        Safe_Delete<CAttackBox*>(m_pAtkBox);
    }
}

void CSkulHead::Set_Active(bool _active)
{
    if (_active == false) {
        m_pAtkBox->Set_DeActive();
    }
    m_bActive = _active;
}

void CSkulHead::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
    ANI_FRAME effectFrame;
    effectFrame.startPoint = { 0,0 };
    effectFrame.endPoint = { 2,2 };
    effectFrame.isLoop = false;
    effectFrame.m_spSize = { 100,100 };
    effectFrame.frameSpeed = 0.02f;
    if (_hitType == ATTACK) {
        CBaseEffect* Effect1 = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
        Effect1->Initialize();
        INFO info = { m_tInfo.fX,m_tInfo.fY , 100, 100 };
        Effect1->Get_Info().Translate_Info(info);
        Effect1->PlayAnim(L"Move_Skul_Effect", effectFrame);
        Effect1->Set_AutoDeaActive(true);
    }

    if (CProjectile* tile = dynamic_cast<CProjectile*>(object))
    {
        return;
    }
    else{
        isHitted = true;
        m_Dir * -1;
        m_tVector = { -50.f, -100.f };
    }
    if (!OnSound) {
        CResourceMgr::Get_Instance()->PlaySoundW(L"Whip_Hit", 0.14f);
        OnSound = true;
    }
}

void CSkulHead::OnCollisionEnter(CObject* object, RECT collRECT)
{
}

void CSkulHead::OnCollisionWall(int dir)
{
    isHitted = true;
    m_Dir * -1;
    m_tVector = {-50.f, -100.f };
    ANI_FRAME effectFrame;
    effectFrame.startPoint = { 0,0 };
    effectFrame.endPoint = { 2,2 };
    effectFrame.isLoop = false;
    effectFrame.m_spSize = { 100,100 };
    effectFrame.frameSpeed = 0.02f;
    CBaseEffect* Effect1 = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
    Effect1->Initialize();
    INFO info = { m_tInfo.fX,m_tInfo.fY , 100, 100 };
    Effect1->Get_Info().Translate_Info(info);
    Effect1->PlayAnim(L"Move_Skul_Effect", effectFrame);
    Effect1->Set_AutoDeaActive(true);
    if (!OnSound) {
        CResourceMgr::Get_Instance()->PlaySoundW(L"Whip_Hit", 0.14f);
        OnSound = true;
    }
}
