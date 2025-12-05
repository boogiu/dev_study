#pragma once
#include "LevelObject.h"

NS_BEGIN(Client)


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

public:
    class CGameObject* Spawn_Npc(const wstring& npcName, _float3 position = {},string SpcTag = {});
    NPC_DATA_DESC Get_NpcData(wstring npcName);

private:
    unordered_map<wstring, NPC_DATA_DESC> m_NpcTable;

public:
	static CNpcSpawner* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	void Free() override;
};

NS_END