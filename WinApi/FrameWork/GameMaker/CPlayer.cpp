#include "pch.h"
#include "CPlayer.h"
#include "Managers.h"
#include "PlayerStates.h"
#include "Skuls.h"
#include "CAnim.h"
#include "CCollider.h"
#include"CTile.h"
#include"CRader.h"
#include"CPlayerRader.h"
#include"CDmgText.h"
#include"CFunction.h"
CPlayer::CPlayer() : m_pNowSkul(nullptr), m_nowState(IDLE), m_pNowAnim(nullptr),
iMax_JumpCount(2), iMax_DashCount(2), iNow_JumpCount(0), iNow_DashCount(0),
DashCoolTime(2.f), DashElapsedTime(0.f), m_correctX(0.f), m_correctY(0.f), m_MaxHp(0.f), m_NowHp(0.f), m_pSubSkul(nullptr)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.fX = 0;
	m_tInfo.fY = 0;

	m_tInfo.Set_Size(200.f, 200.f);
	m_fMoveSpeed = 250.f;
	iMax_JumpCount = 2;
	RestoreJump();

	m_StateContainer.insert({ IDLE,new CPlayerIdleState });
	m_StateContainer.insert({ MOVE,new CPlayerMoveState });
	m_StateContainer.insert({ DASH,new CPlayerDashState });
	m_StateContainer.insert({ JUMP,new CPlayerJumpState });
	m_StateContainer.insert({ FALL, new CPlayerFallState });
	m_StateContainer.insert({ ATTACKA, new CPlayerAttckState });
	m_StateContainer.insert({ ATTACKB, new CPlayerAttckState });
	m_StateContainer.insert({ JUMPATTACK, new CPlayerJumpAtkState });
	m_StateContainer.insert({ SKILLA, new CPlayerSkillAState });
	m_StateContainer.insert({ SKILLB, new CPlayerSkillBState });
	m_StateContainer.insert({ SWITCH, new CPlayerSwitchState });

	m_pNowSkul = new CBaseSkul;
	m_pNowSkul->SetOwner(this);

	//m_pSubSkul = new CEntSkul;
	//m_pSubSkul->SetOwner(this);

	m_pNowAnim = new CAnim(this);
	SetAnim(L"IDLE");

	m_pCollider = new CCollider(this, m_tInfo);
	m_pCollider->Set_OffsetX(25.f, 25.f);
	m_pCollider->Set_OffsetY(40.f, 30.f);

	m_MaxHp = 100.f;
	m_NowHp = m_MaxHp;
}

void CPlayer::Update()
{

	__super::Update_RECT();

	m_tInfo.fX += m_tVector.fX * CTimeMgr::Get_Instance()->Get_Delta() * m_fMoveSpeed;
	float ySpeed = m_tVector.fY * CTimeMgr::Get_Instance()->Get_Delta() * 20 * 0.9;
	if (ySpeed >= 10.f) {
		ySpeed = 10.f;
	}
	else if (ySpeed <= -10.f) {
		ySpeed = -10.f;
	}
	m_tInfo.fY += ySpeed;

	if (m_tVector.fX < 0) { m_Dir = -1; }
	if (m_tVector.fX > 0) { m_Dir = 1; }

	if (m_StateContainer[m_nowState])
		m_StateContainer[m_nowState]->Update(this);

	DashElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
	if (DashCoolTime < DashElapsedTime) {
		RestoreDash();
		DashElapsedTime = 0.f;
	}

	if (m_pNowSkul) {
		m_pNowSkul->Update();
	}
	if (m_pNowAnim) {
		m_pNowAnim->Update();
	}
	if (m_pCollider) {
		m_pCollider->Update();
		m_pCollider->Update_Center(m_tInfo.fX, m_tInfo.fY);
	}

}

void CPlayer::Late_Update()
{
	float friction = 0.5f;
	m_tVector.fX *= friction;
	if (fabs(m_tVector.fX) < 0.1f) m_tVector.fX = 0.f;
	DirectionFlip();

	if (m_pNowSkul) {
		m_pNowSkul->Late_Update();
	}
	if (m_pNowAnim) {
		m_pNowAnim->Late_Update();
	}
	if (m_bMoveLimit) {
		INFO_EX info = m_pCollider->Get_Info();

		// 왼쪽 벽 충돌
		if (info.LT().x <= 0) {
			m_tInfo.fX = info.offsetLeft;
		}

		// 오른쪽 벽 충돌
		if (info.RB().x >= WINCX) {
			m_tInfo.fX = WINCX - info.offsetRight;
		}
	}

}

void CPlayer::Render(HDC _hDC)
{/*
	INFO_EX in = CCamera::Get_Instance()->WorldToScreen(m_tInfo);
	Rectangle(_hDC, in.LT().x, in.LT().y, in.RB().x, in.RB().y);*/
	m_pNowAnim->Render(_hDC);
}

void CPlayer::Release()
{
	ReleaseDC(g_hWnd, memDC);
//
//	if (!m_StateContainer.empty()) {
//		auto iter = m_StateContainer.begin();
//		for (;iter != m_StateContainer.end(); ++iter) {
//			Safe_Delete<CPlayerState*>(iter->second);
//		}
//	}
	for (auto& pair : m_StateContainer) {
		Safe_Delete(pair.second); // 이건 참조로 받음
	}

	m_StateContainer.clear();

	if (m_pNowSkul) {
		Safe_Delete<CSkul*>(m_pNowSkul);
	}
	if (m_pSubSkul) {
		Safe_Delete<CSkul*>(m_pSubSkul);
	}
	if (m_pNowAnim) {
		Safe_Delete<CAnim*>(m_pNowAnim);
	}
	if (m_pCollider) {
		Safe_Delete<CCollider*>(m_pCollider);
	}
}
void CPlayer::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
	if (dynamic_cast<CRader*>(object)) { return; }
	if (dynamic_cast<CPlayerRader*>(object)) { return; }

	if (_hitType == HITTED) {
		CBaseEffect* Effect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
		Effect->Initialize();
		INFO Info;
		if (collRECT.right - collRECT.left != 0) {
			Info = { (float(collRECT.right + collRECT.left) / 2),(float(collRECT.bottom + collRECT.top) / 2), 200, 200 };
		}
		else {
			Info = { m_tInfo.fX,m_tInfo.fY,200,200 };
		}
		ANI_FRAME effectFrame;
		effectFrame.startPoint = { 0,0 };
		effectFrame.endPoint = { 2,3 };
		effectFrame.isLoop = false;
		effectFrame.m_spSize = { 150,150 };
		effectFrame.frameSpeed = 0.05f;
		Effect->Get_Info().Translate_Info(Info);
		Effect->PlayAnim(L"Monster_Atk_Effect", effectFrame);
		Effect->Set_AutoDeaActive(true);

		m_NowHp -= 3;
	}
	else {
		CObject* obj = CObjMgr::Get_Instance()->ReQuestObject<CDmgText>(PARTICLE);
		CDmgText* text = static_cast<CDmgText*>(obj);
		obj->Initialize();
		int i =CFunction::GetInt(10,16);
		text->Set_Text(i);
		obj->Set_Pos(m_pCollider->Get_Info().RB().x+20, m_pCollider->Get_Info().LT().y-20);
		if (m_pNowSkul->GetType() == CSkul::power) {
			CResourceMgr::Get_Instance()->PlaySoundW(L"Hit_Blunt_Large", 0.25f);
		}
		else {
			CResourceMgr::Get_Instance()->PlaySoundW(L"Whip_Hit", 0.2f);
		}

		if (m_nowState != SKILLA && m_nowState != SKILLB) {
			CBaseEffect* Effect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(HAS_DIRECTION));
			Effect->Initialize();
			INFO groundInfo = { ((collRECT.right + collRECT.left) / 2),((collRECT.bottom + collRECT.top) / 2), 150, 150 };
			ANI_FRAME effectFrame;
			effectFrame.startPoint = { 0,0 };
			effectFrame.endPoint = { 1,3 };
			effectFrame.isLoop = false;
			effectFrame.m_spSize = { 150,150 };
			effectFrame.frameSpeed = 0.02f;
			Effect->Get_Info().Translate_Info(groundInfo);
			Effect->PlayAnim(L"Skul_Atk_Effect", effectFrame);
			Effect->Set_AutoDeaActive(true);
		}
		else {
			if (m_nowState == SKILLA) {
				m_pNowSkul->CollisionSkillA(object);
			}
			else {
				m_pNowSkul->CollisionSkillB(object);
			}
		}
	}
}

void CPlayer::OnCollisionEnter(CObject* object, RECT collRECT)
{

}


void CPlayer::ChangeState(STATE newState)
{
	if (m_StateContainer[m_nowState]) {
		m_StateContainer[m_nowState]->Exit(this);
	}

	m_nowState = newState;

	if (m_StateContainer[m_nowState]) {
		m_StateContainer[m_nowState]->Enter(this);
	}
}

void CPlayer::ChangeSkul(wstring Skul,CObject* Item)
{
	CResourceMgr::Get_Instance()->PlaySoundW(L"183_Skull_Get_a_v1", 0.3f);
	CObject* skul = CObjMgr::Get_Instance()->ReQuestObject<CSkulItem>(ITEM);

	if (m_pNowSkul->Get_Name() != std::wstring(L"Base_Skul")){
		static_cast<CSkulItem*>(skul)->Set_Name(m_pNowSkul->Get_Name());
		skul->Initialize();
		skul->Set_Pos(m_tInfo.fX, m_tInfo.fY);
		skul->Add_Velocity(0.f, -10.f);
	}
	size_t pos = Skul.rfind(L"_Item");
	if (pos != wstring::npos)
		Skul.erase(pos);

	if (m_pSubSkul) {
		CSkul* oldSkul = m_pNowSkul;
		m_pNowSkul->Exit();

		if (oldSkul->Get_Name() == Skul)
			return;

		if (Skul == L"Wolf_Skul") m_pNowSkul = new CWolfSkul;
		else if (Skul == L"Ent_Skul") m_pNowSkul = new CEntSkul;

		Safe_Delete<CSkul*>(oldSkul);
		m_pNowSkul->SetOwner(this);
		m_pNowSkul->Enter();
	}
	else {
		if (Skul == L"Wolf_Skul") m_pSubSkul = new CWolfSkul;
		else if (Skul == L"Ent_Skul") m_pSubSkul = new CEntSkul;
		if (m_pSubSkul)
			m_pSubSkul->SetOwner(this);
	}

	m_nowState = IDLE;
}

void CPlayer::SetAnim(const TCHAR* state)
{
	const TCHAR* skulName = m_pNowSkul->Get_Name();
	ANI_FRAME aniFrame;

	TCHAR tmp[64] = {}; // 문자열 누적용 (초기화 중요)

	// 기본 이름 붙이기
	lstrcpy(tmp, skulName);        // 예: "Base_Skul"
	lstrcat(tmp, L"_");            // 예: "Base_Skul_"

	// 방향 추가
	if (m_Dir == -1)
		lstrcat(tmp, L"L");
	else
		lstrcat(tmp, L"R");

	// 애니메이션 프레임 얻기
	m_pNowSkul->Get_AnimFrame(aniFrame, state);
	// 애니메이션 설정	
	m_pNowAnim->Set_Anim(tmp, aniFrame);
}

bool CPlayer::isAnimStart()
{
	if (m_pNowAnim) {
		return m_pNowAnim->isAnimStart();
	}
	return false;
}

bool CPlayer::isAnimEnd()
{
	if (m_pNowAnim) {
		return m_pNowAnim->isAnimEnd();
	}
	return false;
}

void CPlayer::DirectionFlip()
{
	const TCHAR* skulName = m_pNowSkul->Get_Name();
	TCHAR tmp[64] = {}; // 문자열 누적용 (초기화 중요)

	// 기본 이름 붙이기
	lstrcpy(tmp, skulName);        // 예: "Base_Skul"
	lstrcat(tmp, L"_");            // 예: "Base_Skul_"

	// 방향 추가
	if (m_Dir == -1)
		lstrcat(tmp, L"L");
	else
		lstrcat(tmp, L"R");

	m_pNowAnim->ChangeOnlyPath(tmp);
}

void CPlayer::SwapSkul()
{
	if (!m_pSubSkul) return;
	CSkul* tmp = m_pNowSkul;
	m_pNowSkul->Exit();
	m_pNowSkul = m_pSubSkul;
	m_pNowSkul->Enter();
	m_pSubSkul = tmp;
	m_pNowSkul->SwitchStart();
}
