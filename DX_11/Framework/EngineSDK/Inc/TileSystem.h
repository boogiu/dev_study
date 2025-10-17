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

#ifdef _DEBUG
public:
    HRESULT Render_Tiles(ID3D11DeviceContext* pContext, class CPipeLine* pPipeLine);
    void RegisterColorRule(const string& name, function<_float4(const TILE_INFO&)> func);
    void SetActiveColorRule(const string& name); 
    void Set_DebugRender(_bool bDebug) { m_bDebug = bDebug; };

private:
    HRESULT Ready_DebugBuffer();
    void Update_DebugColor(ID3D11DeviceContext* pContext);
    _float4 Get_TileDebugColor(const TILE_INDEX& index);
#endif // _DEBUG

private:
    TILESYSTEM_INFO m_tTileInfo = {};
    vector<BlockLayer> m_TileContainer;

#ifdef _DEBUG
    _uint IB_Count = {};
    ID3D11Buffer* m_pVB = { nullptr };
    ID3D11Buffer* m_pIB = { nullptr };
    ID3D11Buffer* m_TileIndicesBuffer = { nullptr };
    ID3D11ShaderResourceView* m_pTileIndexResource = { nullptr };

    ID3D11InputLayout* m_pLayout = { nullptr };
    class CShader* m_pDebugShader = { nullptr };
    unordered_map<string, function<_float4(const TILE_INFO&)>> m_DebugColorRules;
    string m_ActiveRuleName;
    _bool m_bDebug;
    vector<DEBUG_TILE> pDebugIndex;
#endif // _DEBUG

public:
    static CTileSystem* Create(const TILESYSTEM_INFO& tileInfo);
    virtual void Free() override;
};

NS_END