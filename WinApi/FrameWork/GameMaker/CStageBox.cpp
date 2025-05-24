#include "pch.h"
#include "CStageBox.h"
#include "CAnim.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CSkulItem.h"
#include "CObjMgr.h"
#include "CResourceMgr.h"
CStageBox::CStageBox() : m_pAnim(nullptr)
{
}

CStageBox::~CStageBox()
{
	Release();
}

void CStageBox::Initialize()
{
	INFO info = { WINCX / 2, 400, 150,94 };
	m_tInfo.Translate_Info(info);

	if (!m_pAnim) {
		m_pAnim = new CAnim(this);
	}

	m_tAnim.startPoint = { 0,0 };
	m_tAnim.endPoint = { 0,3 };
	m_tAnim.m_spSize = { 100,63 };
	m_tAnim.LoopPoint = { 0,0 };
	m_tAnim.isLoop = false;
	m_tAnim.frameSpeed = 0.1f;

	m_pAnim->Set_Anim(L"Stage_Box", m_tAnim);
	m_pAnim->Set_BreakPoint({ 0,0 });
	m_bBoxOpen = false;
}

void CStageBox::Update()
{
	if(CCollisionMgr::Get_Instance()->Trigger_With_Player(this)) {
		if (CKeyMgr::Get_Instance()->GetKeyTap('F')) {
			m_pAnim->Set_Stop(false);
		
		}
	}

	if (m_pAnim) {
		m_pAnim->Update();
	}

	if (m_pAnim->isAnimEnd()) {
		if (!m_bBoxOpen) {
			CResourceMgr::Get_Instance()->PlaySoundW(L"chest_open1",0.3f);

			CObject* Jusin = CObjMgr::Get_Instance()->ReQuestObject<CJusin>(ITEM);
			Jusin->Initialize();
			Jusin->Set_Pos(m_tInfo.fX, m_tInfo.fY);
			m_bBoxOpen = true;
		}
	}
}

void CStageBox::Late_Update()
{
	if (m_pAnim) {
		m_pAnim->Late_Update();
	}
}

void CStageBox::Render(HDC _hDC)
{
	if (m_pAnim) {
		m_pAnim->Render(_hDC);
	}
}

void CStageBox::Release()
{
	if (m_pAnim) {
		Safe_Delete<CAnim*>(m_pAnim);
	}
}
