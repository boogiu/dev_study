#include "Client_Defines.h"
#include "Item_Stone.h"
#include "StaticModel.h"
#include "Material.h"
#include "AABB_Collider.h"

#include "BoneFollower.h"

#include "Child.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "ITileService.h"

#include "IObjectService.h"

CItem_Stone::CItem_Stone()
{
}

CItem_Stone::CItem_Stone(const CItem_Stone& rhs)
	:CItem_Object(rhs)
{
}

HRESULT CItem_Stone::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CItem_Stone::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_eState = { DROP };
	return S_OK;
}

void CItem_Stone::Priority_Update(_float dt)
{
}

void CItem_Stone::Update(_float dt)
{
	Update_ByState(dt);
}

void CItem_Stone::Late_Update(_float dt)
{
}

void CItem_Stone::Render_GUI()
{
	__super::Render_GUI();
}

void CItem_Stone::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		if (context.EventTag == "Pick_Up") {
			m_eState = PICKED;
			m_pOwnerMatrix = context.Owner->Get_WorldMatrix();
			Get_Component<CCollider>()->Set_ContextEvent("Picked");
		}
	}
}

void CItem_Stone::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CItem_Stone::OnCollisionExit(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		m_eState = READY_DESTROY;
	}
}



void CItem_Stone::Update_ByState(_float dt)
{
	switch (m_eState) {
	case DROP: {
		Find_Ground();
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
		 m_eState = IDLE;
		}
		 break;
	}
}

CItem_Stone* CItem_Stone::Create()
{
	CItem_Stone* instance = new CItem_Stone();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CItem_Stone");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CItem_Stone::Clone(INIT_DESC* pArg)
{
	CItem_Stone* instance = new CItem_Stone(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CItem_Stone");
		Safe_Release(instance);
	}

	return instance;
}

void CItem_Stone::Free()
{
	__super::Free();
}
