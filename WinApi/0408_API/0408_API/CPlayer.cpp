#include "pch.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CObj.h"
#include "CBullet.h"

CPlayer::CPlayer()
	:m_KeyMgr(nullptr), m_BulletList(nullptr),m_atkRate(0),m_atkCool(GetTickCount())
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 100.f;
	m_tInfo.fX = static_cast<float>(WINCX >> 1);
	m_tInfo.fY = static_cast<float>(WINCY - m_tInfo.fCY);

	m_fSpeed = 10.f;
	m_atkRate = 500;
}

void CPlayer::Update()
{

	CObj::Update_Rect();
	CObj::OutOfBorder();
	InputKey();
}

void CPlayer::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

}

void CPlayer::Release()
{
}

void CPlayer::SetKeyMgr(CKeyMgr* _keyMgr)
{
	m_KeyMgr = _keyMgr;
}

void CPlayer::SetBulletList(list<CObj*>* _bulletList)
{
	m_BulletList = _bulletList;
}

void CPlayer::InputKey()
{

	KEYINFO tmp = m_KeyMgr->KeyUpdate();

	if (tmp.KeyFlag & static_cast<int>(KEY::UP)) {
		if (!(m_tRect.top <= 0))
			m_tInfo.fY -= m_fSpeed;
	}
	if (tmp.KeyFlag & static_cast<int>(KEY::DOWN)) {
		if (!(m_tRect.bottom >= WINCY))
			m_tInfo.fY += m_fSpeed;
	}
	if (tmp.KeyFlag & static_cast<int>(KEY::LEFT)) {
		if (!(m_tRect.left <= 0))
			m_tInfo.fX -= m_fSpeed;
	}
	if (tmp.KeyFlag & static_cast<int>(KEY::RIGHT)) {
		if (!(m_tRect.right >= WINCX))
			m_tInfo.fX += m_fSpeed;
	}

	int tmpDir(0);
	if (tmp.KeyFlag & static_cast<int>(KEY::W)) {
		tmpDir |= sUpDir;
	}
	if (tmp.KeyFlag & static_cast<int>(KEY::A)) {
		tmpDir |= sLeftDir;
	}
	if (tmp.KeyFlag & static_cast<int>(KEY::S)) {
		tmpDir |= sDownDir;
	}
	if (tmp.KeyFlag & static_cast<int>(KEY::D)) {
		tmpDir |= sRightDir;
	}

	if (0 != tmpDir) {
		Fire(tmpDir);
	}
}


//연사 속도 
//getTickCOunt () - 이전 gettickcount < DWORD 속도 이면, 
void CPlayer::Fire(short _Dir)
{
	if (GetTickCount()< m_atkCool + m_atkRate) { //(이전 시간 + 쿨타임) 만큼이 안되었다면
		return;
	}
	else {
		m_BulletList->push_back(CreateBullet(_Dir));
		m_atkCool = GetTickCount();
	}
}

CObj* CPlayer::CreateBullet(short _Dir)
{
	CObj* tmp = new CBullet(_Dir);
	tmp->Initialize();
	tmp->SetPos(m_tInfo.fX, m_tInfo.fY);
	return tmp;
}
