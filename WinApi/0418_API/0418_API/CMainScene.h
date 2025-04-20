#pragma once
#include "CScene.h"
class CMainScene :
    public CScene
{
public: 
    CMainScene();
    virtual ~CMainScene() override;

public:
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC _hDC) override;
    virtual void Release() override;
};

