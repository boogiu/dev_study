#pragma once
#include"CPlayer.h"
class CMonster;
class CSkul {
public:
	enum SkulType{normal , power };
public:
	CSkul();
	virtual ~CSkul();
public:
	virtual void Enter() PURE;
	virtual void Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Exit() PURE;

public:
	virtual void Get_AnimFrame(ANI_FRAME& ani, const TCHAR* state) PURE;
	virtual void DoSkillA() PURE;
	virtual void DoSkillB() PURE;
	virtual void DoJumpAtk() PURE;
	virtual void SwitchSkul() PURE;

public:
	virtual void CollisionSkillA(CObject* obj) PURE;
	virtual void CollisionSkillB(CObject* obj) PURE;
	virtual void EndSkillA() PURE;
	virtual void EndSkillB() PURE;
	virtual INFO Get_Skul_AtkPos() PURE;
	virtual INFO Get_Skul_JUMPAtkPos() PURE;
	virtual POINT Get_FRAME(CPlayer::STATE state) PURE;
	virtual void Find_Target(CMonster* monster);
	virtual void Not_Find_Target();

public:
	const TCHAR* Get_Name()  { return m_sName; };
	 POINT Get_Size()  { return m_SkulSize; };
	 bool CanSkillA() { return bCanSkillA; };
	 bool CanSkillB() { return bCanSkillB; };
	 void SetOwner(CPlayer* _player) { m_pOwner = _player; }
	 SkulType GetType() { return m_skulType; }
	 bool isSwitchOver() { return m_bSwitchEnd; }
	 void SwitchStart() { m_bSwitchEnd = false; }
	 wstring Get_Skull_Icon() { return Skul_Icon; };
	 wstring Get_SkillA_Icon() { return SkillA_Icon; };
	 wstring Get_SkillB_Icon() { return SkillB_Icon; };
protected:
	TCHAR m_sName[128];
	POINT m_SkulSize; //스프라이트 사이즈
	POINT m_FramePoint;
	bool bCanSkillA;
	bool bCanSkillB;
	bool m_bSwitchEnd;

	CPlayer* m_pOwner;
	SkulType m_skulType;

	wstring Skul_Icon;
	wstring SkillA_Icon;
	wstring SkillB_Icon;

};
