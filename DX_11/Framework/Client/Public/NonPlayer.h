#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CNonPlayer :
    public CGameObject
{
public:
    typedef struct tagNPCSpawnDataDesc : public GAMEOBJECT_DESC {
        NPC_DATA_DESC characterDesc = {};
        CLOTH_DATA_DESC characterClothDesc = {};
    }NPC_SPAWN_DATA;

public:
    struct PlayerTracePacket {
        _bool Player_Near = { false };
        _float4 Player_Pos = { };
        _float4 vLook_Player = { };
        _float    Player_distance = { };
        _float traceBone_Radian = {};
        class CPlayer* pPlayer = {};
    }; //나중에 플레이어 트레이스 패킷으로 바꾸고 이벤트 분기는 따로 빼기

    struct NPC_MovementPacket {
        _float fCurrentDegree = {};
        _float fTargetDegree = {};
        _float fCharacterHeight = {};
        _float  fMoveSpeed = { 10.f };
        _float2 vMoveAxis = {};
        _float4 vDstPosition = {};
    };

    struct NPC_EventPacket {
        class CEventSystem* eventSystem= {nullptr};
        
        _float m_InteractCoolDown = {};

        _bool HasAgenda = { false };
        string AgendaType = { "SmallTalk" };
        _uint Ready_SequenceID = {};
        _bool Has_Event() { return HasAgenda && !AgendaType.empty(); }
    };

    struct NPC_TileInfoPacket {
        TILE_INDEX NowIndex = {};
        vector<TILE_INFO> infos;
        _uint neighboValidFlag = {};
    };

    struct NPC_ReservedPacket {
        string Socketbone = { "Armature_Hand_R" };
        CGameObject* pItem = { nullptr };
        POST_ACTION_DATA_DESC reservedAction = {};

        _bool Has_Reserved() { return reservedAction.Type.empty(); }

        void Reset() { pItem = nullptr; 
            reservedAction.Type = {};
            reservedAction.Param1 = {};
            reservedAction.Param2 = {};
            reservedAction.NextSequenceID = {-1};
        }
    };

protected:
    CNonPlayer();
    CNonPlayer(const CNonPlayer& rhs);
    virtual ~CNonPlayer() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override; 
    virtual void Awake() override;

public:
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

protected:
    HRESULT Make_Model(NPC_DATA_DESC desc);
    void Add_Parts();
    void Add_EventListen();

public:
    void LookToPlayer(_float dt);
    void LookTo(_fvector pos);
    void Open_Dialogue(const string tag, void* pArg);
    class CItem_Object* Spawn_Item(const string tag);

public:
    void Set_Animation(const string tag);
    void Set_Emotion(const string tag);
    void Set_Voice(const string tag);
    void Do_PostAction(POST_ACTION_DATA_DESC data);

public:
    NPC_MovementPacket& Get_MovementPack() { return m_MovementPack; }
    PlayerTracePacket& Get_TracePack() { return m_TracePack; }
    NPC_TileInfoPacket& Get_TilePack() { return m_TileInfoPack; }
    NPC_EventPacket& Get_EventPack() { return m_EventPack; }
    NPC_ReservedPacket& Get_ReservedPack() { return m_ReservedPack; }
    
public:
    NPC_DATA_DESC Get_NpcData() { return m_CharacterDesc; }

protected:
    void Update_Movement(_float dt);
    void Update_TileInfo(_float dt);


protected:
    class CNpcState_Machine* m_pMachine = { nullptr };
    NPC_DATA_DESC m_CharacterDesc = {};
    PlayerTracePacket m_TracePack = {};
    NPC_MovementPacket m_MovementPack = {};
    NPC_TileInfoPacket m_TileInfoPack = {};
    NPC_EventPacket m_EventPack = {};
    NPC_ReservedPacket m_ReservedPack = {};

    _float4 prevPos = {};
public:
    static CNonPlayer* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free()override;
};
NS_END
