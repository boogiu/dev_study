#include "Client_Defines.h"
#include "Item_Fruit.h"

#include "StaticModel.h"
#include "Material.h"
#include "AABB_Collider.h"

#include "BoneFollower.h"

#include "Child.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "ITileService.h"

#include "IObjectService.h"


CItem_Fruit::CItem_Fruit()
{
}

CItem_Fruit::CItem_Fruit(const CItem_Fruit& rhs)
	:CItem_Object(rhs)
{
}

HRESULT CItem_Fruit::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CBoneFollower>();
	return S_OK;
}

HRESULT CItem_Fruit::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CItem_Fruit::Priority_Update(_float dt)
{
}

void CItem_Fruit::Update(_float dt)
{
	if (IsDangled)
	{
		if (Get_Component<CBoneFollower>()->HasOwner())
			Get_Component<CBoneFollower>()->Sync_Transform(dt, m_pTransform);
	}
	else
		Update_ByState(dt);
}

void CItem_Fruit::Late_Update(_float dt)
{
}

void CItem_Fruit::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		if (context.EventTag == "Pick_Up") {
			m_eState = PICKED;
			m_pOwnerMatrix = context.Owner->Get_WorldMatrix();
			Get_Component<CCollider>()->Set_ContextEvent("Picked");
		}
	}
}

void CItem_Fruit::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CItem_Fruit::OnCollisionExit(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		m_eState = READY_DESTROY;
	}
}

void CItem_Fruit::Render_GUI()
{
	__super::Render_GUI();
}
void CItem_Fruit::Dangle_Fruit(const string& boneName, _float3 offset)
{
	m_Offset = offset;
	IsDangled = true;
	CGameObject* pObj = Get_Component<CChild>()->Get_Parent();
	Get_Component<CBoneFollower>()->Link_Bone(pObj->Get_Component<CAnimator3D>(), boneName);
	Get_Component<CBoneFollower>()->Set_Offset(XMMatrixTranslation(offset.x, offset.y, offset.z));
}

void CItem_Fruit::Update_ByState(_float dt)
{
	switch (m_eState) {
	case DROP: {
		m_pTransform->Translate({ 0,-dt * 35,0 });
		if (Get_Position().y <= m_MarginY) {
			Find_Ground();
		}
	}
			 break;
	case  BOUND:
		MoveToIndex(dt);
		break;
	case  PICKED:
		FollowHand(dt);
		break;
	case  READY_DESTROY: {
		CGameInstance::GetInstance()->Get_ObjectMgr()->Remove_Object(this);
	}
				 m_eState = IDLE;
				 break;
	}
}

CItem_Fruit* CItem_Fruit::Create()
{
	CItem_Fruit* instance = new CItem_Fruit();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CPlant_Fruit");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CItem_Fruit::Clone(INIT_DESC* pArg)
{
	CItem_Fruit* instance = new CItem_Fruit(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPlant_Fruit");
		Safe_Release(instance);
	}

	return instance;
}

void CItem_Fruit::Free()
{
	__super::Free();
}
