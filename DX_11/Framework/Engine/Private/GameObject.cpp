#include "GameObject.h"
#include "GameInstance.h"
#include "Builder.h"
#include "IRenderService.h"
#include "Model.h"
#include "Material.h"

_uint CGameObject::s_NextID = 1;

CGameObject::CGameObject()
	:m_ObjectID(s_NextID++)
{
}

CGameObject::CGameObject(const CGameObject& rhs)
	:m_ObjectID(s_NextID++)
{
	for (auto& pair : rhs.m_Components) {
		CComponent* comp = pair.second->Clone();
		comp->Set_Owner(this);
		m_Components.emplace(pair.first, comp);
	}

	m_pTransform = Get_Component<CTransform>();
	Safe_AddRef(m_pTransform);
}

HRESULT CGameObject::Initialize_Prototype()
{
	//원본 생성 시, 필요 초기화들 진행
	m_pTransform = Add_Component<CTransform>();

	return S_OK;
}

HRESULT CGameObject::Initialize(INIT_DESC* pArg)
{
	if (pArg == nullptr)
		return S_OK;

	GAMEOBJECT_DESC* obj = static_cast<GAMEOBJECT_DESC*>(pArg);

	//for (auto& pair : obj->CompDesc) {
	//	auto iter = m_Components.find(pair.first);
	//
	//	if (iter == m_Components.end())
	//		MSG_BOX("Wrong Description Imported");
	//	else
	//		iter->second->Initialize(pair.second);
	//}
	//
	//
	for (auto& pair : m_Components)
	{
		auto iter = obj->CompDesc.find(pair.first);

		if (iter == obj->CompDesc.end())
			pair.second->Initialize(nullptr);
		else
			pair.second->Initialize(iter->second);
	}

	m_InstanceName = obj->InstanceName;
	return S_OK;
}

void CGameObject::Engine_Update(_float dt)
{
	OPAQUE_PACKET packet;
	packet.pModel = Get_Component<CModel>();
	packet.pMaterial = Get_Component<CMaterial>();
	packet.pWorldMatrix = m_pTransform->Get_WorldMatrix();

	CGameInstance::GetInstance()->Get_RenderSystem()->Submit_Opaque(packet);
}

void CGameObject::Render_GUI()
{
	m_pTransform->Render_GUI();
	for (auto& pair : m_Components) {
		if (pair.first == type_index(typeid(CTransform))) continue;
		pair.second->Render_GUI();
	}
}

_float4x4* CGameObject::Get_WorldMatrix()
{
	return m_pTransform->Get_WorldMatrix();
}

void CGameObject::Free()
{
	__super::Free();
	for (auto& pair : m_Components)
		Safe_Release(pair.second);

	Safe_Release(m_pTransform);
}
