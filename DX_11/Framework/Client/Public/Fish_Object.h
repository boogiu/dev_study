#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CFish_Object :
    public CGameObject
{
    enum MoveState { IDLE, FIND, GETTED };
public:
    struct FishSpawnDesc : public GAMEOBJECT_DESC {
        FISH_DATA_DESC FishDataDesc = {};
        ITEM_DATA_DESC itemDataDesc = {};
    };

protected:
    CFish_Object();
    CFish_Object(const CFish_Object& rhs);
    virtual ~CFish_Object() DEFAULT;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(INIT_DESC* pArg) override;
    virtual void Priority_Update(_float dt) override;
    virtual void Update(_float dt) override;
    virtual void Late_Update(_float dt) override;

public:
    virtual virtual void Render_GUI()override;

public:
    virtual virtual void Set_OwnMatrix(_float4x4* OwnMat) { m_pOwnerMatrix = OwnMat; };
    ITEM_DATA_DESC Get_ItemData() { return m_ItemDesc; }

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

private:
    void Check_Axis();
    void Check_Rotation(_float dt);

private:
    MoveState m_eState = { IDLE };

    _float m_fElapseTime = {};
    _float m_fDetectRange = {};

    _float m_fCurrentRadian = {};
    _float m_fDestRadian = {};
    _float2 m_vAxis = {};
    _float4x4* m_pOwnerMatrix = { nullptr };
    FISH_DATA_DESC m_FishDesc = {};
    ITEM_DATA_DESC m_ItemDesc = {};

public:
    static CFish_Object* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END