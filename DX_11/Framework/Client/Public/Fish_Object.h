#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CFish_Object :
    public CGameObject
{
    enum MoveState { IDLE, FIND_ENTRANCE,FIND_BACK, DISAPPEAR ,HIT,BITE,CATCHED, GETTED,REMOVED };
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
    FISH_DATA_DESC Get_Data() { return m_FishDesc; }
    virtual void Set_TargetBey(CGameObject* pTarget);
    _bool Hit();
    void Missed();
    void Catch();
    void Get();

public:
public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

private:
    void Move_IDLE(_float dt);
    void Move_FIND(_float dt);
    void Move_BACK(_float dt);
    void Move_HIT(_float dt);
    void Move_BITE(_float dt);
    void Move_CATCHED(_float dt);
    void Move_DISAPPEAR(_float dt);
    void Check_Axis();
    void Check_Rotation(_float dt);

private:
    MoveState m_eState = { IDLE };

    _float m_fAlpha = {1.f};
    _float m_fElapseTime = {};

    _float m_fCurrentRadian = {};
    _float m_fDestRadian = {};

    _float2 m_vAxis = {};

    _float m_fDetectTime = {};
    _float m_fMoveSpeed = {2.f};
    _float4 m_vTargetDir = {};

    _uint m_BiteCount = {  };
    _uint m_NowBiteCount = {  };
    _float m_fBiteTime = {};
    _float m_fEffectTime = {};

    class CFishSub_Tool* m_pTarget = { nullptr };
    _float4x4* m_pOwnerMatrix = { nullptr };

    FISH_DATA_DESC m_FishDesc = {};
    ITEM_DATA_DESC m_ItemDesc = {};

public:
    static CFish_Object* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END