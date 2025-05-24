#pragma once
#include "CSkul.h"
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

    // CSkul을(를) 통해 상속됨
    void DoJumpAtk() override;
};

