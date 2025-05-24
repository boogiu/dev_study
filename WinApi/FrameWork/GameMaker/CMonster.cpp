#include "pch.h"
#include "CMonster.h"
#include "CMonsterState.h"
#include "CAnim.h"

CMonster::CMonster()
	:m_pAnim(nullptr), m_nowState(MONSTER_END), m_bTargeting(false), m_pRader(nullptr), m_pTarget(nullptr),
	m_MaxHp(0.f),m_NowHp(0.f), m_bAttack(false), m_bStickDir(false)
{
}

CMonster::~CMonster()
{
}

void CMonster::SetAnim()
{
	if (!m_pAnim) return;

	wstring dir;
	// 방향 추가
	if (m_Dir == -1)
		dir = L"_L";
	else
		dir = L"_R";
	wstring AnimationKey = m_MonsterName + dir;
	ANI_FRAME ani = m_StateContainer[m_nowState]->Get_AnimInfo();
	m_pAnim->Set_Anim(AnimationKey.c_str(), ani);
}

void CMonster::ChangeState(MONSTER_STATE state)
{
	if (m_nowState == state) return;
	if (m_nowState == MONSTER_DEAD) return;

	if (m_StateContainer[m_nowState]) {
		m_StateContainer[m_nowState]->Exit(this);
	}

	m_nowState = state;

	if (m_StateContainer[m_nowState]) {
		m_StateContainer[m_nowState]->Enter(this);
	}
}

void CMonster::Anim_Direction()
{
	if (!m_pAnim) return;
	if (m_nowState ==MONSTER_DEAD||m_nowState==MONSTER_APPEAR) return;
	if (m_bStickDir) return;
	wstring dir;
	// 방향 추가
	if (m_Dir == -1)
		dir = L"_L";
	else
		dir = L"_R";
	wstring AnimationKey = m_MonsterName + dir;
	m_pAnim->ChangeOnlyPath(AnimationKey.c_str());
}

void CMonster::ChangeAnimPath(wstring path)
{
	ANI_FRAME ani = m_StateContainer[m_nowState]->Get_AnimInfo();
	m_pAnim->Set_Anim(path.c_str(), ani);
}

void CMonster::Get_Damage(float Dmg)
{
	m_NowHp -= Dmg;
	if (m_NowHp < 0) {
		m_NowHp = 0;
		ChangeState(MONSTER_DEAD);
	}
}

void CMonster::Attacked()
{
}
