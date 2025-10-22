#include "Editor_Defines.h"
#include "MapObject.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"

#include "TileBlock.h"

#include "GameInstance.h"
#include "RayReceiver.h"
#include "EditorSystem.h"
#include "ITileService.h"
#include "IResourceService.h"

CMapObject::CMapObject()
{
}

CMapObject::CMapObject(const CMapObject& rhs)
	:CGameObject(rhs)
{
}

HRESULT CMapObject::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CMaterial>();
	Add_Component<CSkeletalModel>();
	Add_Component<CTileBlock>();
	return S_OK;
}

HRESULT CMapObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CMapObject::Priority_Update(_float dt)
{
}

void CMapObject::Update(_float dt)
{
}

void CMapObject::Late_Update(_float dt)
{
}

void CMapObject::Object_OnGrid(TILE_INDEX index)
{
	//Get_Component<CDebugRender>()->Add_DebugBounding(Get_Component<CModel>()->Get_LocalBoundingBox());
	_float4 Anchor = CGameInstance::GetInstance()->Get_TileSystem()->Get_PositionByIndex(index, ANCHOR::Right | ANCHOR::Bottom);
	m_pTransform->Set_Pos({ Anchor.x,Anchor.y,Anchor.z });
	m_SyncedIndex = index;
}

HRESULT CMapObject::Link_Data(const string& folderName)
{
	HRESULT hr = Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, folderName + ".model");
	CMaterial* pMaterial = Get_Component<CMaterial>();
	hr = pMaterial->Link_Material(G_GlobalLevelKey, folderName + ".mat");
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
		instance->Override_Pass("Base");
	}
	if (SUCCEEDED(hr)) {
		ModelName = folderName + ".model";
		MaterialName = folderName + ".mat";
	}

	return hr;
}

HRESULT CMapObject::Load_Object(MAP_OBJECT_HEADER ObjHeader)
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

HRESULT CMapObject::Save_MapData(ofstream& ofs)
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

void CMapObject::Render_GUI()
{
	__super::Render_GUI();
}

CMapObject* CMapObject::Create()
{
	CMapObject* instance = new CMapObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CMapObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CMapObject::Clone(INIT_DESC* pArg)
{
	CMapObject* instance = new CMapObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CMapObject");
		Safe_Release(instance);
	}

	return instance;
}

void CMapObject::Free()
{
	__super::Free();
}