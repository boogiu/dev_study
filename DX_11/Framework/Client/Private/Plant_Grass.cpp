#include "Client_Defines.h"
#include "Plant_Grass.h"
#include "SkeletalModel.h"
#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "GameInstance.h"
CPlant_Grass::CPlant_Grass()
{
}

CPlant_Grass::CPlant_Grass(const CPlant_Grass& rhs)
    :CFieldObject(rhs)
{
}

HRESULT CPlant_Grass::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CPlant_Grass::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CPlant_Grass::Priority_Update(_float dt)
{
}

void CPlant_Grass::Update(_float dt)
{
}

void CPlant_Grass::Late_Update(_float dt)
{
}

void CPlant_Grass::Render_GUI()
{
	__super::Render_GUI();
}

HRESULT CPlant_Grass::Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{

	HRESULT hr = Get_Component<CModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	m_iObjType = objHeader.Object_type;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));

	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_Index = tileSystem->Get_IndexByPosition(Get_Position());

	tileSystem->Add_TileFlagByIndex(objHeader.Index, static_cast<_uint>(TILE_FLAG::FLAG_GRASS));
	tileSystem->Set_Material_ID(objHeader.Index, { 1,1,0,0 });
	return hr;
}

CPlant_Grass* CPlant_Grass::Create()
{
	CPlant_Grass* instance = new CPlant_Grass();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CPlant_Grass");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CPlant_Grass::Clone(INIT_DESC* pArg)
{
	CPlant_Grass* instance = new CPlant_Grass(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPlant_Grass");
		Safe_Release(instance);
	}

	return instance;
}

void CPlant_Grass::Free()
{
	__super::Free();
}