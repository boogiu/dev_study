#pragma once
#include "CObject.h"
class CBarrel;

class CPlayer :
    public CObject
{
public:
    CPlayer();
   virtual  ~CPlayer() override;
public:
    void Initialize() override;
    void Update(float dt) override;
    void Late_Update(float dt) override;
    void Release() override;

    static ObjectType StaticType() { return ObjectType::PLAYER; }
    ObjectType GetType() override { return StaticType(); };

private:
    void Key_Check();
    void Rotate_Dir(float dt);

private:
    float m_fSpeed;
    D3DXVECTOR3 m_vDirectionTo;
    D3DXVECTOR3 m_vNowDir;
    CBarrel* m_Weapon;
};

