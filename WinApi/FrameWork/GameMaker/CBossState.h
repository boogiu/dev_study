#pragma once
class CBoss;
#include "CBoss.h"
#include "CAnim.h"
#include "CAttackBox.h"

class CBossState
{
public:
	CBossState();
	virtual ~CBossState();

public:
	virtual void Enter(CBoss* boss) PURE;
	virtual void Update(CBoss* boss) PURE;
	virtual bool Exit(CBoss* boss) PURE;

public:
	ANI_FRAME Get_AnimInfo() { return aniFrame; }
protected :
	ANI_FRAME aniFrame;
	wstring m_bossName;
	CAttackBox* m_pAtkBox;
};

