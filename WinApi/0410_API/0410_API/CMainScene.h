#pragma once
#include "CScene.h"
class CButton;
class CMainScene :
    public CScene
{
public:
    CMainScene();
    virtual ~CMainScene() override;

public:
    // CScene을(를) 통해 상속됨
   virtual void Initialize() override;
   virtual void Update() override;
   virtual void Render(HDC _hDC) override;
   virtual void Release() override;

private :
    bool m_clicked;
    CButton* m_StartButton;
};

