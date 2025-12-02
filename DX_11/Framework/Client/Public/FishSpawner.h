#pragma once
#include "LevelObject.h"
NS_BEGIN(Client)
class CFishSpawner :
    public CLevelObject
{
private:
    CFishSpawner();
    CFishSpawner(const CFishSpawner& rhs);
    ~CFishSpawner() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt)override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    HRESULT Read_FishData(string filePath);
    void Notice_River(TILE_INDEX index);
    HRESULT Link_ItemSpawner(class CItemSpawner* pItemSpawner);
    _float3 CheckRandPositon();

public:
    FISH_DATA_DESC Get_Data(const string& insectName);
    ITEM_DATA_DESC Get_ItemData(const string& insectName);
private:
    void Spawn_Fish();

private:
    _float m_fElapsedTime = { 24.f };
    _float m_fSpawnTime = {34.f};
    class CItemSpawner* m_pItemSpawner = { nullptr }; 
    unordered_map<TILE_INDEX, _uint, TILE_INDEX_HASH_FUNCTOR> m_HashedTile;
    unordered_map<string, FISH_DATA_DESC> m_FishDataTable;
    unordered_map<_uint, string> m_NameTable;

    vector<TILE_INDEX> m_RiverTile;

public:
    static CFishSpawner* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END