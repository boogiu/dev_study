#include "Client_Defines.h"
#include "Furniture.h"

#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "GameInstance.h"
#include "AABB_Collider.h"
#include "DebugRender.h"
#include "Light.h"


CFurniture::CFurniture()
{
}

CFurniture::CFurniture(const CFurniture& rhs)
    :CGameObject(rhs)
{
}

HRESULT CFurniture::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	Add_Component<CAABB_Collider>();
	Add_Component<CDebugRender>();
	return S_OK;
}

HRESULT CFurniture::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CAABB_Collider>()->Set_CompActive(false);
	return S_OK;
}

void CFurniture::Priority_Update(_float dt)
{
}

void CFurniture::Update(_float dt)
{
}

void CFurniture::Late_Update(_float dt)
{
}

void CFurniture::Render_GUI()
{
	__super::Render_GUI();
}

void CFurniture::OnCollisionEnter(COLLISION_CONTEXT context)
{
}

void CFurniture::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CFurniture::OnCollisionExit(COLLISION_CONTEXT context)
{
}

HRESULT CFurniture::Make_FurnitureByMapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{
	HRESULT hr = Get_Component<CModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));

	 if (CStaticModel* pSkel = Get_Component<CStaticModel>()) {
		Get_Component<CStaticModel>()->Hide_MehsByName("mWinterSnow-mesh");
		Get_Component<CStaticModel>()->Hide_MehsByName("mWinterSnow");
		Get_Component<CStaticModel>()->Hide_MehsByName("Snow_1__mWinterSnow");
		Get_Component<CStaticModel>()->Hide_MehsByName("Snow_1__mWinterSnow-mesh");
		Get_Component<CStaticModel>()->ShadowCast(true);
	}
	 Get_Component<CDebugRender>()->Add_DebugBounding(Get_Component<CStaticModel>()->Get_LocalBoundingBox());

	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_Index = tileSystem->Get_IndexByPosition(Get_Position());

	tileSystem->Add_TileFlagByIndex(objHeader.Index, static_cast<_uint>(
		TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_ONFURNITURE));

	tileSystem->Set_Material_ID(objHeader.Index, { 1,1,0,0 });

	m_InstanceTag = modelMapTable[5];
	if (m_InstanceTag == "WorkBench") {
		Add_Component<CAABB_Collider>()->Make_MinMaxCollider(Get_Component<CModel>()->Get_LocalBoundingBox());
		Get_Component<CAABB_Collider>()->Set_ColliderActive(true);
	}
	string data = objHeader.AdditionalData;

	if (data == "Light") {
		LIGHT_DESC desc = {};
		desc.fLightRange = 20.0f;
		desc.vLightDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		desc.vLightAmbient = _float4(0.35f, 0.35f, 0.35f, 1.f);
		desc.vLightSpecular = _float4(0.2f, 0.2f, 0.2f, 1.f);
		Add_Component<CLight>()->Set_Desc(desc, LIGHT_TYPE::POINT);
	}
	return hr;
}

CFurniture* CFurniture::Create()
{
	CFurniture* instance = new CFurniture();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFurniture");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CFurniture::Clone(INIT_DESC* pArg)
{
	CFurniture* instance = new CFurniture(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CFurniture");
		Safe_Release(instance);
	}

	return instance;
}

void CFurniture::Free()
{
	__super::Free();
}
