#include "GameObject.h"
#include "GameInstance.h"
#include "Builder.h"
#include "IRenderService.h"
#include "StaticModel.h"
#include "AnimatedModel.h"
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
	CModel* model = { nullptr };
	_bool Skinning = false;
	if (Get_Component<CStaticModel>()) {
		model = Get_Component<CStaticModel>();
		Skinning = false;
	}
	else if(Get_Component<CAnimatedModel>()) {
		model = Get_Component<CAnimatedModel>();
		Skinning = true;
	}
	else {
		model = Get_Component<CModel>();
	}
	if (!model) return;

	for (size_t i = 0; i < model->Get_MeshCount(); i++)
	{
		if (!model->isDrawable(i)) continue;

		OPAQUE_PACKET packet;
		packet.bSkinning = Skinning;
		packet.pModel = model;
		packet.DrawIndex = i;
		packet.pMaterial = Get_Component<CMaterial>();
		packet.pWorldMatrix = m_pTransform->Get_WorldMatrix();
		CGameInstance::GetInstance()->Get_RenderSystem()->Submit_Opaque(packet);
	}
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

_float4 CGameObject::Get_Position()
{
	_float4 pos;
	XMStoreFloat4(&pos, m_pTransform->Get_Pos());
	return pos;
}

void CGameObject::Free()
{
	__super::Free();
	for (auto& pair : m_Components)
		Safe_Release(pair.second);

	Safe_Release(m_pTransform);
}
