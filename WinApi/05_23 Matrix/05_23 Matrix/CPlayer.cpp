#include "pch.h"
#include "CPlayer.h"
#include "Components.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	AddComponent<CTransform>();
}

void CPlayer::Update(float dt)
{
	__super::Update_Component(dt);
}

void CPlayer::Late_Update(float dt)
{
}


void CPlayer::Release()
{
}
