#pragma once
namespace Client { class CPlayer; class CNonPlayer;class CItem_Object;}
namespace Engine { class CGameObject;}

struct OnStartDialogue {
	class CGameObject* pListner = {nullptr};
	class CGameObject* pSpeaker = { nullptr };
	_int SequenceID = {};
	string EntryCondition = {};
	wstring SpeakerName = {};
};

struct OnNoticeDialogue {
	class CGameObject* pSubject = { nullptr };
	class CGameObject* pCounter= { nullptr };
};

struct OnEndDialogue {
	class CGameObject* pSpeaker = { nullptr };
	_int NextSequenceID = {};
	string NextCondition = {};
	PostActionMsg msg = {};
};

struct EventMsgDesc {
	_float2 OpenSize = {};
	_float OpenSpeed = {};
	vector<wstring> textSequence;
	function<void()> OnClose;
};

typedef struct tagEventCameraMove {
	string moveTag;
} CAM_MOVE;

typedef struct tagEventPlayerPos {
	_float4 playerPos = {};
	Client::CPlayer* pPlayer = { nullptr };
} PLAYER_POS;

typedef struct tagEventTalkingMsg  {
	_float2 OpenSize = {};
	_float OpenSpeed = {};
	_int SpeakerID = {};
	_int startSequence = {};
	wstring SpeakerName = {};
	class CNonPlayer* pSpeaker = { nullptr };
	function<void(OnEndDialogue)> OnClose;
}TalkingMsgDesc;

typedef struct tagEventTransferItem {
	class CItem_Object* pObject = { nullptr }; 
	_uint pSenderID = {  }; 
}TRANS_ITEM;

typedef struct tagEventTransferItemResponse {
	class CItem_Object* pObject = { nullptr };
	_uint pSenderID = {  };
}RESPONSE_TRANS_ITEM;

typedef struct tagEventNpcToNpc {
	_uint Orderer_NPCID = {};
	_uint Server_NPCID = {};
	string OrderMsg = {};
}EVNET_NPC_TO_NPC;



typedef struct tagCraftResult
{

} CRAFT_RESULT;

typedef struct tagCraftDataDesc
{
	unordered_map<wstring, _uint> InvenData;
	function<void(const CRAFT_RESULT& result)> OnClose;
} CRAFT_DATA_DESC;
