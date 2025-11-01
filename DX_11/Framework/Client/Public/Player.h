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
        _float fCurrentDegree= {};
        _float fTargetDegree = {};
        _float fPlayerHeight = {};
        _float  fMoveSpeed = { 25.f };
        _float2 vInputAxis = {};
    };

    struct TileInfoPacket{
        TILE_INDEX nowIndex = {};
        TILE_INFO Range_FowardInfo = {};
        _uint neighboValidFlag = {};
        vector<TILE_INFO> infos;
    };

    struct ItemPacket {
        TOOL_DATA_DESC  CurItem = {};
        TOOL_DATA_DESC  DstItem = {};
    };

    struct ControlPacket {
        _bool MsgMove = false; //방향키
        _bool MsgAction = false; //도구 사용키 스페이스
        _bool MsgPickup = false;
        _bool MsgBag = false;
        _bool MsgAdd = false; //시프트 키
        void Reset() {
            MsgMove = MsgAdd = MsgAction = MsgPickup = MsgBag = false;
        }
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
    void Update_Movement(_float dt);
    void Update_TileInfo(_float dt);
    void Adjust_To_Foward();

public:
    MovementPacket& Get_MovementPacket() { return m_MovementPack; }
    TileInfoPacket& Get_TileInfoPacket() { return m_TileInfoPack; }
    ItemPacket& Get_ItemPacket() { return m_ItemPack; }
    ControlPacket& Get_ControlPack() { return m_ControlPack; }

    _bool Can_Walk(_float2& moveAxis);
    TILE_INDEX Get_FowardIndex();

public:
    void Change_Item(TOOL_DATA_DESC desc);
    void Set_CurItemData(TOOL_DATA_DESC desc);

public:
    void ActiveCollider_Tool(_bool active, string Event = {});
    void ActiveCollider_LeftHand(_bool active, string Event = {});
    void ActiveCollider_RightHand(_bool active, string Event = {});

private:
    void Add_AnimationClips();
    void Add_PartObjects();

private:
    class CPlayerStateMachine* m_pStateMachine= { nullptr };
 
    _float m_fRange = {8};
    MovementPacket m_MovementPack = {};
    TileInfoPacket m_TileInfoPack = {};
    ItemPacket m_ItemPack = {};
    ControlPacket m_ControlPack = {};

public:
    static CPlayer* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
