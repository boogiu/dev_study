#include "Editor_Defines.h"
#include "FieldOutBlocks.h"

#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"

#include "DebugRender.h"
#include "TileBlock.h"

#include "RayReceiver.h"
#include "EditorSystem.h"
#include "GameInstance.h"
#include "ITileService.h"
#include "IResourceService.h"

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
	if (OnGrid) {
		_float4 Anchor = CGameInstance::GetInstance()->Get_TileSystem()->Get_PositionByIndex(m_SyncedIndex, ANCHOR::Right | ANCHOR::Bottom);
		m_pTransform->Set_Pos({ Anchor.x,Anchor.y,Anchor.z });
	}
}

void CFieldOutBlocks::Update(_float dt)
{
}

void CFieldOutBlocks::Late_Update(_float dt)
{
}

void CFieldOutBlocks::Object_OnGrid(TILE_INDEX index)
{
	Get_Component<CDebugRender>()->Add_DebugBounding(Get_Component<CModel>()->Get_LocalBoundingBox());
	_float4 Anchor = CGameInstance::GetInstance()->Get_TileSystem()->Get_PositionByIndex(index, ANCHOR::Right | ANCHOR::Bottom);
	m_pTransform->Set_Pos({ Anchor.x,Anchor.y,Anchor.z });
	m_SyncedIndex = index;
	OnGrid = true;
}


HRESULT CFieldOutBlocks::Link_Data(const string& folderName)
{
	HRESULT hr = Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, folderName + ".model");
	CMaterial* pMaterial = Get_Component<CMaterial>();
	hr = pMaterial->Link_Material(G_GlobalLevelKey, folderName + ".mat");

	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassRiverXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
		instance->Override_Pass("Base");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mWaterfall")) {
		instance->Override_Pass("Water");
	}
	if (SUCCEEDED(hr)) {
		ModelName = folderName + ".model";
		MaterialName = folderName + ".mat";
	}

	return hr;
}

HRESULT CFieldOutBlocks::Load_Object(MAP_OBJECT_HEADER ObjHeader)
{
	m_SyncedIndex=ObjHeader.Index;
	MaterialName = ObjHeader.MaterialName;
	ModelName= ObjHeader.ModelName;
	m_pTransform->Set_Pos({ ObjHeader.vWorldPos.x,ObjHeader.vWorldPos.y,ObjHeader.vWorldPos.z });

	CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(MaterialName, ObjHeader.MaterialPath);
	CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(ModelName, ObjHeader.ModelPath);


	HRESULT hr = Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, ModelName);
	CMaterial* pMaterial = Get_Component<CMaterial>();
	hr = pMaterial->Link_Material(G_GlobalLevelKey, MaterialName);

	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassRiverXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
		instance->Override_Pass("Base");
	}	
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mWaterfall")) {
		instance->Override_Pass("Water");
	}


	return hr;
}

HRESULT CFieldOutBlocks::Save_MapData(ofstream& ofs)
{
	/*ÇöÀç ÀÎµ¦½º*/
	MAP_OBJECT_HEADER ObjHeader = {};
	ObjHeader.Index = m_SyncedIndex;
	XMStoreFloat4(&ObjHeader.vWorldPos, m_pTransform->Get_Pos());
	strcpy_s(ObjHeader.ModelName, sizeof(ObjHeader.ModelName), ModelName.c_str());
	strcpy_s(ObjHeader.ModelPath, sizeof(ObjHeader.ModelPath), CGameInstance::GetInstance()->Get_ResourceMgr()->Get_ResourcePath(ModelName).c_str());
	strcpy_s(ObjHeader.MaterialName, sizeof(ObjHeader.MaterialName), MaterialName.c_str());
	strcpy_s(ObjHeader.MaterialPath, sizeof(ObjHeader.MaterialPath), CGameInstance::GetInstance()->Get_ResourceMgr()->Get_ResourcePath(MaterialName).c_str());

	ofs.write(reinterpret_cast<const char*>(&ObjHeader), sizeof(MAP_OBJECT_HEADER));
	return S_OK;
}

void CFieldOutBlocks::Render_GUI()
{
	__super::Render_GUI();
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