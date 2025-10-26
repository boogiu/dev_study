#include "Editor_Defines.h"
#include "StructureObject.h"

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

CStructureObject::CStructureObject()
{
}

CStructureObject::CStructureObject(const CStructureObject& rhs)
	:CGameObject(rhs)
{
}

HRESULT CStructureObject::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CMaterial>();
	Add_Component<CStaticModel>();
	Add_Component<CTileBlock>();
	return S_OK;
}

HRESULT CStructureObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Add_Component<CDebugRender>();

	return S_OK;
}

void CStructureObject::Priority_Update(_float dt)
{
}

void CStructureObject::Update(_float dt)
{
}

void CStructureObject::Late_Update(_float dt)
{
}

void CStructureObject::Object_OnGrid(TILE_INDEX index)
{
	CEditorSystem::Editor_Context* pContext = CEditorSystem::GetInstance()->Get_Context();
	_float4 Anchor = CGameInstance::GetInstance()->Get_TileSystem()->Get_PositionByIndex(index, static_cast<ANCHOR>(pContext->eAnchor));
	m_pTransform->Set_Pos({ Anchor.x,Anchor.y,Anchor.z });
	m_SyncedIndex = index;
}

HRESULT CStructureObject::Link_Data(const string& folderName)
{
	HRESULT hr = Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, folderName + ".model");
	CMaterial* pMaterial = Get_Component<CMaterial>();
	hr = pMaterial->Link_Material(G_GlobalLevelKey, folderName + ".mat");
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
		instance->Override_Pass("Base");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassRiverXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassCliffXlu")) {
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

HRESULT CStructureObject::Load_Object(OLD_MAP_OBJECT_HEADER ObjHeader)
{
	m_SyncedIndex = ObjHeader.Index;
	MaterialName = ObjHeader.MaterialName;
	ModelName = ObjHeader.ModelName;
	m_pTransform->Set_Pos({ ObjHeader.vWorldPos.x,ObjHeader.vWorldPos.y,ObjHeader.vWorldPos.z });

	CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(MaterialName, ObjHeader.MaterialPath);
	CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(ModelName, ObjHeader.ModelPath);

	HRESULT hr = Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, ModelName);
	CMaterial* pMaterial = Get_Component<CMaterial>();
	hr = pMaterial->Link_Material(G_GlobalLevelKey, MaterialName);

	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
		instance->Override_Pass("Base");
	}
	return hr;
}

HRESULT CStructureObject::Save_MapData(ofstream& ofs)
{
	/*ÇöÀç ÀÎµ¦½º*/
	MAP_OBJECT_HEADER ObjHeader = {};
	ObjHeader.Index = m_SyncedIndex;
	ObjHeader.Object_type = static_cast<_uint>(m_eType);

	//XMStoreFloat4(&ObjHeader.vWorldPos, m_pTransform->Get_Pos());
	strcpy_s(ObjHeader.ModelName, sizeof(ObjHeader.ModelName), ModelName.c_str());
	strcpy_s(ObjHeader.ModelPath, sizeof(ObjHeader.ModelPath), CGameInstance::GetInstance()->Get_ResourceMgr()->Get_ResourcePath(ModelName).c_str());
	strcpy_s(ObjHeader.MaterialName, sizeof(ObjHeader.MaterialName), MaterialName.c_str());
	strcpy_s(ObjHeader.MaterialPath, sizeof(ObjHeader.MaterialPath), CGameInstance::GetInstance()->Get_ResourceMgr()->Get_ResourcePath(MaterialName).c_str());

	ofs.write(reinterpret_cast<const char*>(&ObjHeader), sizeof(OLD_MAP_OBJECT_HEADER));
	return S_OK;
}

void CStructureObject::Render_GUI()
{
	__super::Render_GUI();
}

CStructureObject* CStructureObject::Create()
{
	CStructureObject* instance = new CStructureObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CStructureObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CStructureObject::Clone(INIT_DESC * pArg)
{
	CStructureObject* instance = new CStructureObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CStructureObject");
		Safe_Release(instance);
	}

	return instance;
}

void CStructureObject::Free()
{
	__super::Free();
}