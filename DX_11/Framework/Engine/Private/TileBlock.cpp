#include "TileBlock.h"
#include "Texture.h"
#include "TileSystem.h"
#include "GameInstance.h"
#include "TileSystem.h"
#include "GameObject.h"
#include "Transform.h"

CTileBlock::CTileBlock()
{
}

CTileBlock::CTileBlock(const CTileBlock& rhs)
	:CComponent(rhs), m_pTileSystem{ rhs.m_pTileSystem }
{
}

HRESULT CTileBlock::Initialize_Prototype()
{
	m_pTileSystem = CGameInstance::GetInstance()->Get_TileSystem();

	if (m_pTileSystem == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CTileBlock::Initialize(COMPONENT_DESC* pArg)
{
	m_pTransform = m_pOwner->Get_Component<CTransform>();

	if (!m_pTransform)
		return E_FAIL;
	Safe_AddRef(m_pTransform);

	return  S_OK;
}

void CTileBlock::Set_Index(_uint Index)
{
	if (!m_pTransform) {
		m_pTransform = m_pOwner->Get_Component<CTransform>();
	}

	m_Index = static_cast<_int>(Index);
}

void CTileBlock::UpdatePosition(const TILESYSTEM_INFO& tileInfo)
{
	_uint x, y, z = {};

	m_pTileSystem->Get_XYZByIndex(m_Index, &x, &y, &z);

	_float newX = tileInfo.OriginPoint.x +	tileInfo.iSizeXPerTile	*0.5f	+	tileInfo.iSizeXPerTile * x;
	_float newY = tileInfo.OriginPoint.y +	tileInfo.iSizeYPerTile * y;
	_float newZ = tileInfo.OriginPoint.z +	tileInfo.iSizeZPerTile	*0.5f	+	tileInfo.iSizeZPerTile * z;

	m_pTransform->Set_Pos({ newX ,newY, newZ });
}

void CTileBlock::Set_TilePostion(_uint x, _uint y, _uint z)
{
	if (m_Index != -1) {
		m_pTileSystem->UnRegister_Tile(m_Index);
	}

	m_Index = m_pTileSystem->Register_Tile(this, x, y, z);
}

void CTileBlock::Get_TilePostion(_uint* x, _uint* y, _uint* z)
{
	m_pTileSystem->Get_XYZByIndex(m_Index, x, y, z);
}

vector<CTileBlock*> CTileBlock::Get_Neighbor()
{
	return vector<CTileBlock*>();
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
	Safe_Release(m_pPaletteTexture);
	Safe_Release(m_pEdgeTexture);
}
