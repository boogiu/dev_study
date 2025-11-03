#pragma once
#include "LevelObject.h"
NS_BEGIN(Client)
class CItemSpawner :
    public CLevelObject
{
private:
    CItemSpawner();
    CItemSpawner(const CItemSpawner& rhs);
    ~CItemSpawner() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt)override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    HRESULT Read_ItemData(wstring filePath);
    ITEM_DATA_DESC Get_ItemData(string ItemTag);

private:
    unordered_map<string, ITEM_DATA_DESC> m_ItemDataTable;

public:
    static CItemSpawner* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END