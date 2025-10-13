#pragma once
#include "GameObject.h"
NS_BEGIN(MapEditor)
class CTileObject :
    public CGameObject
{
private:
    CTileObject();
    CTileObject(const CTileObject& rhs);
    virtual ~CTileObject() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Object_OnGrid(TILE_INDEX index);
    HRESULT Load_Object(MAP_OBJECT_HEADER ObjHeader);

    HRESULT Link_Data(const string& folderName,_bool Base=false);

public:
    HRESULT Save_MapData(ofstream& ofs, _bool Base = false);

public:
    void Render_GUI() override;

private:
    _float2 m_PaletteIndex = {};
    _float2 m_TileScale = {};
    _int m_TileIndex = {};
    string ModelName = {};
    string MaterialName = {};

public:
    static CTileObject* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END