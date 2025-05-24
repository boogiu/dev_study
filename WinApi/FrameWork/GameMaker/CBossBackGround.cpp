#include "pch.h"
#include "CBossBackGround.h"
#include "CResourceMgr.h"
#include "CAnim.h"
#include "CBoss.h"
#include "CTimeMgr.h"

CBossBackGround::CBossBackGround()
	:m_pAnim(nullptr), m_bFadeOut(false), m_bFadeSound(false)
{
}

CBossBackGround::~CBossBackGround()
{
	Release();
}

void CBossBackGround::Initialize()
{
	INFO bossInfo = { WINCX/2,190,300,300 };
	m_tInfo.Translate_Info(bossInfo);
	m_pAnim = new CAnim(this);
	SetAnim(BOSS_IDLE);
	m_bFadeOut = false;
	m_bActive = true;
}

void CBossBackGround::Update()
{
	m_pAnim->Update();

	if (m_bFadeOut) {
		float dt = CTimeMgr::Get_Instance()->Get_Delta();
		m_tInfo.Add_OffsetX(-20 * dt, -20 * dt);
		m_tInfo.Add_OffsetY(-20 * dt, -20.f * dt);
		m_tInfo.fY += 7 * dt;
		if (!m_bFadeSound) {
			m_bFadeSound = true;
		}
	}
	if (m_tInfo.X_Size() < 150) {
		m_bActive = false;
	}
}

void CBossBackGround::Late_Update()
{
	m_pAnim->Late_Update();
}

void CBossBackGround::Render(HDC _hDC)
{
	if(m_bActive)
	m_pAnim->Render(_hDC);
}

void CBossBackGround::Release()
{
	Safe_Delete<CAnim*>(m_pAnim);
}

void CBossBackGround::SetAnim(BOSS_STATE _state)
{
	if (prevState == _state) return;

	ANI_FRAME aniFrame;
	switch (_state)
	{
	case BOSS_INTRO:
		break;
	case BOSS_IDLE:
		aniFrame.m_spSize = { 300,300 };
		aniFrame.startPoint = { 0,0 };
		aniFrame.endPoint = { 6,0 };
		aniFrame.isLoop = true;
		aniFrame.frameSpeed = 0.2f;
		aniFrame.LoopPoint = { 0,0 };
		break;
	case BOSS_DEAD1:
		aniFrame.m_spSize = { 300,300 };
		aniFrame.startPoint = { 0,3 };
		aniFrame.endPoint = { 10,3 };
		aniFrame.isLoop = false;
		aniFrame.frameSpeed = 0.2f;
		aniFrame.LoopPoint = { 0,0 };
		break;
	case BOSS_WAIT:
		break;
	case BOSS_GROUND:
		break;
	case BOSS_METEOR:
		break;
	case BOSS_HOMING:
		break;
	case BOSS_SLASH:
		break;
	case BOSS_ENHANCE:
		break;
	case BOSS_E_IDLE:
		break;
	case BOSS_E_DEAD:
		aniFrame.m_spSize = { 300,300 };
		aniFrame.startPoint = { 0,5 };
		aniFrame.endPoint = { 6,5 };
		aniFrame.isLoop = true;
		aniFrame.LoopPoint = { 0,5 };
		aniFrame.frameSpeed = 0.1f;
		m_bFadeOut = true;
		break;
	case BOSS_E_GROUND:
		break;
	case BOSS_E_METEOR:
		break;
	case BOSS_E_RUSH:
		break;
	case BOSS_NONE:
		break;
	default:
		break;
	}
	prevState = _state;
	// 애니메이션 설정	
	m_pAnim->Set_Anim(L"Boss_BackGround", aniFrame);
}
