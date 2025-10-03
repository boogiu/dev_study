#pragma once
#include "ITileService.h"
NS_BEGIN(Engine)
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

    virtual _int Register_Tile(class CTileBlock* tileBlock, _uint x, _uint y, _uint z)override;
    virtual void UnRegister_Tile(_int Index)override;

    virtual TILESYSTEM_INFO* Get_TileSystemInfo()override { return &m_tTileInfo; }
    virtual void Get_IndexByPosition(_fvector vPos, _uint* x, _uint* y, _uint* z)override;
    virtual void Get_XYZByIndex(_uint Index, _uint* x, _uint* y, _uint* z)override;
    vector<CTileBlock*> Get_NeighborByIndex(_uint Index);
private:
    _uint Make_Index(_uint x, _uint y, _uint z);
private:
    vector<class CTileBlock*> m_Blocks;
    TILESYSTEM_INFO m_tTileInfo = {};

public:
    static CTileSystem* Create(const TILESYSTEM_INFO& tileInfo);
    virtual void Free() override;
};

NS_END