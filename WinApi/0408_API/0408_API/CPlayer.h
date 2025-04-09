#pragma once
#include "CObj.h"

class CKeyMgr;

class CPlayer :
    public CObj
{

public:
    CPlayer();
    virtual ~CPlayer() override;

public:
    // CObj을(를) 통해 상속됨
   virtual void Initialize() override;
   virtual void Update() override;
   virtual void Render(HDC hdc) override;
   virtual void Release() override;

public:
    void SetKeyMgr(CKeyMgr* _keyMgr);
    void SetBulletList(list<CObj*>* _bulletList);

private:
    void InputKey();
    void Fire(short _Dir);
    CObj* CreateBullet(short _Dir);
    
private :
    DWORD m_atkRate;
    DWORD m_atkCool;
    CKeyMgr* m_KeyMgr;
    list<CObj*>* m_BulletList;
};

