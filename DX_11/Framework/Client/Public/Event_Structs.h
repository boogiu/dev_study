#pragma once
namespace Client { class CPlayer; class CNonPlayer;class CItem_Object;}
namespace Engine { class CGameObject;}

enum class EVENT_TYPE {
	None,
	NoticeDialogue, //시작되었음
	DialougueStart, //시작할거임
	DialougueEnd, //끝났음
	Event_MsgUI,
	CameraMove,
	Talking_MsgUI,
	TransItem,
	TransItem_Response,
	Npc_To_Npc,
	Craft_Result,
	Craft_MsgUI,
	Quest_Msg,
	Quest_Msg_Responese,
	Player_Pos,
	FishBeyThrow,
	FishBeyReceive,
	FishBeyResult,
};

struct BaseEvent {
	EVENT_TYPE eType = { EVENT_TYPE::None };
};

struct OnStartDialogue : public BaseEvent {
	class CGameObject* pListner = {nullptr};
	class CGameObject* pSpeaker = { nullptr };
	_int SequenceID = {};
	string EntryCondition = {};
	wstring SpeakerName = {};
};

struct OnNoticeDialogue : public BaseEvent {
	class CGameObject* pSubject = { nullptr };
	class CGameObject* pCounter= { nullptr };
};

struct OnEndDialogue : public BaseEvent {
	class CGameObject* pSpeaker = { nullptr };
	_int NextSequenceID = {};
	string NextCondition = {};
	PostActionMsg msg = {};
};

struct EventMsgDesc : public BaseEvent {
	_float2 OpenSize = {};
	_float OpenSpeed = {};
	vector<wstring> textSequence;
	function<void()> OnClose;
};

typedef struct tagEventCameraMove : public BaseEvent {
	string moveTag;
} CAM_MOVE;

typedef struct tagEventPlayerPos : public BaseEvent {
	_float4 playerPos = {};
	Client::CPlayer* pPlayer = { nullptr };
} PLAYER_POS;

typedef struct tagEventUITalkingMsg : public BaseEvent {
	_float2 OpenSize = {};
	_float OpenSpeed = {};
	_int SpeakerID = {};
	_int startSequence = {};
	wstring SpeakerName = {};
	class CNonPlayer* pSpeaker = { nullptr };
	function<void(OnEndDialogue)> OnClose;
}TalkingMsgDesc;

typedef struct tagEventTransferItem : public BaseEvent {
	class CItem_Object* pObject = { nullptr }; 
	_uint pSenderID = {  }; 
}TRANS_ITEM;

typedef struct tagEventTransferItemResponse : public BaseEvent {
	class CItem_Object* pObject = { nullptr };
	_uint pSenderID = {  };
}RESPONSE_TRANS_ITEM;

typedef struct tagEventNpcToNpc : public BaseEvent {
	_uint Orderer_NPCID = {};
	_uint Server_NPCID = {};
	string OrderMsg = {};
}EVNET_NPC_TO_NPC;

typedef struct tagCraftResult : public BaseEvent
{
	string Result_ItemTag = {};
	ITEM_DATA_DESC ResourceItem_01 = {};
	_uint ResourceItemCount_01 = {};
	ITEM_DATA_DESC ResourceItem_02 = {};
	_uint ResourceItemCount_02 = {};
	ITEM_DATA_DESC ResourceItem_03 = {};
	_uint ResourceItemCount_03 = {};

	void Make_Result(CraftData data) {
		Result_ItemTag = data.Result_Item.FileName;
		ResourceItem_01 = data.ResourceItem_01;
		ResourceItemCount_01 = data.ResourceItemCount_01;
		ResourceItem_02 = data.ResourceItem_02;
		ResourceItemCount_02 = data.ResourceItemCount_02;
		ResourceItem_03 = data.ResourceItem_03;
		ResourceItemCount_03 = data.ResourceItemCount_03;
	}
} CRAFT_RESULT;

typedef struct tagCraftDataDesc : public BaseEvent
{
	unordered_map<wstring, _uint> InvenData;
	function<void(const CRAFT_RESULT& result)> OnClose;
} CRAFT_DATA_DESC;

typedef struct tagQuestEventMsg : public BaseEvent {
	CGameObject* pQuestPublisher = { nullptr };
	string eventMsg = {};
}QUEST_MSG;

typedef struct tagQuestEventResponse : public BaseEvent {
	CGameObject* pEventPublisher = { nullptr };
	_bool EvtConsumed = { false };
}QUEST_RESPONSE;


typedef struct tagFishBeyThrowEvent : public BaseEvent {
	_vector vDirection = {};
	_bool bActiveAction = {};
}POLE_THROW_EVENT;


typedef struct tagFishBeyRecieveEvent : public BaseEvent {
	_float4* pDestPos = { nullptr };
}POLE_BEY_RECIEVE;

typedef struct tagFishBeyBiteEvent : public BaseEvent {
	enum state {NONE, BITE, MISSED, CATCHED};
	state evtState = {};
	CGameObject* pCapturedFish = { nullptr };
}POLE_BITE_RESULT;