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

private:
    void KeyUpdate();
    void MovePlayer();
    void FireBullet();
    
private:
    vector<CGameObject*> m_list;
};

