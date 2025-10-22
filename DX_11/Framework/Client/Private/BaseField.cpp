#include "BaseField.h"
#include "Client_Defines.h"

#include "StaticModel.h"
#include "InstanceModel.h"

#include "Material.h"
#include "MaterialInstance.h"

#include "GameInstance.h"
#include "ITileService.h"
#include "ObjectMgr.h"
#include "IResourceService.h"
#include "Builder.h"

#include "Texture.h"
#include "ObjectContainer.h"
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
	Add_Component<CInstanceModel>();
	Add_Component<CMaterial>();

	return S_OK;
}

HRESULT CBaseField::Initialize(INIT_DESC * pArg)
{
	__super::Initialize(pArg);
	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();

	CInstanceModel::INSTANCE_INIT_DESC instanceDesc = {};
	instanceDesc.ElementCount = 5;
	instanceDesc.ElementKey = "Tile_Instancing";
	instanceDesc.instanceCount = 16000;
	instanceDesc.instanceStride = sizeof(INSTANCE_TILE);
	instanceDesc.pElementDesc = VTX_TILEINSTANCE::Elements;

	vector<CInstanceModel::INSTANCE_INIT_DESC> pVector;
	pVector.push_back(instanceDesc);

	CMaterial* pMaterial = Get_Component<CMaterial>();
	pMaterial->Link_Material("GamePlay_Level", "Base_0.mat");
	for (auto& instance : pMaterial->Get_Material_Instance()) {
		instance->Override_Pass("Instancing");
	}
	Get_Component<CInstanceModel>()->Link_InstanceData(pDevice, pVector, "GamePlay_Level", "Base_0.model");
	Get_Component<CInstanceModel>()->Link_InstanceWithMesh(0, 0);
	Get_Component<CInstanceModel>()->Link_InstanceWithMesh(1, 0);

	m_Tiles.reserve(16000);

	return S_OK;
}

void CBaseField::Priority_Update(_float dt)
{
}

void CBaseField::Update(_float dt)
{
	ID3D11DeviceContext* pContext = CGameInstance::GetInstance()->Get_Context();
	Get_Component<CInstanceModel>()->Update_Instance(pContext, m_Tiles.data(), 0, static_cast<_uint>(m_Tiles.size()));
}

void CBaseField::Late_Update(_float dt)
{

}

void CBaseField::Load_BaseTile(ifstream& ifs, _uint Count)
{
	for (size_t i = 0; i < Count; i++)
	{
		INSTANCE_TILE tileInfo = {};
		ifs.read(reinterpret_cast<char*>(&tileInfo), sizeof(tileInfo));
		Add_Tile(tileInfo);
	}
}

void CBaseField::Render_GUI()
{
	__super::Render_GUI();
}
void CBaseField::Add_Tile(INSTANCE_TILE tile)
{
	m_Tiles.push_back(tile);
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
