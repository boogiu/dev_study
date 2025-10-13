#pragma once
#include "ITileService.h"
NS_BEGIN(Engine)
using BlockLayer = vector<class CTileBlock*>;

class  CTileSystem :
    public ITileService
{
 public:
     enum NEIGHBOR_INDEX { UPLEFT, UP, UPRIGHT, LEFT, CENTER, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT,END };
private:
    CTileSystem();
    virtual ~CTileSystem() DEFAULT;

public:
    HRESULT Initialize(const TILESYSTEM_INFO& tileInfo);
    virtual TILE_INDEX Get_IndexByPosition(_float4 WorldPos) override;
    virtual _float4 Get_PositionByIndex(TILE_INDEX tileIndex, ANCHOR anchor ) override;
    virtual TILE_INDEX Register_Tile(class CTileBlock* block, TILE_INDEX index)override;
    virtual vector<class CTileBlock*> Get_NeighborByIndex(TILE_INDEX index) override;

private:
    class CTileBlock* Get_TileBlockByIndex(TILE_INDEX index);
private:
    _bool Check_ValidIndex(TILE_INDEX index);

private:
    TILESYSTEM_INFO m_tTileInfo = {};
    vector<BlockLayer> m_TileContainer;

public:
    static CTileSystem* Create(const TILESYSTEM_INFO& tileInfo);
    virtual void Free() override;
};

NS_END