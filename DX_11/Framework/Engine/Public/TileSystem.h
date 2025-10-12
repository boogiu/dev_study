#pragma once
#include "ITileService.h"
NS_BEGIN(Engine)
using BlockLayer = vector<class CTileBlock*>;

class  CTileSystem :
    public ITileService
{
private:
    CTileSystem();
    virtual ~CTileSystem() DEFAULT;

public:
    struct Offset {int dx, dz; };

public:
    HRESULT Initialize(const TILESYSTEM_INFO& tileInfo);
    virtual TILE_INDEX Get_IndexByPosition(_float4 WorldPos) override;
    virtual TILE_INDEX Register_Tile(class CTileBlock* block, TILE_INDEX index)override;

private:
    _bool Check_ValidIndex(TILE_INDEX index);
private:
    TILESYSTEM_INFO m_tTileInfo = {};
    vector<BlockLayer> m_TileContainer;

    vector<TILE_INSTANCE> m_TileInstances;
public:
    static CTileSystem* Create(const TILESYSTEM_INFO& tileInfo);
    virtual void Free() override;
};

NS_END