#pragma once
#include "CSkul.h"
class CObject;
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

    // CSkul을(를) 통해 상속됨
    void Update() override;
    void Late_Update() override;

private:
    float CoolTime;
    float ElapsedTime;

    CObject* mySkul;

    // CSkul을(를) 통해 상속됨
    void DoJumpAtk() override;
};

