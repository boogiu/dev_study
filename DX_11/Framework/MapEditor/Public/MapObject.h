#pragma once
#include "GameObject.h"
NS_BEGIN(MapEditor)
class CMapObject :
    public CGameObject
{
private:
    CMapObject();
    CMapObject(const CMapObject& rhs);
    virtual ~CMapObject() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Object_OnGrid(TILE_INDEX index);
    HRESULT Link_Data(const string& folderName);
    HRESULT Load_Object(MAP_OBJECT_HEADER ObjHeader);

public:
    HRESULT Save_MapData(ofstream& ofs);
public:
    void Render_GUI() override;

private:
    _float2 m_PaletteIndex = {};
    _float2 m_TileScale = {};
    TILE_INDEX m_SyncedIndex = {};
    string ModelName = {};
    string MaterialName = {};
    _bool OnGrid = { false };

public:
    static CMapObject* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END