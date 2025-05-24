#pragma once
#include "CScene.h"
class CObject;
class CStage;

class CBattleScene :
    public CScene
{
public:
    CBattleScene();
    ~CBattleScene() override;
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    // CScene을(를) 통해 상속됨
    void CloseScene() override;
public:
    void Change_Stage();

private:
    int m_iStageNumber;
    vector <CStage*> m_StageVector;
};

