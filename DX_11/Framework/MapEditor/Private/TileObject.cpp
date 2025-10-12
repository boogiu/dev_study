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

}

void CTileObject::Update(_float dt)
{

}

void CTileObject::Late_Update(_float dt)
{
}

void CTileObject::Object_OnGrid(TILE_INDEX index)
{
	Get_Component<CTileBlock>()->On_Grid(index);
}

void CTileObject::Set_Selected(_bool selected)
{
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

	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassXlu")) {
		instance->Override_Pass("Edge");
	}
	return hr;
}

HRESULT CTileObject::Save_Blocks(ofstream& ofs, _bool Base)
{
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
