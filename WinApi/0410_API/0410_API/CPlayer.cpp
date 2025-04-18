#include "pch.h"
#include "CPlayer.h"
#include "Managers.h"
#include "Objects.h"
#include "CRadar.h"

CPlayer::CPlayer()
	:m_fJumpPow(0), ShotPrevTime(GetTickCount64()), ShotCool(200), m_Radar(nullptr), m_OnHitTime(0),m_xDir(0)
{
	ZeroMemory(m_szBuffer, lstrlen(m_szBuffer));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.fCX = 60.f;
	m_tInfo.fCY = 60.f;

	m_tInfo.fX = static_cast<float>(WINCX >> 1);
	m_tInfo.fY = static_cast<float>(WINCY - m_tInfo.fCY);

	m_fSpeed = 8.f;
	m_fJumpPow = 15.f;
	if (!m_Radar) {
		m_Radar = new CRadar;
		m_Radar->Initialize();
	}

	CPhysicsMgr::GetInstance().RegisterPhysics(this, OBJ_LAYER::PLAYER, PHYSICS::ALL);
}

void CPlayer::Update()
{
	CObject::Update_Rect();
	CheckKey();
	Update_State();
	MovePlayer();
	m_Radar->SetPos(m_tInfo.fX, m_tInfo.fY);
	m_Radar->Update();
}

void CPlayer::Render(HDC _hDC)
{
	m_Radar->Render(_hDC);
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CPlayer::Release()
{
	Safe_Delete<CObject*>(m_Radar);
}


void CPlayer::Update_State()
{
	
	if (m_sState & sJUMP) {
		m_sState &= (~sMOVE); //점프 중에는 움직일 수 없다.
	}
	else {
		m_sState |= sMOVE;
	}

	if (m_sState & sONHIT) {
		if (m_OnHitTime + 400 < GetTickCount64()) {
				m_sState &= (~sONHIT);
				m_sState |= sMOVE;
		}
	}
	else {
		m_sState |= (sMOVE);
	}
}

void CPlayer::OnCollisionEnter(CObject* _object)
{
	if (dynamic_cast<CMonster*>(_object)) {
		OnHit();
	}
}

void CPlayer::CheckKey()
{
	if (CInputMgr::GetInstance().KeyDown(VK_LEFT)) {
		if(m_sState & sMOVE)
			m_tVelocity.fVX = -1.f;
	}

	if (CInputMgr::GetInstance().KeyDown(VK_RIGHT)) {
		if (m_sState & sMOVE)
			m_tVelocity.fVX = 1.f;
	}

	if (CInputMgr::GetInstance().KeyDown(VK_SPACE)) {
		if (!(m_sState & sJUMP))
			JumpPlayer();
	}

	if (CInputMgr::GetInstance().KeyDown(VK_CONTROL)) {
		FireBullet();
	}
}

void CPlayer::MovePlayer()
{
	if (m_sState& sONHIT) {
		m_tInfo.fX += (10 * m_tVelocity.fVX); //오른쪽
		m_tInfo.fY += (8 * m_tVelocity.fVY); //아래로
	}
	else {
		m_tInfo.fX += (m_fSpeed * m_tVelocity.fVX); //오른쪽
		m_tInfo.fY += (m_fJumpPow * m_tVelocity.fVY); //아래로
	}
	
	//limit
	if (m_tInfo.fY > WINCY - (m_tInfo.fCY * 0.5f)) {
		m_tInfo.fY = WINCY - (m_tInfo.fCY * 0.5f);
		m_tVelocity.fVY = 0.f;
		m_sState &= ~(sJUMP);
	}

}

void CPlayer::JumpPlayer()
{
	m_tVelocity.fVY = -1.f;
	m_sState |= sJUMP;
}

void CPlayer::FireBullet()
{
	if(ShotPrevTime + ShotCool < GetTickCount64()){
		CObject* bullet = CObjMgr::GetInstance().GetBullet(CSceneMgr::GetInstance().GetNowScene());
		bullet->Activate();
		bullet->SetPos(m_tInfo.fX, m_tInfo.fY);
		CObject* tmpTarget= static_cast<CRadar*>(m_Radar)->GetTarget();
		if (tmpTarget) {
			static_cast<CBullet*>(bullet)->SetTarget(tmpTarget->GetPos());
		}
		ShotPrevTime = GetTickCount64();
	}
}

void CPlayer::OnHit()
{
	if (!(m_sState & sONHIT)) {
		m_OnHitTime = GetTickCount64();
		m_tVelocity.fVX *=-1;
		m_tVelocity.fVY = -1;
		m_sState &= ~(sMOVE);
		m_sState |= sONHIT;
	}
}

void CPlayer::Activate()
{
}

void CPlayer::DeActivte()
{
}

