#pragma once
#include "CSkul.h"
class CAttackBox;
class CPlayerRader;

class CEntSkul :
    public CSkul
{
public:
    CEntSkul();
    ~CEntSkul()override;
public:

    // CSkul을(를) 통해 상속됨
    void Update() override;
    void Late_Update() override;
    void Get_AnimFrame(ANI_FRAME& ani, const TCHAR* state) override;
    void DoSkillA() override;
    void DoSkillB() override;
    void SwitchSkul() override;
    void DoJumpAtk() override;
    void CollisionSkillA(CObject* obj) override;
    void CollisionSkillB(CObject* obj) override;
    void EndSkillA() override;
    void EndSkillB() override;
    virtual void Find_Target(CMonster* monster)override;
    virtual void Not_Find_Target()override;

private:
    void FireFiest();
    void SwtichSmoke();

private:
    CAttackBox* m_pAtkBox;
    CObject* m_pTarget;
    vector<CObject*> fiestVector;
    CPlayerRader* m_pRader;
    ANI_FRAME GrabFrame;
    int MaxFiest;
    int NowFiest;
    bool GrabEffect;
    bool FiestFired;
    bool FiestEffect;
    bool FiestSound;

    // CSkul을(를) 통해 상속됨
    void Enter() override;

    // CSkul을(를) 통해 상속됨
    void Exit() override;

    // CSkul을(를) 통해 상속됨
    INFO Get_Skul_AtkPos() override;
    INFO Get_Skul_JUMPAtkPos() override;

    // CSkul을(를) 통해 상속됨
    POINT Get_FRAME(CPlayer::STATE state) override;
};

