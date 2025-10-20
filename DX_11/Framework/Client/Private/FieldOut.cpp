#include "Client_Defines.h"
#include "FieldOut.h"

#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"

CFieldOut::CFieldOut()
{
}

CFieldOut::CFieldOut(const CFieldOut& rhs)
	:CGameObject(rhs)
{
}


HRESULT CFieldOut::Initialize_Prototype()
{
	__super::Initialize();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CFieldOut::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	FIELDOUT_DESC * pDesc =static_cast<FIELDOUT_DESC*> (pArg);

	HRESULT hr = Get_Component<CStaticModel>()->Link_Model(pDesc->LevelTag,pDesc->ModelName);
	hr= Get_Component<CMaterial>()->Link_Material(pDesc->LevelTag,pDesc->MaterialName);

	if (SUCCEEDED(hr)) {
		Override_Pass();
	}
    return hr;
}

void CFieldOut::Priority_Update(_float dt)
{
}

void CFieldOut::Update(_float dt)
{
}

void CFieldOut::Late_Update(_float dt)
{
}

void CFieldOut::Render_GUI()
{
	__super::Render_GUI();
}

void CFieldOut::Override_Pass()
{
	CMaterial* pMaterial = Get_Component<CMaterial>();

	//if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassXlu")) {
	//	instance->Override_Pass("Edge");
	//}
	//if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassRiverXlu")) {
	//	instance->Override_Pass("Edge");
	//}
	//if (auto instance = pMaterial->Get_MaterialInstanceByName("mWaterfall")) {
	//	instance->Override_Pass("Water");
	//}
	//if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
	//	instance->Override_Pass("Base");
	//}
}

CFieldOut* CFieldOut::Create()
{
	CFieldOut* instance = new CFieldOut();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFieldOut");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CFieldOut::Clone(INIT_DESC* pArg)
{
	CFieldOut* instance = new CFieldOut(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CFieldOut");
		Safe_Release(instance);
	}

	return instance;
}

void CFieldOut::Free()
{
	__super::Free();
}
