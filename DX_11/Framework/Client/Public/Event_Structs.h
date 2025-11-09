#pragma once
namespace Client { class CPlayer; }

struct EventMsgDesc {
	_float2 OpenSize = {};
	_float OpenSpeed = {};
	vector<wstring> textSequence;
	function<void()> OnClose;
};

typedef struct tagEventPlayerPos {
	_float4 playerPos = {};
	Client::CPlayer* pPlayer = { nullptr };
} PLAYER_POS;

typedef struct tagEventTalking {
	class CGameObject* speaker = { nullptr };
	string Listner = {};
}TALKING_EVENT;

typedef struct tagEventTalkingMsg  {
	_float2 OpenSize = {};
	_float OpenSpeed = {};
	wstring Speaker = {};
	vector<wstring> textSequence;
	vector<function<void()>> SequenceActing;
	function<void()> OnClose;
}TalkingMsgDesc;