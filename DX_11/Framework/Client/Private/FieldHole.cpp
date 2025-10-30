#include "Client_Defines.h"
#include "FieldHole.h"
#include "StaticModel.h"
#include "Material.h"
#include "AABB_Collider.h"

#include "GameInstance.h"

CFieldHole::CFieldHole()
{
}

CFieldHole::CFieldHole(const CFieldHole& rhs)
    :CGameObject(rhs)
{
	m_InstanceTag = "Hole";
}

HRESULT CFieldHole::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	HRESULT hr = Add_Component<CStaticModel>()->Link_Model("GamePlay_Level", "UnitIconHoleOn.model");
	hr = Add_Component<CMaterial>()->Link_Material("GamePlay_Level", "UnitIconHoleOn.mat");
	Add_Component<CAABB_Collider>();

	return S_OK;
}

HRESULT CFieldHole::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	TILE_INDEX index= CGameInstance::GetInstance()->Get_TileSystem()->Get_IndexByPosition(Get_Position());
	CGameInstance::GetInstance()->Get_TileSystem()->Add_TileFlagByIndex(index,static_cast<_uint>(TILE_FLAG::FLAG_DIGGED | TILE_FLAG::FLAG_BLOCKED));
	return S_OK;
}

void CFieldHole::Priority_Update(_float dt)
{
}

void CFieldHole::Update(_float dt)
{
}

void CFieldHole::Late_Update(_float dt)
{
}

void CFieldHole::Render_GUI()
{
	__super::Render_GUI();
}

CGameObject* CFieldHole::Clone(INIT_DESC* pArg)
{
	CFieldHole* instance = new CFieldHole(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CFieldHole");
		Safe_Release(instance);
	}

	return instance;
}

CFieldHole* CFieldHole::Create()
{
	CFieldHole* instance = new CFieldHole();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFieldHole");
		Safe_Release(instance);
	}

	return instance;
}

void CFieldHole::Free()
{
	__super::Free();
}
