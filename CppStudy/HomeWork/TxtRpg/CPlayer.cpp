#include "CPlayer.h"
#include "pch.h"

CPlayer::CPlayer()
	:cName(nullptr)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	broker.SetPlayer(this); //브로커에게 자신 전달
}

void CPlayer::Update()
{
}

void CPlayer::Render()
{
}

void CPlayer::Release()
{
	delete[] cName;
}

void CPlayer::SetName()
{
	if (cName != nullptr) return;
	cout << "누군가 당신의 이름을 물어봅니다" << endl;
	cout << "당신을 무엇이라고 소개하시겠습니까?" << endl;
	cName = SafeChar();
}

char*  CPlayer::GetName()
{
	if (cName == nullptr) return nullptr;

	return cName;
}

void CPlayer::SetClass(int _class)
{
}

void CPlayer::GetClass()
{
}
