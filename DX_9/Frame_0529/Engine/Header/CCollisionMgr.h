#pragma once
#include "CBase.h"

BEGIN(Engine)
class CCollider;

class ENGINE_DLL CCollisionMgr :
    public CBase
{
    DECLARE_SINGLETON(CCollisionMgr)

private:
    explicit CCollisionMgr();
    virtual ~CCollisionMgr();

public:
    void Update_Collision(float& dt);
    void LateUpdate_Collision(float& dt);
    void Add_Collider(CCollider* pCol);
    void Remove_Collider(CCollider* pCol);
    void Clear_Container();

private:
    unordered_map<COLLIDER_LAYER, unordered_set<CCollider*>> m_ColliderMap;
private:
    void Free();
};

END