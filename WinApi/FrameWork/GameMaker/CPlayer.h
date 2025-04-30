#pragma once
#include "CObject.h"
class CPlayer :
    public CObject
{
    enum STATE{IDLE,RUN, JUMP,LANDING,FALLING};

public: 
    CPlayer();
    virtual ~CPlayer() override;
public:
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC _hDC) override;
    virtual void Release() override;

private:
    void Key_Update();
    void State_Check();

private:
    float m_animIndex;
    STATE m_NowSTATE;
    vector<vector<POINT>> m_vecAnim;
};
