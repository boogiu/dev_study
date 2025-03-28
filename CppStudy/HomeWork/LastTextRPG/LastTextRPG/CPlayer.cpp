#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
}

void CPlayer::Release()
{
}

void CPlayer::SetName()
{
	while (true)
	{
		wcout << L"사용할 이름을 입력해주세요." << endl;
		m_Sname = CInput::SafeChar();
		wcout << L"당신의 이름은 " << m_Sname << L"입니다." << endl;
		wcout << L"1. 확인 \t 2. 다시 설정" << endl;
		if (CInput::SafeInt(1, 2) == 1) {
			break;
		}
	}
	
}

void CPlayer::SetClass(CLASS _class)
{
}
