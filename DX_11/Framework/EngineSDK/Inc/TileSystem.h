#pragma once
#include "ITileService.h"
NS_BEGIN(Engine)

using BlockLayer = vector<TILE_INFO>;

struct DEBUG_TILE {
    TILE_INDEX index;
    _float4 Color = {1.f,1.f,1.f,1.f};
};

class  CTileSystem :
    public ITileService
{
private:
    CTileSystem();
    virtual ~CTileSystem() DEFAULT;

public:
    HRESULT Initialize(const TILESYSTEM_INFO& tileInfo);
    virtual TILE_INDEX Get_IndexByPosition(_float4 WorldPos) override;
    virtual _float4 Get_PositionByIndex(TILE_INDEX tileIndex, ANCHOR anchor ) override;
    virtual TILE_INDEX Register_Tile(class CTileBlock* block, TILE_INDEX index, _bool CanFail)override;
    virtual vector<class CTileBlock*> Get_NeighborByIndex(TILE_INDEX index) override;

    virtual vector<TILE_INDEX> Get_IndeciesByArea(_float4 vMin, _float4 vMax) override;
    virtual HRESULT Add_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag) override;
    virtual HRESULT Add_TileFlagByIndex(TILE_INDEX index, _uint flag) override;

    virtual HRESULT Remove_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag) override;
    virtual HRESULT Remove_TileFlagByIndex(TILE_INDEX index, _uint flag) override;
    virtual _uint Get_TileFlagByIndex(TILE_INDEX index) override;
    virtual _bool Check_TileFlagByPosition(_float4 WorldPos, _uint flag) override;

private:
    class CTileBlock* Get_TileBlockByIndex(TILE_INDEX index);
    _bool Check_ValidIndex(TILE_INDEX index);
    TILE_INFO Find_Info(TILE_INDEX index);

private:
    TILESYSTEM_INFO m_tTileInfo = {};
    vector<BlockLayer> m_TileContainer;

    unordered_map<class CGameObject*, vector<INSTANCE_TILE>> m_AutoTileSystem;
    vector<TILE_INDEX> m_DirtyTile;
public:
    static CTileSystem* Create(const TILESYSTEM_INFO& tileInfo);
    virtual void Free() override;
};


/*
타일 인포에서 자신에게 해당되는 인스턴스 담당 오브젝트를 들고 있음.
해당 인덱스의 타입이 바뀌면 시스템이 담당 인스턴스로 바뀌어주는 것임.
*/
NS_END