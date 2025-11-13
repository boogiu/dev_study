#include "TileBlock.h"
#include "Texture.h"
#include "TileSystem.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"
#include "Model.h"

/*이클래스는 타일 시스템이 들고 애드래프하고 있어서 여기서도 타일 시스템 애드레프하면 순환참조 일어남*/
CTileBlock::CTileBlock()
{
}

CTileBlock::CTileBlock(const CTileBlock& rhs)
	:CComponent(rhs)
{
}

HRESULT CTileBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTileBlock::Initialize(COMPONENT_DESC* pArg)
{
	/*이니셜라이즈 당시 트랜스폼과 타일 시스템이 있는지 확인*/
	m_pTransform = m_pOwner->Get_Component<CTransform>();

	if (!m_pTransform)
		return E_FAIL;

	m_pTileSystem = CGameInstance::GetInstance()->Get_TileSystem();

	if (m_pTileSystem == nullptr)
		return E_FAIL;

	Safe_AddRef(m_pTransform);

	return  S_OK;
}

TILE_INDEX CTileBlock::On_Grid(TILE_INDEX tileIndex, string eType, _bool CanFail)
{
	m_pTileSystem->Register_Tile(this, tileIndex, CanFail);
	m_eType = eType;
	return m_tIndex;
}

void CTileBlock::Set_Index(TILE_INDEX tileIndex)
{
	m_tIndex = tileIndex;
}

void CTileBlock::Update_Position(TILESYSTEM_INFO& systemInfo)
{

	_float tileSizeX = systemInfo.SizePerTile().x;
	_float tileSizeY = m_pOwner->Get_Position().y;
	_float tileSizeZ = systemInfo.SizePerTile().z;

	
	_float3 anchor = {
		systemInfo.vWorldMin.x + (tileSizeX * m_tIndex.IndexX),
		systemInfo.vWorldMin.y+0.05f ,
		systemInfo.vWorldMin.z + (tileSizeZ * m_tIndex.IndexZ)
	};

	_float3 worldPos = {};
	/*그 기준은 모델의 바운딩 박스로*/
	if (CModel* pModel = m_pOwner->Get_Component<CModel>()) {
		MINMAX_BOX box = pModel->Get_WorldBoundingBox();
		_float halfSizeX = (box.vMax.x - box.vMin.x) * 0.5f;
		_float halfSizeZ = (box.vMax.z - box.vMin.z) * 0.5f;
		_float halfSizeY = (box.vMax.y - box.vMin.y) * 0.5f;

		worldPos = {
			anchor.x + halfSizeX,
			m_pOwner->Get_Position().y,
			anchor.z + halfSizeZ
		};

	}
	else {//모델 없으면 그냥 중점에 맞추는걸로
		worldPos = {
		anchor.x + tileSizeX * 0.5f,
		m_pOwner->Get_Position().y,
		anchor.z + tileSizeZ * 0.5f
		};
	}
	m_pTransform->Set_Pos({ worldPos.x, XMVectorGetY(m_pTransform->Get_Pos()),worldPos.z });
}

_uint CTileBlock::Get_NeigborState()
{
	vector<class CTileBlock*> TileNeighbor = m_pTileSystem->Get_NeighborByIndex(m_tIndex);
	_uint Result = 0;

	for (size_t i = 0; i < TileNeighbor.size(); i++)
	{
		if (TileNeighbor[i] == this) continue;
		if (TileNeighbor[i] && TileNeighbor[i]->m_eType == m_eType)
				Result |= (1 << i);
	}

	return Result;
}

_uint CTileBlock::Get_NeigborCount()
{
	vector<class CTileBlock*> TileNeighbor = m_pTileSystem->Get_NeighborByIndex(m_tIndex);
	_uint Count = {};
	for (size_t i = 0; i < TileNeighbor.size(); i++)
	{
		if (TileNeighbor[i] == this) continue;
		if (TileNeighbor[i] && TileNeighbor[i]->m_eType == m_eType)
			Count++;
	}
	return Count;
}


CTileBlock* CTileBlock::Create()
{
	CTileBlock* instance = new CTileBlock();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
		MSG_BOX("CTileBlock Comp Failed To Create : CTileBlock");
	}
	return instance;
}

CComponent* CTileBlock::Clone()
{
	return new CTileBlock(*this);
}

void CTileBlock::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
}

