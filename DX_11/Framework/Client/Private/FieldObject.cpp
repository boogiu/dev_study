#include "Client_Defines.h"
#include "FieldObject.h"

#include "Model.h"
#include "Material.h"

CFieldObject::CFieldObject()
{
}

CFieldObject::CFieldObject(const CFieldObject& rhs)
	:CGameObject(rhs)
{
}

HRESULT CFieldObject::Initialize_Prototype()
{
	__super::Initialize();
	return S_OK;
}

HRESULT CFieldObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CFieldObject::Awake()
{
	if (CModel* pModel = Get_Component<CModel>()) {
		pModel->ShadowCast(true);
	}
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
	__super::Render_GUI();
}

HRESULT CFieldObject::Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{
	HRESULT hr = Get_Component<CModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	m_iObjType = objHeader.Object_type;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));

	return S_OK;
}

void CFieldObject::Free()
{
	__super::Free();
}
