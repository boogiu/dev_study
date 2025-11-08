#pragma once
#include "IService.h"
NS_BEGIN(Engine)
class ENGINE_DLL ITileService abstract :
public IService
{
protected:
	virtual  ~ITileService() DEFAULT;
public:
	virtual void Update(_float dt) PURE;
	virtual HRESULT Execute_InstanceModel(const string& levelKey, const string& modelKey, const string& materialKey) PURE;
	virtual HRESULT Set_Material_ID(TILE_INDEX tileIndex, _float4 materialID) PURE;
	virtual TILE_INDEX Get_IndexByPosition(_float4 WorldPos) PURE;
	virtual _float Get_TileHeightByPosition(_float4 WorldPos)PURE;
	virtual TILE_INDEX Register_Tile(class CTileBlock* block, TILE_INDEX index, _bool CanFail)PURE;
	virtual TILESYSTEM_INFO Get_TileSystemInfo() PURE;
	virtual _float4 Get_PositionByIndex(TILE_INDEX tileIndex, ANCHOR anchor) PURE;
	virtual _uint Get_NeighborInfoByIndex(TILE_INDEX index, vector<TILE_INFO>& container) PURE;
	virtual vector<class CTileBlock*> Get_NeighborByIndex(TILE_INDEX index) PURE;

	virtual vector<TILE_INDEX> Get_IndeciesByArea(_float4 vMin, _float4 vMax) PURE;
	virtual TILE_INFO Get_InfoByIndex(TILE_INDEX index) PURE;
	virtual INSTANCE_TILE Get_InstanceInfoByIndex(TILE_INDEX index) PURE;
	virtual HRESULT Save_TileSystemData(const string& SavePath) PURE;
	virtual HRESULT Executer_SystemByData(const string& LoadPath) PURE;
	virtual HRESULT Add_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag) PURE;
	virtual HRESULT Add_TileFlagByIndex(TILE_INDEX index, _uint flag) PURE;
	virtual HRESULT Remove_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag) PURE;
	virtual HRESULT Remove_TileFlagByIndex(TILE_INDEX index, _uint flag) PURE;
	virtual _uint Get_TileFlagByIndex(TILE_INDEX index) PURE;
	virtual _bool Check_TileFlagByPosition(_float4 WorldPos, _uint flag) PURE;
	virtual void Change_CornerHeight(TILE_INDEX index, _float leftTop, _float rightTop, _float rightBottom, _float leftBottom) PURE;
	virtual vector<TILE_INDEX> Request_Path_To(TILE_INDEX src, TILE_INDEX dst, _uint avoidFlag) PURE;

};
NS_END
