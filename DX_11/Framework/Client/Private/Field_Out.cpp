#include "Client_Defines.h"
#include "Field_Out.h"

#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"

CField_Out::CField_Out()
{
}

CField_Out::CField_Out(const CField_Out& rhs)
	:CFieldObject(rhs)
{
}


HRESULT CField_Out::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CField_Out::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	//HRESULT hr = Get_Component<CStaticModel>()->Link_Model(pDesc->LevelTag,pDesc->ModelName);
	//hr= Get_Component<CMaterial>()->Link_Material(pDesc->LevelTag,pDesc->MaterialName);

	//if (SUCCEEDED(hr)) {
	//	Override_Pass();
	//}
    return S_OK;
}

void CField_Out::Priority_Update(_float dt)
{
}

void CField_Out::Update(_float dt)
{
}

void CField_Out::Late_Update(_float dt)
{
}

void CField_Out::Render_GUI()
{
	__super::Render_GUI();
}

HRESULT CField_Out::Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{
	// iter->first ID
		// iter->second  name , modelID, MaterialID, path,path

	HRESULT hr = Get_Component<CStaticModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	hr= Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	m_iObjType = objHeader.Object_type;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));
	if (SUCCEEDED(hr)) {
		Override_Pass();
	}
	return S_OK;
}

void CField_Out::Override_Pass()
{
	CMaterial* pMaterial = Get_Component<CMaterial>();

	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassRiverXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mWaterfall")) {
		instance->Override_Pass("Water");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
		instance->Override_Pass("Base");
	}
}


CField_Out* CField_Out::Create()
{
	CField_Out* instance = new CField_Out();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CField_Out");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CField_Out::Clone(INIT_DESC* pArg)
{
	CField_Out* instance = new CField_Out(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CField_Out");
		Safe_Release(instance);
	}

	return instance;
}

void CField_Out::Free()
{
	__super::Free();
}
