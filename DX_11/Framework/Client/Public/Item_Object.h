#pragma once
#include "FieldObject.h"
NS_BEGIN(Client)

class CItem_Object final:
    public CGameObject
{
protected:
    TILE_FLAG CantDrop = TILE_FLAG::FLAG_ONITEM | TILE_FLAG::FLAG_STONE | TILE_FLAG::FLAG_TREE | TILE_FLAG::FLAG_BLOCKED;
public:
    enum ITEM_STATE { IDLE, DROP, THROW, BOUND, FINDED, PICKED, READY_DESTROY };

public:
    struct DROP_ITEM_DESC : GAMEOBJECT_DESC{
        ITEM_DATA_DESC itemDesc = {};
    };

protected:
    CItem_Object();
    CItem_Object(const CItem_Object& rhs);
    virtual ~CItem_Object() DEFAULT;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(INIT_DESC* pArg) override;
    virtual void Priority_Update(_float dt) override;
    virtual void Update(_float dt) override;
    virtual void Late_Update(_float dt) override;

public:
    virtual virtual void Render_GUI()override;

public:
    virtual void Set_Throw(_fvector StartPos, _fvector throwDir);

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

public:
    void Dangle_Item(const string& boneName, _float3 offset);
protected:
   virtual void Throw_Item(_float dt);
   virtual void Find_Ground();
   virtual void MoveToIndex(_float dt);
   virtual void FollowHand(_float dt);
   virtual void Remove_Item();
   virtual void Update_ByState(_float dt);

public:
    ITEM_DATA_DESC Get_ItemData() { return m_ItemInfo; }

protected:
    _bool IsDangled = { false };
    _float m_MarginY = {};
    _float m_fBoundingTime = {};
    _float4x4* m_pOwnerMatrix = {};
    TILE_INDEX m_SyncedIndex = {};
    ITEM_STATE m_eState = { DROP };
    _float4 m_DstPosition = {};
    ITEM_DATA_DESC m_ItemInfo = {};
private:
    _float3 m_Offset = {};
public:    
    static CItem_Object* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END