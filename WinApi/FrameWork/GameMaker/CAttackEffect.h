#pragma once
#include "CBaseEffect.h"

class CAttackEffect :
    public CBaseEffect
{
public:
    CAttackEffect();
    ~CAttackEffect();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    virtual bool HasAtkBox() const override { return true; } 
    virtual const CAttackBox* Get_AtkBox() override { return m_pAtkBox; }

private:
    CAttackBox*m_pAtkBox;
};

