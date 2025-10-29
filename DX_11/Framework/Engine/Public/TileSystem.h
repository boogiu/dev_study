#pragma once
#include "ITileService.h"
NS_BEGIN(Engine)

using BlockLayer = vector<TILE_INFO>;

class  CTileSystem :
    public ITileService
{
private:
    CTileSystem();
    virtual ~CTileSystem() DEFAULT;

public:
    HRESULT Initialize(const TILESYSTEM_INFO& tileInfo);
    virtual void Update(_float dt) override;

public:
    virtual HRESULT Execute_InstanceModel(const string& levelKey, const string& modelKey, const string& materialKey) override;
    virtual HRESULT Set_Material_ID(TILE_INDEX tileIndex, _float4 materialID) override;

public:
    virtual TILESYSTEM_INFO Get_TileSystemInfo() override { return m_tTileInfo; };
    virtual _float Get_TileHeightByPosition(_float4 WorldPos)override;
    virtual TILE_INDEX Get_IndexByPosition(_float4 WorldPos) override;
    virtual _float4 Get_PositionByIndex(TILE_INDEX tileIndex, ANCHOR anchor ) override;
    virtual TILE_INDEX Register_Tile(class CTileBlock* block, TILE_INDEX index, _bool CanFail)override;
    virtual _uint Get_NeighborInfoByIndex(TILE_INDEX index, vector<TILE_INFO>& container) override;
    virtual vector<class CTileBlock*> Get_NeighborByIndex(TILE_INDEX index) override;

    virtual vector<TILE_INDEX> Get_IndeciesByArea(_float4 vMin, _float4 vMax) override;
    virtual HRESULT Add_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag) override;
    virtual HRESULT Add_TileFlagByIndex(TILE_INDEX index, _uint flag) override;

    virtual HRESULT Remove_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag) override;
    virtual HRESULT Remove_TileFlagByIndex(TILE_INDEX index, _uint flag) override;
    virtual _uint Get_TileFlagByIndex(TILE_INDEX index) override;
    virtual _bool Check_TileFlagByPosition(_float4 WorldPos, _uint flag) override;
    virtual TILE_INFO Get_InfoByIndex(TILE_INDEX index) override;
    virtual INSTANCE_TILE Get_InstanceInfoByIndex(TILE_INDEX index) override;
    virtual void Change_CornerHeight(TILE_INDEX index,_float leftTop, _float rightTop, _float rightBottom, _float leftBottom) override;

public:
    virtual HRESULT Save_TileSystemData(const string& SavePath) override;
    virtual HRESULT Executer_SystemByData(const string& LoadPath) override;

private:
    class CTileBlock* Get_TileBlockByIndex(TILE_INDEX index);
    _bool Get_TileInfoByIndex(TILE_INDEX index, TILE_INFO& info);
    _bool Check_ValidIndex(TILE_INDEX index);
    TILE_INFO Find_Info(TILE_INDEX index);

private:
    TILESYSTEM_INFO m_tTileInfo = {};
 
    /*For Instance*/
    vector<TILE_INDEX> m_DirtyTile;

    vector<INSTANCE_TILE> m_InstanceTiles;
    vector<TILE_INFO> m_TileInfos;
    vector<INSTANCE_INIT_DESC> m_instanceDesc;
    class CInstanceModel* m_pInstanceModel = { nullptr };
    class CMaterial* m_pTileMaterial = { nullptr };
    _float4x4 m_pTileWorldMatrix = {  };
    ID3D11DeviceContext* m_pContext = { nullptr };

public:
    static CTileSystem* Create(const TILESYSTEM_INFO& tileInfo);
    static CTileSystem* CreateByData(const string& LoadPath);
    virtual void Free() override;
};


/*
타일 인포에서 자신에게 해당되는 인스턴스 담당 오브젝트를 들고 있음.
해당 인덱스의 타입이 바뀌면 시스템이 담당 인스턴스로 바뀌어주는 것임.
*/
NS_END