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
	CTransform* transform = AddComponent<CTransform>();
	transform->Set_Scale(10.f,10.f,10.f);
	transform->Set_Position(10.f,10.f,10.f);
	transform->Set_Rotation(0.f, 0.f,0.f);


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
