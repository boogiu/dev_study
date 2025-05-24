#pragma once
#include "CObject.h"
class CAnim;
class CAttackLine;
class CSlashEffect :
    public CObject
{
public:
    CSlashEffect();
    ~CSlashEffect();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    ANI_FRAME aniFrame;
    CAnim* m_pAnim;
    CAttackLine* m_AtkLine;

};

