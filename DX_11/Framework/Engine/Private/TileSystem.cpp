#include "TileSystem.h"
#include "TileBlock.h"

CTileSystem::CTileSystem()
{
}


HRESULT CTileSystem::Initialize(const TILESYSTEM_INFO& tileInfo)
{
	m_tTileInfo = tileInfo;

	m_TileContainer.resize(m_tTileInfo.iTileCountY);

	for (BlockLayer& layer : m_TileContainer) {
		layer.resize(m_tTileInfo.iTileCountZ * m_tTileInfo.iTileCountZ, nullptr);
	}

	return S_OK;
}

TILE_INDEX CTileSystem::Get_IndexByPosition(_float4 WorldPos)
{
	_float DistanceX = WorldPos.x - m_tTileInfo.vWorldMin.x;
	_float DistanceY = WorldPos.y - m_tTileInfo.vWorldMin.y;
	_float DistanceZ = WorldPos.z - m_tTileInfo.vWorldMin.z;

	_int    TileIndexX = static_cast<_int>(floorf(DistanceX / m_tTileInfo.SizePerTile().x));
	_int    TileIndexY = static_cast<_int>(floorf(DistanceY / m_tTileInfo.SizePerTile().y));
	_int    TileIndexZ = static_cast<_int>(floorf(DistanceZ / m_tTileInfo.SizePerTile().z));

	TILE_INDEX index = {};

	if (TileIndexX < 0 || TileIndexX >= static_cast<_int>(m_tTileInfo.iTileCountX))
		index.IndexX = -1;
	else
		index.IndexX = TileIndexX;

	if (TileIndexY < 0 || TileIndexY >= static_cast<_int>(m_tTileInfo.iTileCountY))
		index.IndexY = -1;
	else
		index.IndexY = TileIndexY;

	if (TileIndexZ < 0 || TileIndexZ >= static_cast<_int>(m_tTileInfo.iTileCountZ))
		index.IndexZ = -1;
	else
		index.IndexZ = TileIndexZ;

	return index;
}

_float4 CTileSystem::Get_PositionByIndex(TILE_INDEX tileIndex, ANCHOR anchor)
{
	_float3 vOrigin = m_tTileInfo.vWorldMin;
	_float3 result = {};

	_uint anchorFlags = static_cast<_uint>(anchor);

	if (anchorFlags & static_cast<_uint>(ANCHOR::Left))
		result.x = tileIndex.IndexX * m_tTileInfo.SizePerTile().x;
	else if (anchorFlags & static_cast<_uint>(ANCHOR::Right))
		result.x = (tileIndex.IndexX + 1) * m_tTileInfo.SizePerTile().x;
	else
		result.x = (tileIndex.IndexX * m_tTileInfo.SizePerTile().x) + (m_tTileInfo.SizePerTile().x * 0.5f);

	if (anchorFlags & static_cast<_uint>(ANCHOR::Top))
		result.z = (tileIndex.IndexZ + 1) * m_tTileInfo.SizePerTile().z;
	else if (anchorFlags & static_cast<_uint>(ANCHOR::Bottom))
		result.z = tileIndex.IndexZ * m_tTileInfo.SizePerTile().z;
	else
		result.z = (tileIndex.IndexZ * m_tTileInfo.SizePerTile().z) + (m_tTileInfo.SizePerTile().z * 0.5f);


	result.y = (tileIndex.IndexY * m_tTileInfo.SizePerTile().y);

	return _float4{ result.x, result.y, result.z, 1.f };
}

TILE_INDEX CTileSystem::Register_Tile(CTileBlock* block, TILE_INDEX index)
{
	if (!Check_ValidIndex(index))
		return TILE_INDEX();

	BlockLayer& TileLayer = m_TileContainer[index.IndexY];
	TILE_INDEX desireIndex = index;
	while (true) {
		_int XZIndex = desireIndex.IndexX + desireIndex.IndexZ * m_tTileInfo.iTileCountX;

		if (nullptr == TileLayer[XZIndex]) {
			TileLayer[XZIndex] = block;

			block->Set_Index(desireIndex);
			block->Update_Position(m_tTileInfo);
			return desireIndex;
		}

		if (desireIndex.IndexZ >= static_cast<_int>(m_tTileInfo.iTileCountZ))
		{
			return TILE_INDEX{};
		}
		else {
			desireIndex.IndexX += 1;
			if (desireIndex.IndexX >= static_cast<_int>(m_tTileInfo.iTileCountZ)) {
				desireIndex.IndexX = 0;
				desireIndex.IndexZ += 1;
			}
		}
	}
}

vector<class CTileBlock*> CTileSystem::Get_NeighborByIndex(TILE_INDEX index)
{
	vector<class CTileBlock*> neighBorTile;

	if (!Check_ValidIndex(index))
		return neighBorTile;

	BlockLayer& Layer = m_TileContainer[index.IndexY]; //°°Àº Ãþ ·¹ÀÌ¾î Å½»ö
	neighBorTile.resize(NEIGHBOR_INDEX::END, nullptr);
	//UPLEFT, UP, UPRIGHT, LEFT, CENTER, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT,END    
	neighBorTile[0] = Get_TileBlockByIndex({ index.IndexX - 1, index.IndexY,index.IndexZ + 1 });//zÀ§·Î x¿Þ;
	neighBorTile[1] = Get_TileBlockByIndex({ index.IndexX       , index.IndexY,index.IndexZ + 1 });
	neighBorTile[2] = Get_TileBlockByIndex({ index.IndexX + 1, index.IndexY,index.IndexZ + 1 });
	neighBorTile[3] = Get_TileBlockByIndex({ index.IndexX - 1, index.IndexY,index.IndexZ });
	neighBorTile[4] = Get_TileBlockByIndex({ index.IndexX, index.IndexY,index.IndexZ });
	neighBorTile[5] = Get_TileBlockByIndex({ index.IndexX + 1, index.IndexY,index.IndexZ });
	neighBorTile[6] = Get_TileBlockByIndex({ index.IndexX - 1, index.IndexY,index.IndexZ - 1 });
	neighBorTile[7] = Get_TileBlockByIndex({ index.IndexX , index.IndexY,index.IndexZ - 1 });
	neighBorTile[8] = Get_TileBlockByIndex({ index.IndexX + 1, index.IndexY,index.IndexZ - 1 });

	return neighBorTile;
}

CTileBlock* CTileSystem::Get_TileBlockByIndex(TILE_INDEX index)
{
	if (!Check_ValidIndex(index))
		return nullptr;

	BlockLayer& Layer = m_TileContainer[index.IndexY]; //°°Àº Ãþ ·¹ÀÌ¾î Å½»ö
	return Layer[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX];
}

_bool CTileSystem::Check_ValidIndex(TILE_INDEX index)
{
	if (index.IndexX < 0 || index.IndexX >= static_cast<_int>(m_tTileInfo.iTileCountX))
		return false;
	if (index.IndexY < 0 || index.IndexY >= static_cast<_int>(m_tTileInfo.iTileCountY))
		return false;
	if (index.IndexZ < 0 || index.IndexZ >= static_cast<_int>(m_tTileInfo.iTileCountZ))
		return false;

	return true;
}

CTileSystem* CTileSystem::Create(const TILESYSTEM_INFO& tileInfo)
{
	CTileSystem* instance = new CTileSystem();
	if (FAILED(instance->Initialize(tileInfo))) {
		Safe_Release(instance);
	}
	return instance;
}

void CTileSystem::Free()
{
	__super::Free();

}
