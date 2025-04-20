#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
	: m_fSpeed(0.f)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
}

void CPlayer::Update()
{
	__super::Update_Rect();
	KeyCheck();
}

void CPlayer::Late_Update()
{
}

void CPlayer::Render(HDC _hDC)
{
}

void CPlayer::Release()
{
}

void CPlayer::KeyCheck()
{
}

void CPlayer::FireBullet(wstring _bulletName)
{
}
