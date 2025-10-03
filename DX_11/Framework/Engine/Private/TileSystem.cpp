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
    _uint TileSize = m_tTileInfo.iTileCountX* m_tTileInfo.iTileCountY* m_tTileInfo.iTileCountZ;
    m_Blocks.resize(TileSize, nullptr);

    return S_OK;
}


_int CTileSystem::Register_Tile(CTileBlock* tileBlock, _uint x, _uint y, _uint z)
{
    _uint countX = m_tTileInfo.iTileCountX;
    _uint countZ = m_tTileInfo.iTileCountZ;
    _uint countY = m_tTileInfo.iTileCountY;

    while (true)
    {
        _uint Index = x + z * countX + y * (countX * countZ);

        if (m_Blocks[Index] == nullptr) {
            m_Blocks[Index] = tileBlock;
            Safe_AddRef(m_Blocks[Index]);

            tileBlock->Set_Index(Index);
            tileBlock->UpdatePosition(m_tTileInfo);
            return Index;
        }
        x++;
        if (x >= countX) {
            x = 0;
            z++;
            if (z >= countZ) {
                z = 0;
                y++;
                if (y >= countY) {
                    MSG_BOX("There is no Empty Space To Register Tile:  CTileSystem");
                    return -1;
                }
            }
        }
    }
}


void CTileSystem::UnRegister_Tile(_int Index)
{
    if (Index < 0 || Index >= m_Blocks.size()) return;

    Safe_Release(m_Blocks[Index]);
    m_Blocks[Index] = nullptr;
}

void CTileSystem::Get_IndexByPosition(_fvector vPos, _uint* x, _uint* y, _uint* z)
{
    _float4 vDistanceVector = {};
    XMStoreFloat4(&vDistanceVector, vPos - XMLoadFloat4(&m_tTileInfo.OriginPoint));
    /*원점에서의 거리 측정*/

    *x = (_uint)vDistanceVector.x / m_tTileInfo.iSizeXPerTile;  
    *y = (_uint)vDistanceVector.y / m_tTileInfo.iSizeYPerTile;  
    *z = (_uint)(vDistanceVector.z / m_tTileInfo.iSizeZPerTile);
}


void CTileSystem::Get_XYZByIndex(_uint Index, _uint* x, _uint* y, _uint* z)
{
    _uint YDevide = m_tTileInfo.iTileCountX * m_tTileInfo.iTileCountZ;
    *y = Index / YDevide;

    _uint XZDevide = Index % YDevide;

    *z = XZDevide / m_tTileInfo.iTileCountX;
    *x= XZDevide% m_tTileInfo.iTileCountX;
}


vector<CTileBlock*> CTileSystem::Get_NeighborByIndex(_uint Index)
{
    _uint x, y, z;
    Get_XYZByIndex(Index, &x, &y, &z);

    struct Offset { int dx, dy, dz; };

    vector<CTileBlock*> neighbors;
    neighbors.reserve(static_cast<_int>(TILE_NEIGHBOR::END));

    for (size_t i = 0; i < static_cast<_int>(TILE_NEIGHBOR::END); i++)
    {
        int nx = static_cast<int>(x) + NeighborOffsets[i].dx;
        int nz = static_cast<int>(z) + NeighborOffsets[i].dz;

        if (nx < 0 || nx >= static_cast<int>(m_tTileInfo.iTileCountX)) continue;
        if (nz < 0 || nz >= static_cast<int>(m_tTileInfo.iTileCountX)) continue;

        _uint neighborIndex = Make_Index(nx, y, nz);
        neighbors.push_back(m_Blocks[neighborIndex]);
    }

    return neighbors;
}


_uint CTileSystem::Make_Index(_uint x, _uint y, _uint z)
{
    _uint countX = m_tTileInfo.iTileCountX;
    _uint countZ = m_tTileInfo.iTileCountZ;
    _uint countY = m_tTileInfo.iTileCountY;

    _uint Index = x + z * countX + y * (countX * countZ);

    return Index;
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

    for (auto& tileBlock : m_Blocks) {
        Safe_Release(tileBlock);
    }

    m_Blocks.clear();
}
