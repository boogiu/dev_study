#pragma once
#include "CObject.h"
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
};

