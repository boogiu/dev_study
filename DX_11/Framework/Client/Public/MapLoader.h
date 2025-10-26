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
};
NS_END

