#pragma once
#include"CPlayer.h"
class CSkul {
public:
	enum SkulType{normal , power };
public:
	CSkul();
	virtual ~CSkul();
public:
	virtual void Update() PURE;
	virtual void Late_Update() PURE;

public:
	virtual void Get_AnimFrame(ANI_FRAME& ani, const TCHAR* state) PURE;
	virtual void DoSkillA() PURE;
	virtual void DoSkillB() PURE;
	virtual void DoJumpAtk() PURE;
	virtual void SwitchSkul() PURE;

public:
	const TCHAR* Get_Name()  { return m_sName; };
	 POINT Get_Size()  { return m_SkulSize; };
	 bool CanSkillA() { return bCanSkillA; };
	 bool CanSkillB() { return bCanSkillB; };
	 void SetOwner(CPlayer* _player) { m_pOwner = _player; }
	 SkulType GetType() { return m_skulType; }
	 bool isSwitchOver() { return m_bSwitchEnd; }
	 void SwitchStart() { m_bSwitchEnd = false; }

protected:
	TCHAR m_sName[64];
	POINT m_SkulSize; //스프라이트 사이즈
	POINT m_FramePoint;
	bool bCanSkillA;
	bool bCanSkillB;
	bool m_bSwitchEnd;

	CPlayer* m_pOwner;
	SkulType m_skulType;
};
