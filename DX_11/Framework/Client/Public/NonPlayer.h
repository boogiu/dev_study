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
	};

	/*미래 진행에 관련*/
	struct NPC_EventPacket {
		/*이벤트 시스템*/
		class CEventSystem* eventSystem = { nullptr };
		/*인터렉션 쿨다운*/
		_float m_InteractCoolDown = {};
		PostActionMsg reservedMsg = {};
		_int nextSequenceID = 0;

		_bool ActionDone = { false };
		_bool SequenceUsed = { false };

		string externalCondition = { "NormalTalking" };

		_bool isReservedAction() const { return !reservedMsg.Type.empty(); }
		_bool hasNextSequence() const { return nextSequenceID != -1; }

		_uint ConsumeSequence()
		{
			if (m_InteractCoolDown < 0.2f)
				return -1;
			SequenceUsed = true;
			_int seq = nextSequenceID;
			nextSequenceID = -1;
			return seq;
		}

		PostActionMsg ConsumePostAction()
		{
			if (m_InteractCoolDown < 0.2f)
				return PostActionMsg();
			ActionDone = true;
			PostActionMsg msg = reservedMsg;
			reservedMsg = {}; // 리셋
			return msg;
		}

		void Reset()
		{
			m_InteractCoolDown = 0.f;
			reservedMsg = {};
			ActionDone = false;
			SequenceUsed = false;
		}
	};

	struct NPC_TileInfoPacket {
		TILE_INDEX NowIndex = {};
		vector<TILE_INFO> infos;
		_uint neighboValidFlag = {};
	};

	/*현재 진행에 관련*/
	struct NPC_ActionStatePacket {
		string nowAction = {};
		ActionPhase phase = { ActionPhase::None };

		_bool Is(const string& name, ActionPhase p) const {
			return nowAction == name && phase == p;
		}

		void Begin(const string& name) {
			nowAction = name;
			phase = ActionPhase::Start;
		}

		void NextPhase() {
			if (phase == ActionPhase::Start) phase = ActionPhase::Proceed;
			else if (phase == ActionPhase::Proceed) phase = ActionPhase::End;
			else if (phase == ActionPhase::End) phase = ActionPhase::None;
		}

		void Reset() {
			nowAction.clear();
			phase = ActionPhase::None;
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
	void Add_MatAnimator();

public:
	void LookToPlayer(_float dt);
	void LookTo(_fvector pos);
	class CItem_Object* Spawn_Item(const string tag, _float3 pos = {});

public:
	virtual void Set_Animation(const string tag);
	virtual void Set_Emotion(const string tag);
	virtual void Set_Voice(const string tag);
	virtual void Set_Camera(const string tag);
	virtual void Set_Closed(OnEndDialogue endMsg);

public:
	virtual void Serve_Order(const string& order, _uint orderer);

public:
	_float4x4 Get_SocketMatrix(string socketName);

public:
	NPC_MovementPacket& Get_MovementPack() { return m_MovementPack; }
	PlayerTracePacket& Get_TracePack() { return m_TracePack; }
	NPC_TileInfoPacket& Get_TilePack() { return m_TileInfoPack; }
	NPC_EventPacket& Get_EventPack() { return m_EventPack; }
	NPC_ActionStatePacket& Get_ActionPack() { return m_ActionPack; }

public:
	NPC_DATA_DESC Get_NpcData() { return m_CharacterDesc; }

protected:
	void Update_Movement(_float dt);
	void Update_TileInfo(_float dt);
	/* string Socketbone = { "Armature_Hand_R" };*/
protected:
	class CNpcState_Machine* m_pMachine = { nullptr };
	NPC_DATA_DESC m_CharacterDesc = {};

	PlayerTracePacket m_TracePack = {};
	NPC_MovementPacket m_MovementPack = {};
	NPC_TileInfoPacket m_TileInfoPack = {};
	NPC_EventPacket m_EventPack = {};
	NPC_ActionStatePacket m_ActionPack = {};

	TILE_INDEX prevIndex = {};
public:
	static CNonPlayer* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	virtual void Free()override;
};
NS_END
