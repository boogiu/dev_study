#pragma once
#include "GameObject.h"
NS_BEGIN(MapEditor)


constexpr _uint TileExist = 1 << 0  ;
class CMapTileInstance :
    public CGameObject
{
private:
    CMapTileInstance();
    CMapTileInstance(const CMapTileInstance& rhs);
    virtual ~CMapTileInstance() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Add_Tile(_float4x4 matrix, _float4 materialType);
    void Add_Tile(_float4 position, _float4 materialType);
public:
    void Load_Tile(INSTANCE_TILE tile);

private:
    HRESULT Clear_Tile( );

public:
    void Render_GUI() override;
public:
    HRESULT Save_Tiles(ofstream& ofs);
    _uint Get_TileCount() { return m_Tiles.size(); }

private:
    vector<INSTANCE_TILE> m_Tiles;
public:
    static CMapTileInstance* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END