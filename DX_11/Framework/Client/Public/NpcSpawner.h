#pragma once
#include "LevelObject.h"

NS_BEGIN(Client)

using  SEQUENCE = unordered_map<_int, vector<SEQUENCE_DATA_DESC>>;

class CNpcSpawner
	: public CLevelObject
{
	private:
	CNpcSpawner();
    CNpcSpawner(const CNpcSpawner& rhs);
    ~CNpcSpawner() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt)override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    HRESULT Read_CharacterData(const string& filePath);
    HRESULT Read_CharacterSequece(const string& filePath);

public:
    HRESULT Spawn_Npc(const wstring& npcName, _float3 position = {});
    vector<SEQUENCE_DATA_DESC> Get_SequenceData(_int npcID, _int sequenceID);
    NPC_DATA_DESC Get_NpcData(wstring npcName);

private:
    unordered_map<wstring, NPC_DATA_DESC> m_NpcTable;
    unordered_map<_int, SEQUENCE> m_NpcSequenceTable;

public:
	static CNpcSpawner* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	void Free() override;
};

NS_END