#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CPlayer :
    public CGameObject
{
public:
    typedef struct tagPlayerPartsDesc : GAMEOBJECT_DESC {
        CGameObject* pPlayer = { nullptr };
    }PLAYER_PARTS_DESC;

private:
    CPlayer();
    CPlayer(const CPlayer& rhs);
    virtual ~CPlayer() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    _float2 Get_InputAxis() { return m_vInputAxis; };
    _float Get_MoveSpeed() { return m_fMoveSpeed; };
    ITEM_TYPE Get_CurrentItemType();

public:
    void Change_Item(ITEM_DATA_DESC desc);
    ITEM_DATA_DESC Get_CurItemData() { return m_CurItem; };
    ITEM_DATA_DESC Get_DstItemData() { return m_DstItem; };
    void Set_CurItemData(ITEM_DATA_DESC desc);
    TILE_INDEX Get_FowardIndex();
    void ActiveCollider_Tool(_bool active, string Event = {});

private:
    void Add_AnimationClips();
    void Add_PartObjects();

private:
    class CPlayerStateMachine* m_pStateMachine= { nullptr };
    _float2 m_vInputAxis = {};
    _float  m_fMoveSpeed = {25.f};

    ITEM_DATA_DESC m_CurItem = {};
    ITEM_DATA_DESC m_DstItem = {};

public:
    static CPlayer* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
