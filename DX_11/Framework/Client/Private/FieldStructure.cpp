#include "Client_Defines.h"
#include "FieldStructure.h"

#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"

CFieldStructure::CFieldStructure()
{
}

CFieldStructure::CFieldStructure(const CFieldStructure& rhs)
    :CGameObject(rhs)
{
}

HRESULT CFieldStructure::Initialize_Prototype()
{
	__super::Initialize();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CFieldStructure::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	FIELDSTR_DESC* pDesc = static_cast<FIELDSTR_DESC*> (pArg);

	HRESULT hr = Get_Component<CStaticModel>()->Link_Model(pDesc->LevelTag, pDesc->ModelName);
	hr = Get_Component<CMaterial>()->Link_Material(pDesc->LevelTag, pDesc->MaterialName);

	return hr;
}

void CFieldStructure::Priority_Update(_float dt)
{
}

void CFieldStructure::Update(_float dt)
{
}

void CFieldStructure::Late_Update(_float dt)
{
}

void CFieldStructure::Render_GUI()
{
	__super::Render_GUI();
}

CFieldStructure* CFieldStructure::Create()
{
	CFieldStructure* instance = new CFieldStructure();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFieldStructure");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CFieldStructure::Clone(INIT_DESC* pArg)
{
	CFieldStructure* instance = new CFieldStructure(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CFieldStructure");
		Safe_Release(instance);
	}

	return instance;
}

void CFieldStructure::Free()
{
	__super::Free();
}