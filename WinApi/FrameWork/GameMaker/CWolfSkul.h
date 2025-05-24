#pragma once
#include "CSkul.h"
class CWolfSkul :
    public CSkul
{
public:
    CWolfSkul();
    ~CWolfSkul()override;
public:
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
    void Enter() override;
    void Exit() override;

    // CSkul을(를) 통해 상속됨
    INFO Get_Skul_AtkPos() override;
    INFO Get_Skul_JUMPAtkPos() override;
    CAttackBox* m_pAtkBox;
    // CSkul을(를) 통해 상속됨
    POINT Get_FRAME(CPlayer::STATE state) override;
};

