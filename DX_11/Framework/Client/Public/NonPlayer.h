#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CNonPlayer :
    public CGameObject
{
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
        _bool HasAgenda = { false };
        _bool playerEntrance = { false };
        AgendaType eAgendaType = {AgendaType::SmallTalk};
        NpcState eState = { NpcState::Idle};
    };

    struct NPC_TileInfoPacket {
        TILE_INDEX NowIndex = {};
        vector<TILE_INFO> infos;
        _uint neighboValidFlag = {};
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
    void Add_BaseAnimClip();
    void Add_Parts();
    void Add_EventListen();

public:
    void LookToPlayer(_float dt);
    void Open_Dialogue(const string tag, void* pArg);
public:
    NPC_MovementPacket& Get_MovementPack() { return m_MovementPack; }
    PlayerTracePacket& Get_TracePack() { return m_TracePack; }
    NPC_TileInfoPacket& Get_TilePack() { return m_TileInfoPack; }
    NPC_EventPacket& Get_EventPack() { return m_EventPack; }
    
protected:
    void Update_Movement(_float dt);
    void Update_TileInfo(_float dt);


protected:
    class CNpcState_Machine* m_pMachine = { nullptr };
    PlayerTracePacket m_TracePack = {};
    NPC_MovementPacket m_MovementPack = {};
    NPC_TileInfoPacket m_TileInfoPack = {};
    NPC_EventPacket m_EventPack = {};

    _float m_fDuration = {};
public:
    virtual void Free()override;
};
NS_END
