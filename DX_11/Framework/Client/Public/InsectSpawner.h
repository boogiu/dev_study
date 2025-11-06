#pragma once
#include "LevelObject.h"
NS_BEGIN(Client)
class CInsectSpawner
	: public CLevelObject
{
private:
	CInsectSpawner();
	CInsectSpawner(const CInsectSpawner& rhs);
	~CInsectSpawner();

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(INIT_DESC* pArg) override;
	void Priority_Update(_float dt)override;
	void Update(_float dt)override;
	void Late_Update(_float dt)override;

public:
	HRESULT Link_ItemSpawner(class CItemSpawner* pItemSpawner);
	HRESULT Read_InsectData(wstring filePath);
	HRESULT Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; return S_OK; };
public:
	INSECT_DATA_DESC Get_Data(const string& insectName);
	ITEM_DATA_DESC Get_ItemData(const string& insectName);
	class CInsect_Object* SpawnInsect(string insectName, _float4 pos);

private:
	void Update_Spawner(_float dt);
	_float3 CheckRandPositon();

private:
	class CItemSpawner* m_pItemSpawner = { nullptr };
	CGameObject* m_pTarget = { nullptr };
	_float m_fSpawnTime = { 24.f };
	_float m_fElapsedTime = { 0.f };
	unordered_map<string, INSECT_DATA_DESC> m_InsectDataTable;
	unordered_map<_uint, string> m_NameTable;
public:
	static CInsectSpawner* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	void Free() override;
};

NS_END