#pragma once
#include "FieldObject.h"
NS_BEGIN(Client)
class CPlant_Fruit :
    public CGameObject
{
    enum Fruit_State {DANGLE, DROP, BOUND, FINDED, IDLE, PICKED};

private:
    CPlant_Fruit();
    CPlant_Fruit(const CPlant_Fruit& rhs);
    virtual ~CPlant_Fruit() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

public:
    void Dangle_Fruit(const string& boneName, _float3 offset);

private:
    void Update_ByState(_float dt);

private:
    void OnGround();
    void MoveToIndex(_float dt);
    void FollowHand(_float dt);

private:
    _float m_MarginY = {};
    _float m_fBoundingTime = {};


    _float3 m_Offset = {};
    TILE_INDEX m_SyncedIndex = {};
    _float4 m_DstPosition = {};
    Fruit_State m_eState = { DROP };

    _float4x4* m_pOwnerMatrix = {};
public:
    static CPlant_Fruit* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END