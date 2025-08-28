#include "GameObject.h"
#include "GameInstance.h"
#include "Builder.h"

CGameObject::CGameObject()
{
}

CGameObject::CGameObject(const CGameObject& rhs)
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

	for (auto& pair : obj->CompDesc) {
		auto iter = m_Components.find(pair.first);

		if (iter == m_Components.end())
			MSG_BOX("Wrong Description Imported");
		else
			iter->second->Initialize(pair.second);
	}

	m_InstanceName = obj->InstanceName;
	return S_OK;
}


void CGameObject::Free()
{
	__super::Free();
	for (auto& pair : m_Components)
		Safe_Release(pair.second);

	Safe_Release(m_pTransform);
}
