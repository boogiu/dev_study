#pragma once
#include "ITileService.h"
NS_BEGIN(Engine)

struct TILE_INFO {
    _uint TileFlag = {}; //타일 타입 비트 플래그
    class CGameObject* pOverlapObject = { nullptr }; //실제 배치된 타일
    class CTileBlock* pTileBlock = { nullptr }; //실제 배치된 타일
};

using BlockLayer = vector<TILE_INFO>;

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
    virtual HRESULT Remove_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag) override;
    virtual _uint Get_TileFlagByIndex(TILE_INDEX index) override;
    virtual _bool Check_TileFlagByPosition(_float4 WorldPos, _uint flag) override;

#ifdef _DEBUG
public:
    HRESULT Render_Tiles(ID3D11DeviceContext* pContext);
#endif // _DEBUG

private:
    class CTileBlock* Get_TileBlockByIndex(TILE_INDEX index);
    _bool Check_ValidIndex(TILE_INDEX index);

#ifdef _DEBUG
private:
    HRESULT Ready_DebugBuffer();
#endif // _DEBUG

private:
    TILESYSTEM_INFO m_tTileInfo = {};
    vector<BlockLayer> m_TileContainer;

#ifdef _DEBUG
    _uint IB_Count = {};
    ID3D11Buffer* m_pVB = { nullptr };
    ID3D11Buffer* m_pIB = { nullptr };
    ID3D11InputLayout* m_pLayout = { nullptr };
    class CShader* m_pDebugShader = { nullptr };
#endif // _DEBUG

public:
    static CTileSystem* Create(const TILESYSTEM_INFO& tileInfo);
    virtual void Free() override;
};

NS_END