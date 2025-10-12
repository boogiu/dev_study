#include "TileSystem.h"
#include "TileBlock.h"

static const CTileSystem::Offset NeighborOffsets[] = {
        { 0,  0}, // CENTER
        {+1,  0}, // RIGHT
        {+1, -1}, // RIGHT_BOTTOM
        { 0, -1}, // BOTTOM
        {-1, -1}, // BOTTOM_LEFT
        {-1,  0}, // LEFT
        {-1, +1}, // LEFT_TOP
        { 0, +1}, // TOP
        {+1, +1}  // TOP_RIGHT
};

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
    _float DistanceX = WorldPos.x - m_tTileInfo.OriginPoint.x;
    _float DistanceY = WorldPos.y - m_tTileInfo.OriginPoint.y;
    _float DistanceZ = WorldPos.z - m_tTileInfo.OriginPoint.z;

    _int    TileIndexX = static_cast<_int>(floorf(DistanceX / m_tTileInfo.iSizeXPerTile));
    _int    TileIndexY = static_cast<_int>(floorf(DistanceY / m_tTileInfo.iSizeYPerTile));
    _int    TileIndexZ = static_cast<_int>(floorf(DistanceZ / m_tTileInfo.iSizeZPerTile));

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

TILE_INDEX CTileSystem::Register_Tile(CTileBlock* block, TILE_INDEX index)
{
    if(!Check_ValidIndex(index))
        return TILE_INDEX();

    BlockLayer& TileLayer =  m_TileContainer[index.IndexY];
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
                desireIndex.IndexZ+= 1;
            }
        }
   }
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
