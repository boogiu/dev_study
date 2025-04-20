#pragma once
#include "CGameObject.h"
class CPlayer :
    public CGameObject
{
public:
    CPlayer();
    virtual ~CPlayer() override;

public:
   virtual void Initialize() override;
   virtual void Update() override;
   virtual void Late_Update() override;
   virtual void Render(HDC _hDC) override;
   virtual void Release() override;

public:
    void KeyCheck();
    void FireBullet(wstring _bulletName);

private:
    float m_fSpeed;

};

