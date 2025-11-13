#pragma once
#include "LevelObject.h"
NS_BEGIN(Client)
using  SEQUENCE = unordered_map<_int, vector<SEQUENCE_DATA_DESC>>;

class CDialogueManager :
	public CLevelObject
{
private:
	CDialogueManager();
	CDialogueManager(const CDialogueManager& rhs);
	~CDialogueManager();

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(INIT_DESC* pArg) override;
	void Priority_Update(_float dt)override;
	void Update(_float dt)override;
	void Late_Update(_float dt)override;

public:
	void Set_FreindSystem(class CEventSystem* pSystem, class CUI_Responcer* pUIResponcer, class CNpcSpawner*pSpawner);
public:
	void onStart_Dialogue(OnStartDialogue startMsg);
	void onEnd_Dialogue(OnEndDialogue endMsg);

private:
	void Set_EventSystem();
public:
	vector<SEQUENCE_DATA_DESC> Get_SequenceData(_int npcID, _int sequenceID);
	HRESULT Read_CharacterSequece(const string& filePath);

private:
	unordered_map<_int, SEQUENCE> m_NpcSequenceTable;
	class CEventSystem* m_pSystem = { nullptr };
	class CUI_Responcer* m_pUIResponcer = {nullptr};
	class CNpcSpawner* m_pNpcSpawner = {nullptr};

	class CGameObject* m_pCurrentSpeaker = { nullptr };
public:
	static CDialogueManager* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	void Free() override;
};

NS_END