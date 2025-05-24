#include "pch.h"
#include "CPlayer.h"
#include "Managers.h"
#include "PlayerStates.h"
#include "Skuls.h"
#include "CAnim.h"
#include "CColider.h"

CPlayer::CPlayer() : m_pNowSkul(nullptr), m_nowState(IDLE), m_pNowAnim(nullptr),
iMax_JumpCount(2), iMax_DashCount(2), iNow_JumpCount(0), iNow_DashCount(0),
DashCoolTime(2.f), DashElapsedTime(0.f), m_correctX(0.f)
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

	m_pSubSkul = new CEntSkul;
	m_pSubSkul->SetOwner(this);

	m_pNowAnim = new CAnim(this);
	SetAnim(L"IDLE");

	m_Colider = new CColider(m_tInfo);
	m_Colider->Set_OffsetX(25.f, 25.f);
	m_Colider->Set_OffsetY(40.f, 30.f);
}

void CPlayer::Update()
{

	__super::Update_RECT();

	m_tInfo.fX += m_tVector.fX * 1.5f;
	m_tInfo.fY += m_tVector.fY;

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
	if (m_Colider) {
		m_Colider->Update();
		m_Colider->Update_Center(m_tInfo.fX, m_tInfo.fY);
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
	m_tInfo.fX += m_correctX;
	m_correctX = 0;
	//m_bGround = false;
}

void CPlayer::Render(HDC _hDC)
{
	m_pNowAnim->Render(_hDC);
}

void CPlayer::Release()
{
	ReleaseDC(g_hWnd, memDC);

	if (!m_StateContainer.empty()) {
		auto iter = m_StateContainer.begin();
		for (;iter != m_StateContainer.end(); ++iter) {
			Safe_Delete<CPlayerState*>(iter->second);
		}
	}
	if (m_pNowSkul) {
		Safe_Delete<CSkul*>(m_pNowSkul);
	}
	if (m_pSubSkul) {
		Safe_Delete<CSkul*>(m_pSubSkul);
	}
	if (m_pNowAnim) {
		Safe_Delete<CAnim*>(m_pNowAnim);
	}
	if (m_Colider) {
		Safe_Delete<CColider*>(m_Colider);
	}
}

void CPlayer::OnCollisionEnter(CObject* object, RECT collRECT)
{
	if (CTile* tile = dynamic_cast<CTile*>(object)) {
		if (tile->Get_Role() == WALL) {
			LONG distanceX = collRECT.right - collRECT.left;
			LONG distanceY = collRECT.bottom - collRECT.top;

			if (distanceX < distanceY) {
				// 수평 충돌이 맞는 경우에만 처리
				if (m_tVector.fX > 0) {
					// 오른쪽으로 이동 중 → 오른쪽 벽 충돌
					if (m_correctX > -distanceX)
						m_correctX = -distanceX;
				}
				else if (m_tVector.fX < 0) {
					// 왼쪽으로 이동 중 → 왼쪽 벽 충돌
					if (m_correctX < distanceX)
						m_correctX = distanceX;
				}

				m_tVector.fX = 0;
			}
		}
	}
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


bool CPlayer::isOnGround()
{
	return m_bGround;
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
	CSkul* tmp = m_pNowSkul;
	m_pNowSkul = m_pSubSkul;
	m_pSubSkul = tmp;
	m_pNowSkul->SwitchStart();
}

void CPlayer::ChangeSkul(CSkul* m_pCurSkul)
{

}