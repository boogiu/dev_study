#pragma once
#include "CSkul.h"
class CObject;
class CAttackBox;
class CBaseSkul :
    public CSkul
{
public:
    CBaseSkul();
    ~CBaseSkul()override;

public:
    void DoSkillA() override;
    void DoSkillB() override;
    void SwitchSkul() override;
    virtual void Get_AnimFrame(ANI_FRAME& ani, const TCHAR* state) override;
    void Update() override;
    void Late_Update() override;
    void DoJumpAtk() override;
    void CollisionSkillA(CObject* obj) override;
    void CollisionSkillB(CObject* obj) override;
    void EndSkillA() override;
    void EndSkillB() override;
    void Enter() override;
    void Exit() override;

private:
    CAttackBox* m_pAtkBox;
    // CSkul을(를) 통해 상속됨
    INFO Get_Skul_AtkPos() override;
    INFO Get_Skul_JUMPAtkPos() override;
private:
    float CoolTime;
    float ElapsedTime;
    float SwitchTime;
    CObject* mySkul;

    // CSkul을(를) 통해 상속됨
    POINT Get_FRAME(CPlayer::STATE state) override;
};

