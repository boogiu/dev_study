#include "Client_Defines.h"
#include "FishSub_Tool.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "BoneFollower.h"
#include "Animator3D.h"
#include "AABB_Collider.h"

CFishSub_Tool::CFishSub_Tool()
{
}

CFishSub_Tool::CFishSub_Tool(const CFishSub_Tool& rhs)
	:CGameObject(rhs)
{
}

CFishSub_Tool::~CFishSub_Tool()
{
}

HRESULT CFishSub_Tool::Initialize_Prototype()
{

	__super::Initialize_Prototype();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	Add_Component<CAABB_Collider>();
	return S_OK;
}

HRESULT CFishSub_Tool::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", "Sub.model");
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "Sub.mat");
	m_InstanceTag = "Sub";
	return S_OK;
}

void CFishSub_Tool::Priority_Update(_float dt)
{

}

void CFishSub_Tool::Update(_float dt)
{
}

void CFishSub_Tool::Late_Update(_float dt)
{
}

void CFishSub_Tool::Render_GUI()
{
	__super::Render_GUI();

	XMVECTOR v = m_pTransform->Get_WorldPos(); // XMVector 반환
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, v);    // ← XMVector → float3 변환

	ImGui::Text("World Position");
	ImGui::Text("X: %.3f", pos.x);
	ImGui::Text("Y: %.3f", pos.y);
	ImGui::Text("Z: %.3f", pos.z);
}


void CFishSub_Tool::OnCollisionEnter(COLLISION_CONTEXT context)
{
}

void CFishSub_Tool::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CFishSub_Tool::OnCollisionExit(COLLISION_CONTEXT context)
{
}

CFishSub_Tool* CFishSub_Tool::Create()
{
	CFishSub_Tool* instance = new CFishSub_Tool();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFishSub_Tool");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CFishSub_Tool::Clone(INIT_DESC* pArg)
{
	CFishSub_Tool* instance = new CFishSub_Tool(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CFishSub_Tool");
		Safe_Release(instance);
	}

	return instance;
}

void CFishSub_Tool::Free()
{
	__super::Free();
}