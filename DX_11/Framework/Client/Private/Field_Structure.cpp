#include "Client_Defines.h"
#include "Field_Structure.h"
#include "SkeletalModel.h"
#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"

#include "GameInstance.h"
#include "AABB_Collider.h"
CField_Structure::CField_Structure()
{
}

CField_Structure::CField_Structure(const CField_Structure& rhs)
	:CFieldObject(rhs)
{
}

HRESULT CField_Structure::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	Add_Component<CAABB_Collider>();
	return S_OK;
}

HRESULT CField_Structure::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CField_Structure::Priority_Update(_float dt)
{
}

void CField_Structure::Update(_float dt)
{
}

void CField_Structure::Late_Update(_float dt)
{
}

void CField_Structure::Render_GUI()
{
	__super::Render_GUI();
	ImGui::Text("Type : %d", m_iObjType);
}

void CField_Structure::OnCollisionEnter(COLLISION_CONTEXT ctx)
{
	if (ctx.Owner->Has_Tag("Player")) {
		int i = 0;
	}
}

HRESULT CField_Structure::Sync_MapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{

	HRESULT hr = Get_Component<CModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	m_iObjType = objHeader.Object_type;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));

	if (FAILED(hr)) {
		int i = 0;
	}

	if (CSkeletalModel* pSkel = Get_Component<CSkeletalModel>()) {
		Get_Component<CSkeletalModel>()->Hide_MehsByName("mWinterSnow-mesh");
		Get_Component<CSkeletalModel>()->Hide_MehsByName("mWinterSnow");
		Get_Component<CSkeletalModel>()->Hide_MehsByName("Snow_1__mWinterSnow");
		Get_Component<CSkeletalModel>()->Hide_MehsByName("Snow_1__mWinterSnow-mesh");
	}
	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance())
	{
		instance->Override_Pass("Structure");
	}

	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	auto Box = Get_Component<CModel>()->Get_WorldBoundingBox();
	auto Indecies = tileSystem->Get_IndeciesByArea(
		{ Box.vMin.x,Box.vMin.y ,Box.vMin.z,1.f },
		{ Box.vMax.x,Box.vMax.y ,Box.vMax.z,1.f });

	for (auto idx : Indecies)
	{
		tileSystem->Add_TileFlagByIndex(idx, static_cast<_uint>(
			TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_STRUCTURE));
	}

	return hr;
}

CField_Structure* CField_Structure::Create()
{
	CField_Structure* instance = new CField_Structure();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CField_Structure");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CField_Structure::Clone(INIT_DESC* pArg)
{
	CField_Structure* instance = new CField_Structure(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CField_Structure");
		Safe_Release(instance);
	}

	return instance;
}

void CField_Structure::Free()
{
	__super::Free();
}