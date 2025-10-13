#pragma once
#include "IService.h"
NS_BEGIN(Engine)
class ENGINE_DLL ITileService abstract :
public IService
{
protected:
	virtual  ~ITileService() DEFAULT;
public:
	virtual TILE_INDEX Get_IndexByPosition(_float4 WorldPos) PURE;
	virtual TILE_INDEX Register_Tile(class CTileBlock* block, TILE_INDEX index)PURE;
	virtual _float4 Get_PositionByIndex(TILE_INDEX tileIndex, ANCHOR anchor) PURE;
	virtual vector<class CTileBlock*> Get_NeighborByIndex(TILE_INDEX index) PURE;
};
NS_END
