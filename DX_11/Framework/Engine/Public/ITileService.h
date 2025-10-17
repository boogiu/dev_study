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
	virtual TILE_INDEX Register_Tile(class CTileBlock* block, TILE_INDEX index, _bool CanFail)PURE;
	virtual _float4 Get_PositionByIndex(TILE_INDEX tileIndex, ANCHOR anchor) PURE;
	virtual vector<class CTileBlock*> Get_NeighborByIndex(TILE_INDEX index) PURE;
	virtual vector<TILE_INDEX> Get_IndeciesByArea(_float4 vMin, _float4 vMax) PURE;


	virtual HRESULT Add_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag) PURE;
	virtual HRESULT Add_TileFlagByIndex(TILE_INDEX index, _uint flag) PURE;
	virtual HRESULT Remove_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag) PURE;
	virtual HRESULT Remove_TileFlagByIndex(TILE_INDEX index, _uint flag) PURE;
	virtual _uint Get_TileFlagByIndex(TILE_INDEX index) PURE;
	virtual _bool Check_TileFlagByPosition(_float4 WorldPos, _uint flag) PURE;

#ifdef _DEBUG
	virtual HRESULT Render_Tiles(ID3D11DeviceContext* pContext, class CPipeLine* pPipeLine) PURE;    
	virtual void RegisterColorRule(const string& name, function<_float4(const TILE_INFO&)> func)PURE;
	virtual void   Set_DebugRender(_bool bDebug) PURE;

	virtual void SetActiveColorRule(const string& name)PURE;
#endif // _DEBUG
};
NS_END
