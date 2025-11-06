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
    class CItem_Object* SpawnItem(string ItemTag , _float3 pos);
    class CItem_Object* SpawnItem(string ItemTag );
    class CItem_Object* ThrowItem(string ItemTag, _fvector pos, _cvector MoveDir);
    class CItem_Object* ThrowItem(ITEM_DATA_DESC data, _fvector pos, _cvector MoveDir);

private:
    unordered_map<string, ITEM_DATA_DESC> m_ItemDataTable;

public:
    static CItemSpawner* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END