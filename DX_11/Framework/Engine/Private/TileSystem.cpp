#include "TileSystem.h"
#include "TileBlock.h"
#include "GameInstance.h"
#include "Shader.h"

#ifdef _DEBUG
#include "IResourceService.h"
#include "ICameraService.h"
#include "PipeLine.h"
#endif // _DEBUG


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

HRESULT CTileSystem::Add_TileFlagByIndex(TILE_INDEX index, _uint flag)
{
	if (!Check_ValidIndex(index))
		return E_FAIL;

	BlockLayer& Layer = m_TileContainer[index.IndexY]; //°°Àº Ãþ ·¹ÀÌ¾î Å½»ö
	Layer[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX].TileFlag |= flag;

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

HRESULT CTileSystem::Remove_TileFlagByIndex(TILE_INDEX index, _uint flag)
{
	if (!Check_ValidIndex(index))
		return E_FAIL;

	BlockLayer& Layer = m_TileContainer[index.IndexY]; //°°Àº Ãþ ·¹ÀÌ¾î Å½»ö	
	Layer[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX].TileFlag &= ~flag;

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

TILE_INFO CTileSystem::Find_Info(TILE_INDEX index)
{
	if (!Check_ValidIndex(index))
		return TILE_INFO();

	BlockLayer& Layer = m_TileContainer[index.IndexY]; //°°Àº Ãþ ·¹ÀÌ¾î Å½»ö
	return Layer[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX];
}

#ifdef _DEBUG
HRESULT CTileSystem::Ready_DebugBuffer()
{
#pragma region Create_Vertex
	vector<VTXTILE> VB;
	vector<_uint> IB;

	const _float3 vMin = m_tTileInfo.vWorldMin;
	const _float3 vTileSize = m_tTileInfo.SizePerTile();
	const _uint iTileCountX = m_tTileInfo.iTileCountX;
	const _uint iTileCountZ = m_tTileInfo.iTileCountZ;

 	VB.reserve(iTileCountX * iTileCountZ * 4);
	IB.reserve(iTileCountX * iTileCountZ * 6);

	for (size_t z = 0; z < iTileCountZ; ++z)
	{
		for (size_t x = 0; x < iTileCountX; ++x)
		{
			_uint tileIndex = (_uint)(z * iTileCountX + x);

			// Å¸ÀÏ ±âÁØ À§Ä¡
			 float baseX = vMin.x + x * vTileSize.x;
			 float baseY = vMin.y + 0.5f;
			 float baseZ = vMin.z + z * vTileSize.z;

			VTXTILE v0, v1, v2, v3;

			v0.vPosition = _float3(baseX, baseY, baseZ);
			v1.vPosition = _float3(baseX, baseY, baseZ + vTileSize.z);
			v2.vPosition = _float3(baseX + vTileSize.x, baseY, baseZ + vTileSize.z);
			v3.vPosition = _float3(baseX + vTileSize.x, baseY, baseZ);

			v0.iTileOffset = v1.iTileOffset = v2.iTileOffset = v3.iTileOffset = tileIndex;

			_uint baseVertex = (_uint)VB.size();

			VB.push_back(v0);
			VB.push_back(v1);
			VB.push_back(v2);
			VB.push_back(v3);

			IB.push_back(baseVertex + 0);
			IB.push_back(baseVertex + 1);
			IB.push_back(baseVertex + 2);

			IB.push_back(baseVertex + 0);
			IB.push_back(baseVertex + 2);
			IB.push_back(baseVertex + 3);
		}
	}

	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = sizeof(VTXTILE) * m_tTileInfo.iTileCountX * m_tTileInfo.iTileCountZ*4;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = sizeof(VTXTILE);

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = VB.data();

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

	HRESULT hr = CGameInstance::GetInstance()->Get_Device()->CreateBuffer(&VBDesc, &subData, &m_pVB);
	hr = CGameInstance::GetInstance()->Get_Device()->CreateBuffer(&IBDesc, &IBData, &m_pIB);
#pragma endregion

#pragma region Create_InpuLayOut
	auto pDevice = CGameInstance::GetInstance()->Get_Device();
	D3DX11_PASS_DESC passDesc = {};

	m_pDebugShader = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Shader(G_GlobalLevelKey, "VTX_Debug.hlsl");
	Safe_AddRef(m_pDebugShader);
	m_pDebugShader->GetPassSignature("DebugTile", &passDesc);

	pDevice->CreateInputLayout(
		VTXTILE::Elements, VTXTILE::iElementCount,
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
		&m_pLayout);
#pragma endregion


#pragma region Create_StructureBuffer
	D3D11_BUFFER_DESC IndicesBufferDesc = {};
	IndicesBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	IndicesBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	IndicesBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	IndicesBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	IndicesBufferDesc.StructureByteStride = sizeof(DEBUG_TILE);
	IndicesBufferDesc.ByteWidth = sizeof(DEBUG_TILE) * m_tTileInfo.iTileCountX * m_tTileInfo.iTileCountZ;

	pDevice->CreateBuffer(&IndicesBufferDesc, nullptr, &m_TileIndicesBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC TileResourceDesc = {};
	TileResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX; /*ÅØ½ºÃ³°¡ ¾Æ´Ï´Ù! */
	TileResourceDesc.BufferEx.NumElements = m_tTileInfo.iTileCountX * m_tTileInfo.iTileCountZ;
	TileResourceDesc.BufferEx.FirstElement = 0; 
	TileResourceDesc.Format = DXGI_FORMAT_UNKNOWN;										/*ÇÈ¼¿ ¾Æ´Ô*/
	pDevice->CreateShaderResourceView(m_TileIndicesBuffer, &TileResourceDesc, &m_pTileIndexResource);
#pragma endregion

	return hr;
}

void CTileSystem::Update_DebugColor(ID3D11DeviceContext* pContext)
{
	D3D11_MAPPED_SUBRESOURCE pDebugBuffer = {};
	HRESULT hr = pContext->Map(
		m_TileIndicesBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pDebugBuffer
	);

	if (pDebugIndex.empty())
		pDebugIndex.resize(m_tTileInfo.iTileCountX * m_tTileInfo.iTileCountZ);

	for (_int z = 0; z < m_tTileInfo.iTileCountZ; ++z)
	{
		for (_int x = 0; x < m_tTileInfo.iTileCountX; ++x)
		{
			DEBUG_TILE i = {};
			_int idx = z * m_tTileInfo.iTileCountX + x;
			pDebugIndex[idx].Color = Get_TileDebugColor(TILE_INDEX{x,0,z});
		}
	}

	memcpy(pDebugBuffer.pData, pDebugIndex.data(), sizeof(DEBUG_TILE)* pDebugIndex.size());

	pContext->Unmap(m_TileIndicesBuffer, 0);
}

void CTileSystem::RegisterColorRule(const string& name, function<_float4(const TILE_INFO&)> func)
{
	m_DebugColorRules[name] = func;
}

void CTileSystem::SetActiveColorRule(const string& name)
{
	m_ActiveRuleName = name;
}

_float4 CTileSystem::Get_TileDebugColor(const TILE_INDEX& index)
{
	auto iter = m_DebugColorRules.find(m_ActiveRuleName);
	if (iter == m_DebugColorRules.end())
		return { 0.f,0.f,0.f,1.f };

	return iter->second(Find_Info(index));
}

HRESULT CTileSystem::Render_Tiles(ID3D11DeviceContext* pContext, CPipeLine* pPipeLine)
{
	if (m_bDebug != true) return S_OK;

	Update_DebugColor(pContext);
	m_pDebugShader->SetConstantBuffer("FrameBuffer", pPipeLine->Get_FrameBuffer());
	
	SHADER_PARAM TileIndiciesParam = {};
	TileIndiciesParam.iSize = sizeof(TILE_INDEX) * m_tTileInfo.iTileCountX *  m_tTileInfo.iTileCountZ;
	TileIndiciesParam.typeName = "StructuredBuffer";
	TileIndiciesParam.pData = m_pTileIndexResource;
	m_pDebugShader->Bind_Value("g_TileIndecies", TileIndiciesParam);
	
	ID3D11Buffer* pVertexBuffers[] = { m_pVB };
	_uint  pVertexStride[] = { sizeof(VTXTILE) };
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
	Safe_Release(m_TileIndicesBuffer);
	Safe_Release(m_pTileIndexResource);
#endif // _DEBUG

}
