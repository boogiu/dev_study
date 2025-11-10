#pragma once
namespace Client { class CPlayer; class CNonPlayer;}

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
	class CGameObject* pSpeaker = { nullptr };
	class CGameObject* pListner = { nullptr };
	string Listner = {};
}TALKING_EVENT;

typedef struct tagEventTalkingMsg  {
	_float2 OpenSize = {};
	_float OpenSpeed = {};

	_int SpeakerID = {};
	_int startSequence = {};
	CNonPlayer* Speaker = { nullptr };
	function<void(_bool)> OnClose;
}TalkingMsgDesc;

///	typedef struct tagSequenceControlMsg  {
///		_float2 OpenSize = {};
///		_float OpenSpeed = {};
///	
///		wstring Speaker = {};
///	
///		vector<wstring> textSequence;
///		vector<function<void()>> SequenceActing;
///		vector<_float>	SequenceTime;
///		function<void()> OnClose;
///	}SequenceMsgDesc;
