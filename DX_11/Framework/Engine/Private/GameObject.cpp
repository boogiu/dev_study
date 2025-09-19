#include "GameObject.h"
#include "GameInstance.h"
#include "Builder.h"
#include "IRenderService.h"
#include "StaticModel.h"
#include "AnimatedModel.h"
#include "Material.h"
#include "Animator3D.h"

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
	OPAQUE_PACKET packet;
	packet.pModel = { nullptr };
	packet.bSkinning = false;
	packet.pMaterial = Get_Component<CMaterial>();
	packet.pWorldMatrix = m_pTransform->Get_WorldMatrix();

	if (auto pStatic = Get_Component<CStaticModel>()) {
		packet.pModel = pStatic;
		packet.bSkinning = false;
	}
	else if (auto pAnim = Get_Component<CAnimatedModel>()) {
		packet.pModel = pAnim;
		packet.bSkinning = true;
		packet.pAnimator = Get_Component<CAnimator3D>();
		if (!packet.pAnimator) return;
	}
	else {
		return;
	}

	for (size_t i = 0; i < packet.pModel->Get_MeshCount(); i++)
	{
		if (!packet.pModel->isDrawable(i)) continue;
		packet.DrawIndex = i;
		packet.MaterialIndex = packet.pModel->Get_MaterialIndex(i);
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
