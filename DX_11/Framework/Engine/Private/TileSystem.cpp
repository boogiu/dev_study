#include "TileSystem.h"
#include "TileBlock.h"
#include "GameInstance.h"
#include "Shader.h"
#include "IResourceService.h"

CTileSystem::CTileSystem()
{
}


HRESULT CTileSystem::Initialize(const TILESYSTEM_INFO& tileInfo)
{
	m_tTileInfo = tileInfo;

	m_TileContainer.resize(m_tTileInfo.iTileCountY);

	for (BlockLayer& layer : m_TileContainer) {
		layer.resize(m_tTileInfo.iTileCountX * m_tTileInfo.iTileCountZ, {});
	}

	#ifdef _DEBUG
	Ready_DebugBuffer();
	#endif

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

TILE_INDEX CTileSystem::Register_Tile(CTileBlock* block, TILE_INDEX index,_bool CanFail)
{
	if (!Check_ValidIndex(index))
		return TILE_INDEX();

	BlockLayer& TileLayer = m_TileContainer[index.IndexY];
	TILE_INDEX desireIndex = index;
	while (true) {
		_int XZIndex = desireIndex.IndexX + desireIndex.IndexZ * m_tTileInfo.iTileCountX;

		if (nullptr == TileLayer[XZIndex].pTileBlock) {
			TileLayer[XZIndex].pTileBlock = block;

			block->Set_Index(desireIndex);
			block->Update_Position(m_tTileInfo);
			return desireIndex;
		}

		else if (CanFail) {
			return TILE_INDEX();
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
	neighBorTile.resize(9, nullptr);
	//UPLEFT, UP, UPRIGHT, LEFT, CENTER, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT,END    
	neighBorTile[0] =		Get_TileBlockByIndex({ index.IndexX - 1, index.IndexY,index.IndexZ + 1 });//zÀ§·Î x¿Þ;
	neighBorTile[1] =		Get_TileBlockByIndex({ index.IndexX       , index.IndexY,index.IndexZ + 1 });
	neighBorTile[2] =		Get_TileBlockByIndex({ index.IndexX + 1, index.IndexY,index.IndexZ + 1 });
	neighBorTile[3] =		Get_TileBlockByIndex({ index.IndexX - 1, index.IndexY,index.IndexZ });
	neighBorTile[4] =		Get_TileBlockByIndex({ index.IndexX, index.IndexY,index.IndexZ });
	neighBorTile[5] =		Get_TileBlockByIndex({ index.IndexX + 1, index.IndexY,index.IndexZ });
	neighBorTile[6] =		Get_TileBlockByIndex({ index.IndexX - 1, index.IndexY,index.IndexZ - 1 });
	neighBorTile[7] =		Get_TileBlockByIndex({ index.IndexX , index.IndexY,index.IndexZ - 1 });
	neighBorTile[8] =		Get_TileBlockByIndex({ index.IndexX + 1, index.IndexY,index.IndexZ - 1 });

	return neighBorTile;
}

vector<TILE_INDEX> CTileSystem::Get_IndeciesByArea(_float4 vMin, _float4 vMax)
{
	if (vMin.x > vMax.x) swap(vMin.x, vMax.x);
	if (vMin.y > vMax.y) swap(vMin.y, vMax.y);
	if (vMin.z > vMax.z) swap(vMin.z, vMax.z);

	TILE_INDEX MinIndex = Get_IndexByPosition(vMin);
	TILE_INDEX MaxIndex = Get_IndexByPosition(vMax);

	vector<TILE_INDEX> indices;

	if (!Check_ValidIndex(MinIndex) || !Check_ValidIndex(MinIndex)) { return indices; }

	for (int y = MinIndex.IndexY; y <= MaxIndex.IndexY; ++y)
	{
		for (int z = MinIndex.IndexZ; z <= MaxIndex.IndexZ-1; ++z)
		{
			for (int x = MinIndex.IndexX; x <= MaxIndex.IndexX-1; ++x)
			{
				TILE_INDEX idx;
				idx.IndexX = x;
				idx.IndexY = y;
				idx.IndexZ = z;
				if (!Check_ValidIndex(idx)) continue;

				indices.push_back(idx);
			}
		}
	}
	return indices;
}

HRESULT CTileSystem::Add_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag)
{
	for (auto Index : indices) {
		if (!Check_ValidIndex(Index))
			continue;
		BlockLayer& Layer = m_TileContainer[Index.IndexY]; //°°Àº Ãþ ·¹ÀÌ¾î Å½»ö
		Layer[Index.IndexX + Index.IndexZ * m_tTileInfo.iTileCountX].TileFlag |= flag;
	}
	return S_OK;
}

HRESULT CTileSystem::Remove_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag)
{
	for (auto Index : indices) {
		if (!Check_ValidIndex(Index))
			continue;
		BlockLayer& Layer = m_TileContainer[Index.IndexY]; //°°Àº Ãþ ·¹ÀÌ¾î Å½»ö	
		Layer[Index.IndexX + Index.IndexZ * m_tTileInfo.iTileCountX].TileFlag &= ~flag;

	}
	return S_OK;
}

_uint CTileSystem::Get_TileFlagByIndex(TILE_INDEX index)
{
	BlockLayer& Layer = m_TileContainer[index.IndexY]; //°°Àº Ãþ ·¹ÀÌ¾î Å½»ö	
	return Layer[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX].TileFlag;
}

_bool CTileSystem::Check_TileFlagByPosition(_float4 WorldPos, _uint flag)
{
	TILE_INDEX Index = Get_IndexByPosition(WorldPos);
	if (!Check_ValidIndex(Index)) return false;

	BlockLayer& Layer = m_TileContainer[Index.IndexY]; //°°Àº Ãþ ·¹ÀÌ¾î Å½»ö	
	_uint Flag =  Layer[Index.IndexX + Index.IndexZ * m_tTileInfo.iTileCountX].TileFlag;
	return(Flag & flag) != 0;
}


CTileBlock* CTileSystem::Get_TileBlockByIndex(TILE_INDEX index)
{
	if (!Check_ValidIndex(index))
		return nullptr;

	BlockLayer& Layer = m_TileContainer[index.IndexY]; //°°Àº Ãþ ·¹ÀÌ¾î Å½»ö
	return Layer[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX].pTileBlock;
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

#ifdef _DEBUG
HRESULT CTileSystem::Ready_DebugBuffer()
{
	_float3 vMin = m_tTileInfo.vWorldMin;
	_float3 vMax = m_tTileInfo.vWorldMax;

	vector<VTXCOL> VB;
	VB.resize(m_tTileInfo.iTileCountX * m_tTileInfo.iTileCountZ);

	for (size_t i = 0; i < m_tTileInfo.iTileCountZ; i++)
	{
		for (size_t j = 0; j < m_tTileInfo.iTileCountX; j++)
		{
			_uint       iIndex = i * m_tTileInfo.iTileCountX + j;
			VB[iIndex].vPosition = _float3(vMin.x + j, vMin.y+0.5f, vMin.z + i);
			VB[iIndex].vColor = { 1.f,1.f,0.f,1.f };
		}
	}

	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = sizeof(VTXCOL) * m_tTileInfo.iTileCountX * m_tTileInfo.iTileCountZ;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = sizeof(VTXCOL);

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = VB.data();
	HRESULT hr = CGameInstance::GetInstance()->Get_Device()->CreateBuffer(&VBDesc, &subData, &m_pVB);


	vector<_uint> IB;
	_uint       iNumIndices = {};
	for (size_t i = 0; i < m_tTileInfo.iTileCountZ - 1; i++)
	{
		for (size_t j = 0; j < m_tTileInfo.iTileCountX - 1; j++)
		{
			_uint       iIndex = i * m_tTileInfo.iTileCountX + j;

			_uint       iIndices[4] = {
				iIndex + m_tTileInfo.iTileCountX,
				iIndex + m_tTileInfo.iTileCountX + 1,
				iIndex + 1,
				iIndex
			};

			IB.push_back(iIndices[0]);
			IB.push_back(iIndices[1]);
			IB.push_back(iIndices[2]);
			IB.push_back(iIndices[0]);
			IB.push_back(iIndices[2]);
			IB.push_back(iIndices[3]);
		}
	}
	IB_Count = IB.size();

	D3D11_BUFFER_DESC IBDesc;
	IBDesc.ByteWidth = sizeof(_uint) * IB.size();
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;
	IBDesc.StructureByteStride = sizeof(_uint);

	D3D11_SUBRESOURCE_DATA IBData;
	IBData.pSysMem = IB.data();

	hr = CGameInstance::GetInstance()->Get_Device()->CreateBuffer(&IBDesc, &IBData, &m_pIB);

	auto pDevice = CGameInstance::GetInstance()->Get_Device();
	D3DX11_PASS_DESC passDesc = {};

	m_pDebugShader = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Shader(G_GlobalLevelKey, "VTX_Debug.hlsl");
	Safe_AddRef(m_pDebugShader);
	m_pDebugShader->GetPassSignature("DebugTile", &passDesc);

	pDevice->CreateInputLayout(
		VTXCOL::Elements, VTXCOL::iElementCount,
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
		&m_pLayout);

	return hr;
}
#endif // _DEBUG

#ifdef _DEBUG
HRESULT CTileSystem::Render_Tiles(ID3D11DeviceContext* pContext)
{
	//m_pDebugShader->Bind_Value("matView", camMgr->Get_ViewMatrix());
	//m_pDebugShader->SetMatrix("matProjection", camMgr->Get_ProjMatrix());

	ID3D11Buffer* pVertexBuffers[] = { m_pVB };
	_uint  pVertexStride[] = { sizeof(VTXCOL) };
	_uint  pVertexOffset[] = { 0 };
	pContext->IASetInputLayout(m_pLayout);
	m_pDebugShader->Apply("DebugTile", pContext);

	pContext->IASetVertexBuffers(0, 1, pVertexBuffers, pVertexStride, pVertexOffset);
	pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->DrawIndexed(IB_Count, 0, 0);

	return S_OK;
}
#endif // _DEBUG
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

#ifdef _DEBUG
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
	Safe_Release(m_pLayout);
	Safe_Release(m_pDebugShader);
#endif // _DEBUG

}
