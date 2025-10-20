#include "BaseField.h"
#include "Client_Defines.h"

#include "StaticModel.h"
#include "InstanceModel.h"

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
	//Add_Component<CStaticModel>();
	Add_Component<CInstanceModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CBaseField::Initialize(INIT_DESC * pArg)
{
	__super::Initialize(pArg);
	BASEFIELD_DESC* pDesc = static_cast<BASEFIELD_DESC*> (pArg);
	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();

	CInstanceModel::INSTANCE_INIT_DESC instanceDesc = {};
	instanceDesc.ElementCount = 4;
	instanceDesc.ElementKey = "GrassInstance";
	instanceDesc.instanceCount = 10000;
	instanceDesc.instanceStride = sizeof(INSTANCE_GRASS);
	instanceDesc.pElementDesc =  VTX_TILEGRASS::Elements;

	vector<CInstanceModel::INSTANCE_INIT_DESC> pVector;
	pVector.push_back(instanceDesc);
	Get_Component<CMaterial>()->Link_Material(pDesc->LevelTag, pDesc->MaterialName);
	Get_Component<CInstanceModel>()->Link_InstanceData(pDevice, pVector, pDesc->LevelTag, pDesc->ModelName);
	Get_Component<CInstanceModel>()->Link_InstanceWithMesh(0,0);
	Get_Component<CInstanceModel>()->Link_InstanceWithMesh(1,0);

	Override_Pass();
	m_Grasses.reserve(10000);
	return S_OK;
}

void CBaseField::Priority_Update(_float dt)
{
}
void CBaseField::Update(_float dt)
{
	const _uint gridSizeX = 100;   // 가로 개수
	const _uint gridSizeZ = 100;   // 세로 개수
	const _float spacing = 10.f;  // 간격

	// 인스턴스 총 개수 보장
	m_Grasses.resize(gridSizeX * gridSizeZ);

	for (_uint z = 0; z < gridSizeZ; ++z)
	{
		for (_uint x = 0; x < gridSizeX; ++x)
		{
			_uint idx = z * gridSizeX + x;

			INSTANCE_GRASS& grass = m_Grasses[idx];
			grass.vRight = { 1.f, 0.f, 0.f, 0.f };
			grass.vUp = { 0.f, 1.f, 0.f, 0.f };
			grass.vLook = { 0.f, 0.f, 1.f, 0.f };
			grass.vTranslation = { x * spacing, 0.f, z * spacing, 1.f };
		}
	}

	ID3D11DeviceContext* pContext = CGameInstance::GetInstance()->Get_Context();
	Get_Component<CInstanceModel>()->Update_Instance(pContext, m_Grasses.data(), 0, static_cast<_uint>(m_Grasses.size()));
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
	for (auto& instance : pMaterial->Get_Material_Instance()) {
		instance->Override_Pass("Instancing");
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
