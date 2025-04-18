#pragma once
#include "CObject.h"
class CRadar :
    public CObject
{
public:
    CRadar();
    virtual ~CRadar() override;

    // CObject을(를) 통해 상속됨
public:
    void Initialize() override;
    void Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void Activate() override;
    void DeActivte() override;

public :
   CObject* GetTarget();
public:
    virtual void OnCollisionEnter(CObject* _object) override;
    virtual void OnCollisionOut(CObject* _object) override;

private:
    queue<CObject*> m_vecTargets;
};

