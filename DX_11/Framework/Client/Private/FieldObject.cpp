#include "Client_Defines.h"
#include "FieldObject.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
CFieldObject::CFieldObject()
{
}

CFieldObject::CFieldObject(const CFieldObject& rhs)
{
}

HRESULT CFieldObject::Initialize_Prototype()
{
	__super::Initialize();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CFieldObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	//HRESULT hr = Get_Component<CStaticModel>()->Link_Model(pDesc->LevelTag, pDesc->ModelName);
	//hr = Get_Component<CMaterial>()->Link_Material(pDesc->LevelTag, pDesc->MaterialName);

	return S_OK;
}

void CFieldObject::Priority_Update(_float dt)
{
}

void CFieldObject::Update(_float dt)
{
}

void CFieldObject::Late_Update(_float dt)
{
}

void CFieldObject::Render_GUI()
{
}

CFieldObject* CFieldObject::Create()
{
	CFieldObject* instance = new CFieldObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFieldObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CFieldObject::Clone(INIT_DESC* pArg)
{
	CFieldObject* instance = new CFieldObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CFieldObject");
		Safe_Release(instance);
	}

	return instance;
}

void CFieldObject::Free()
{
	__super::Free();
}
