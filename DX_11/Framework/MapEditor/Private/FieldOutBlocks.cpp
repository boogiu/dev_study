#include "Editor_Defines.h"
#include "FieldOutBlocks.h"

#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"

#include "DebugRender.h"
#include "TileBlock.h"

#include "RayReceiver.h"
#include "EditorSystem.h"

CFieldOutBlocks::CFieldOutBlocks()
{
}

CFieldOutBlocks::CFieldOutBlocks(const CFieldOutBlocks& rhs)
	:CGameObject(rhs)
{
}

HRESULT CFieldOutBlocks::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CMaterial>();
	Add_Component<CStaticModel>();
	Add_Component<CRayReceiver>();
	Add_Component<CTileBlock>();

	return S_OK;
}

HRESULT CFieldOutBlocks::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	m_PaletteIndex = { 0.f, 96 * 0.4f };
	Add_Component<CDebugRender>();

	return S_OK;
}

void CFieldOutBlocks::Priority_Update(_float dt)
{
}

void CFieldOutBlocks::Update(_float dt)
{
}

void CFieldOutBlocks::Late_Update(_float dt)
{
}

void CFieldOutBlocks::Object_OnGrid(TILE_INDEX index)
{
	for (auto& bounding : Get_Component<CModel>()->Get_MeshBoundingBox()) {
		Get_Component<CDebugRender>()->Add_DebugBounding(bounding);
	}
	Get_Component<CTileBlock>()->On_Grid(index);
}


HRESULT CFieldOutBlocks::Link_Data(const string& folderName)
{
	HRESULT hr = Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, folderName + ".model");
	hr = Get_Component<CMaterial>()->Link_Material(G_GlobalLevelKey, folderName + ".mat");
	
	return hr;
}

HRESULT CFieldOutBlocks::Save_Blocks(ofstream& ofs)
{
	return S_OK;
}

void CFieldOutBlocks::Render_GUI()
{

}

CFieldOutBlocks* CFieldOutBlocks::Create()
{
	CFieldOutBlocks* instance = new CFieldOutBlocks();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CBlockObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CFieldOutBlocks::Clone(INIT_DESC* pArg)
{
	CFieldOutBlocks* instance = new CFieldOutBlocks(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CBlockObject");
		Safe_Release(instance);
	}

	return instance;
}

void CFieldOutBlocks::Free()
{
	__super::Free();
}