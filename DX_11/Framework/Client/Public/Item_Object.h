#pragma once
#include "FieldObject.h"
NS_BEGIN(Client)
class CItem_Object :
    public CGameObject
{
protected :
    TILE_FLAG CantDrop = TILE_FLAG::FLAG_ONITEM | TILE_FLAG::FLAG_STONE | TILE_FLAG::FLAG_TREE | TILE_FLAG::FLAG_BLOCKED;
    enum ITEM_STATE { IDLE, DROP, BOUND, FINDED, PICKED, READY_DESTROY };

public:
    struct DROP_ITEM_DESC : GAMEOBJECT_DESC{
        ITEM_DATA_DESC itemDesc = {};
    };

protected:
    CItem_Object();
    CItem_Object(const CItem_Object& rhs);
    virtual ~CItem_Object() DEFAULT;

public:
   virtual  HRESULT Initialize_Prototype() override;
   virtual  HRESULT Initialize(INIT_DESC* pArg) override;
   virtual  void Priority_Update(_float dt) override;
   virtual  void Update(_float dt) override;
   virtual  void Late_Update(_float dt) override;
   virtual  virtual void Render_GUI()override;

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

protected:
   virtual void Find_Ground();
   virtual void MoveToIndex(_float dt);
   virtual void FollowHand(_float dt);
   virtual void Remove_Item();

protected:
    _float m_MarginY = {};
    _float m_fBoundingTime = {};
    TILE_INDEX m_SyncedIndex = {};
    _float4 m_DstPosition = {};
    ITEM_STATE m_eState = { DROP };

    _float4x4* m_pOwnerMatrix = {};
public:    void Free() override;
};
NS_END