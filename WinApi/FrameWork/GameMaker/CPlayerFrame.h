#pragma once
#include "CBase_UI.h"

class CIcon;
class CPlayerFrame :
    public CBase_UI
{
public:
    CPlayerFrame();
    ~CPlayerFrame();
    enum class COMPONENT{
        HP_BAR,A_BTN,S_BTN,
        SKUL_1, SKUL_2,
        SKILL_1, SKILL_2,
        MAX_UI
    };
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    //unordered_map < COMPONENT, CIcon* > m_IconContainer;
    unordered_map < const TCHAR*, CIcon* > m_IconContainer;

    float maxBarLength;
    float nowBarLength;
};

