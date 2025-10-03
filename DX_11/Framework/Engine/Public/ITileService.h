#pragma once
#include "IService.h"
NS_BEGIN(Engine)
class ENGINE_DLL ITileService abstract :
public IService
{
protected:
	virtual  ~ITileService() DEFAULT;
public:
    virtual _int Register_Tile(class CTileBlock* tileBlock, _uint x, _uint y, _uint z)PURE;
    virtual void UnRegister_Tile(_int Index)PURE;
    virtual TILESYSTEM_INFO* Get_TileSystemInfo() PURE;
    virtual void Get_IndexByPosition(_fvector vPos, _uint* x, _uint* y, _uint* z)   PURE;
    virtual void Get_XYZByIndex(_uint Index, _uint* x, _uint* y, _uint* z)   PURE;
};
NS_END
