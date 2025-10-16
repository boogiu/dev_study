#pragma once
#include "Base.h"

NS_BEGIN(Client)
class CMapLoader :
    public CBase
{
private:
    CMapLoader();
    ~CMapLoader() DEFAULT;

    /*맵 로더는 로드하는 것만 하나?*/
    /*익스큐트 타일 시스템까지 같이 해버릴까?*/

public:
    static HRESULT Load_MapData(string filePath, const LAYER_DESC& Desc);
};
NS_END

