#pragma once
#include "CBase_UI.h"

class CIcon;
class CPlayerFrame :
    public CBase_UI
{
public:
    CPlayerFrame();
    ~CPlayerFrame();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    unordered_map < const TCHAR*, CIcon* > m_IconContainer;

};

