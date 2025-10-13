#include "Editor_Defines.h"
#include "TileObject.h"

#include "SkeletalModel.h"
#include "StaticModel.h"

#include "Material.h"
#include "MaterialData.h"
#include "MaterialInstance.h"

#include "Texture.h"

#include "DebugRender.h"
#include "TileBlock.h"

#include "RayReceiver.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "IResourceService.h"

CTileObject::CTileObject()
{
}

CTileObject::CTileObject(const CTileObject& rhs)
	:CGameObject(rhs)
{
}

HRESULT CTileObject::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CMaterial>();
	Add_Component<CStaticModel>();
	Add_Component<CRayReceiver>();
	Add_Component<CTileBlock>();
	return S_OK;
}

HRESULT CTileObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	m_PaletteIndex = { 0.f, 0.2f };

	return S_OK;
}

void CTileObject::Priority_Update(_float dt)
{
	_ubyte neighBorState= Get_Component<CTileBlock>()->Get_NeigborState();

	if (neighBorState == 0) {

	}
}

void CTileObject::Update(_float dt)
{

}

void CTileObject::Late_Update(_float dt)
{
}

void CTileObject::Object_OnGrid(TILE_INDEX index)
{
	TILE_TYPE eType = TILE_TYPE::NONE;

	if (ModelName.find("DarkSoil") != string::npos) {
		eType = TILE_TYPE::ROAD;
	}

	Get_Component<CTileBlock>()->On_Grid(index, eType);
}

HRESULT CTileObject::Load_Object(MAP_OBJECT_HEADER ObjHeader)
{
	Get_Component<CTileBlock>()->On_Grid(ObjHeader.Index);
	MaterialName = ObjHeader.MaterialName;
	ModelName = ObjHeader.ModelName;
	m_pTransform->Set_Pos({ ObjHeader.vWorldPos.x,ObjHeader.vWorldPos.y,ObjHeader.vWorldPos.z });

	CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(MaterialName, ObjHeader.MaterialPath);
	CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(ModelName, ObjHeader.ModelPath);

	HRESULT hr = Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, ModelName);
	CMaterial* pMaterial = Get_Component<CMaterial>();
	hr = pMaterial->Link_Material(G_GlobalLevelKey, MaterialName);

	return S_OK;
}

HRESULT CTileObject::Link_Data(const string& folderName, _bool Base)
{
	HRESULT hr = Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, folderName + ".model");
	hr = Get_Component<CMaterial>()->Link_Material(G_GlobalLevelKey, folderName + ".mat");
	CMaterial* pMaterial = Get_Component<CMaterial>();

	if (Base) {
		for (auto& instance : pMaterial->Get_Material_Instance()) {
			instance->Override_Pass("Base");
		}
	}
	
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
		instance->Override_Pass("Base");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassCliffXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassRiverXlu")) {
		instance->Override_Pass("Edge");
	}

	if (SUCCEEDED(hr)) {
		ModelName = folderName + ".model";
		MaterialName = folderName + ".mat";
	}

	return hr;
}

HRESULT CTileObject::Save_MapData(ofstream& ofs, _bool Base)
{
	/*ÇöÀç ÀÎµ¦½º*/
	MAP_OBJECT_HEADER ObjHeader = {};
	ObjHeader.Index = Get_Component<CTileBlock>()->Get_Index();
	XMStoreFloat4(&ObjHeader.vWorldPos, m_pTransform->Get_Pos());
	strcpy_s(ObjHeader.ModelName, sizeof(ObjHeader.ModelName), ModelName.c_str());
	strcpy_s(ObjHeader.ModelPath, sizeof(ObjHeader.ModelPath), CGameInstance::GetInstance()->Get_ResourceMgr()->Get_ResourcePath(ModelName).c_str());
	strcpy_s(ObjHeader.MaterialName, sizeof(ObjHeader.MaterialName), MaterialName.c_str());
	strcpy_s(ObjHeader.MaterialPath, sizeof(ObjHeader.MaterialPath), CGameInstance::GetInstance()->Get_ResourceMgr()->Get_ResourcePath(MaterialName).c_str());

	ofs.write(reinterpret_cast<const char*>(&ObjHeader), sizeof(MAP_OBJECT_HEADER));
	return S_OK;
}


void CTileObject::Render_GUI()
{
	__super::Render_GUI();

}

CTileObject* CTileObject::Create()
{
	CTileObject* instance = new CTileObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CTileObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CTileObject::Clone(INIT_DESC* pArg)
{
	CTileObject* instance = new CTileObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CTileObject");
		Safe_Release(instance);
	}

	return instance;
}

void CTileObject::Free()
{
	__super::Free();
}
