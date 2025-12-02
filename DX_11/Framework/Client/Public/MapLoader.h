#pragma once
#include "Base.h"

NS_BEGIN(Client)
static unordered_map<_uint, vector<string>>g_ModelMapTable;

class CMapLoader :
    public CBase
{
private:
    CMapLoader();
    ~CMapLoader() DEFAULT;

public:
    static HRESULT Load_MapData(string filePath, const LAYER_DESC& Desc);
    static HRESULT Load_ModelData();

    HRESULT Reserved_Load(string filePath, const LAYER_DESC& Desc);
    HRESULT Load_Sequential(const LAYER_DESC& Desc);
    _bool isComplete() { return reservedMapObj.empty() && reservedMapTile.empty(); }
private:
    queue< NEW_MAP_OBJECT_HEADER> reservedMapObj = {};
    queue< NEW_MAP_TILE_HEADER> reservedMapTile= {};

public:
    static CMapLoader* Create();
    virtual void Free() override;
};
NS_END

