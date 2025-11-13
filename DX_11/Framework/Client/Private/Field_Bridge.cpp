#include "Client_Defines.h"
#include "Field_Bridge.h"
#include "SkeletalModel.h"
#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "GameInstance.h"

CField_Bridge::CField_Bridge()
{
}

CField_Bridge::CField_Bridge(const CField_Bridge& rhs)
	:CFieldObject(rhs)
{
}

HRESULT CField_Bridge::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CField_Bridge::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CField_Bridge::Priority_Update(_float dt)
{
}

void CField_Bridge::Update(_float dt)
{
}

void CField_Bridge::Late_Update(_float dt)
{
}

void CField_Bridge::Render_GUI()
{
	__super::Render_GUI();
}

HRESULT CField_Bridge::Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{

	HRESULT hr = Get_Component<CModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	m_iObjType = objHeader.Object_type;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));
	Get_Component<CModel>()->SetDrawable(1, false);

	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	auto Box = Get_Component<CModel>()->Get_WorldBoundingBox();

	auto Indecies = tileSystem->Get_IndeciesByArea(
		{ Box.vMin.x,Box.vMin.y ,Box.vMin.z,1.f },
		{ Box.vMax.x,Box.vMax.y ,Box.vMax.z,1.f });

	for (auto idx : Indecies)
	{
		tileSystem->Add_TileFlagByIndex(idx, static_cast<_uint>(TILE_FLAG::FLAG_BRIDGE));
		tileSystem->Set_Material_ID(idx, { 1,1,0,0 });
	}

	return hr;
}

CField_Bridge* CField_Bridge::Create()
{
	CField_Bridge* instance = new CField_Bridge();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CField_Bridge");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CField_Bridge::Clone(INIT_DESC* pArg)
{
	CField_Bridge* instance = new CField_Bridge(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CField_Bridge");
		Safe_Release(instance);
	}

	return instance;
}

void CField_Bridge::Free()
{
	__super::Free();
}