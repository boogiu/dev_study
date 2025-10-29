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

    struct MovementPacket {
        _bool bFliping = { false };
        _bool bMovable = { true };
        _bool bRunning = { true };
        _float fCurrentDegree= {};
        _float fTargetDegree = {};
        _float fPlayerHeight = {};
        _float  fMoveSpeed = { 25.f };
        _float2 vInputAxis = {};
    };

    struct TileInfoPacket{
        TILE_INDEX nowIndex = {};
        _uint neighboValidFlag = {};
        vector<TILE_INFO> infos;
    };

    struct ItemPacket {
        ITEM_DATA_DESC  CurItem = {};
        ITEM_DATA_DESC  DstItem = {};
    };

    struct InteractionPacket {
        _bool isUsingTool = { false };
    };

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
    void Update_Input(_float dt);
    void Update_TileInfo(_float dt);

public:
    MovementPacket& Get_MovementPacket() { return m_MovementPack; }
    TileInfoPacket& Get_TileInfoPacket() { return m_TileInfoPack; }
    ItemPacket& Get_ItemPacket() { return m_ItemPack; }
    InteractionPacket& Get_InteractionPacket() { return m_InteractionPack; }

    _bool Can_Walk(_float2 moveAxis);
    TILE_INDEX Get_FowardIndex();

public:
    void Change_Item(ITEM_DATA_DESC desc);
    void Set_CurItemData(ITEM_DATA_DESC desc);

public:
    void ActiveCollider_Tool(_bool active, string Event = {});
    void ActiveCollider_LeftHand(_bool active, string Event = {});

private:
    void Add_AnimationClips();
    void Add_PartObjects();

private:
    class CPlayerStateMachine* m_pStateMachine= { nullptr };
 

    MovementPacket m_MovementPack = {};
    TileInfoPacket m_TileInfoPack = {};
    ItemPacket m_ItemPack = {};
    InteractionPacket m_InteractionPack = {};

public:
    static CPlayer* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
