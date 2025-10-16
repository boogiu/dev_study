#include "BaseField.h"
#include "Client_Defines.h"

#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"

#include "GameInstance.h"
#include "ITileService.h"

CBaseField::CBaseField()
{
}

CBaseField::CBaseField(const CBaseField& rhs)
	:CGameObject(rhs)
{
}

HRESULT CBaseField::Initialize_Prototype()
{
	__super::Initialize();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CBaseField::Initialize(INIT_DESC * pArg)
{
	__super::Initialize(pArg);
	BASEFIELD_DESC* pDesc = static_cast<BASEFIELD_DESC*> (pArg);

	HRESULT hr = Get_Component<CStaticModel>()->Link_Model(pDesc->LevelTag, pDesc->ModelName);
	hr = Get_Component<CMaterial>()->Link_Material(pDesc->LevelTag, pDesc->MaterialName);

	if (SUCCEEDED(hr)) {
		Override_Pass();
	}

	auto TileService = CGameInstance::GetInstance()->Get_TileSystem();

	BOUNDING_BOX box =Get_Component<CModel>()->Get_WorldBoundingBox();
	auto IndexVector = TileService->Get_IndeciesByArea(
		{box.vMin.x, box.vMin.y, box.vMin.z,1.f},
		{ box.vMax.x, box.vMax.y, box.vMax.z,1.f }
	);

	TileService->Add_TileFlagByIndex(IndexVector, static_cast<_uint>(TILE_FLAG::WALKABLE));

	return hr;
}

void CBaseField::Priority_Update(_float dt)
{
}

void CBaseField::Update(_float dt)
{
}

void CBaseField::Late_Update(_float dt)
{
}

void CBaseField::Render_GUI()
{
	__super::Render_GUI();
}

void CBaseField::Override_Pass()
{
	CMaterial* pMaterial = Get_Component<CMaterial>();
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
		instance->Override_Pass("Base");
	}
}

CBaseField* CBaseField::Create()
{
	CBaseField* instance = new CBaseField();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CBaseField");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CBaseField::Clone(INIT_DESC* pArg)
{
	CBaseField* instance = new CBaseField(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CBaseField");
		Safe_Release(instance);
	}

	return instance;
}

void CBaseField::Free()
{
	__super::Free();
}
