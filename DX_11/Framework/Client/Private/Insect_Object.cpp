#include "Client_Defines.h"
#include "Insect_Object.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "Animator3D.h"

#include "AABB_Collider.h"
#include "GameInstance.h"
#include "TileSystem.h"

CInsect_Object::CInsect_Object()
{
}

CInsect_Object::CInsect_Object(const CInsect_Object& rhs)
	:CGameObject(rhs)
{
}

HRESULT CInsect_Object::Initialize_Prototype()
{
	__super::Initialize_Prototype();	
	Add_Component<CAABB_Collider>();
	return S_OK;
}

HRESULT CInsect_Object::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	InsectSpawnDesc* pDesc = static_cast<InsectSpawnDesc*>(pArg);

	HRESULT hr = Add_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", pDesc->insectDataDesc.modelName);
	hr = Add_Component<CMaterial>()->Link_Material("GamePlay_Level", pDesc->insectDataDesc.materialName);
	Add_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", pDesc->insectDataDesc.modelName);

	m_ItemDesc = pDesc->itemDataDesc;
	m_InstanceTag = "Insect";
	m_InstanceName = pDesc->insectDataDesc.InsectFileName;

	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Fly.anim", m_InstanceName, true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Capture.anim", m_InstanceName, true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "FlyWait.anim", m_InstanceName, true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Get.anim", m_InstanceName, false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Wait.anim", m_InstanceName, true);
	Get_Component<CAnimator3D>()->Change_Animation("Fly.anim");

	Get_Component<CAABB_Collider>()->Make_MinMaxCollider({ {-3,-5,-3},{3,5,3 } });
	m_fDetectRange = 30.f;
	return S_OK;
}

void CInsect_Object::Priority_Update(_float dt)
{
	auto tileSys = CGameInstance::GetInstance()->Get_TileSystem();
	m_PrevIndex = m_CurrIndex;
}

void CInsect_Object::Update(_float dt)
{
	auto tileSys = CGameInstance::GetInstance()->Get_TileSystem();
	m_fLifeTime += dt;

	switch (m_eState)
	{
	case Client::CInsect_Object::STOP:
		Check_Player();
		MoveFly(dt);
		break;
	case Client::CInsect_Object::RUN:
		Check_Player();
		MoveFly(dt);
		break;
	case Client::CInsect_Object::RUNAWAY:
		Check_Player();
		MoveRunaway(dt);
		break;

	case Client::CInsect_Object::CAPTURE: {
		Get_Component<CAnimator3D>()->Change_Animation("Capture.anim", true);
	}
	break;
	case Client::CInsect_Object::END:
		break;
	default:
		break;
	}

	m_CurrIndex = tileSys->Get_IndexByPosition(Get_Position());
	tileSys->Remove_TileFlagByIndex(m_PrevIndex, static_cast<_uint>(TILE_FLAG::FLAG_INSECT));
	tileSys->Add_TileFlagByIndex(m_CurrIndex, static_cast<_uint>(TILE_FLAG::FLAG_INSECT));

	Get_Component<CAnimator3D>()->Update_Animation(dt);
}

void CInsect_Object::Late_Update(_float dt)
{
	
}

void CInsect_Object::Render_GUI()
{
	__super::Render_GUI();
	ImGui::Text("X : %d, Z:%d", m_CurrIndex.IndexX, m_CurrIndex.IndexZ);
}

void CInsect_Object::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Net")) {
		if (context.EventTag == "NetSwing")
		{
			m_eState = CAPTURE;
		}
	}
}

void CInsect_Object::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CInsect_Object::OnCollisionExit(COLLISION_CONTEXT context)
{
}


void CInsect_Object::MoveFly(_float dt)
{
	_vector curPos = m_pTransform->Get_Pos();
	_vector LookVector = m_pTransform->Dir(STATE::LOOK);
	_float MoveSpeed = { 5.f }; /*움직임 속도*/

	if (m_fLifeTime > 8.f) {
		if (m_eState == RUN)
			m_eState = STOP;
		else
			m_eState = RUN;
	}
	
	if(m_eState == RUN)
		m_pTransform->Translate(LookVector * dt * MoveSpeed);
}

void CInsect_Object::MoveRunaway(_float dt)
{
	_vector LookAt = m_pTransform->Get_Pos() + XMLoadFloat4(&m_vMoveDir);
	_float MoveSpeed = { 20.f }; 
	m_pTransform->LookAt(LookAt);
	m_pTransform->Translate(XMLoadFloat4(&m_vMoveDir) * dt * MoveSpeed);
}

void CInsect_Object::Check_Player()
{
	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	_float4 pos = Get_Position();
	vector<TILE_INDEX> Neighbor = TileSys->Get_IndeciesByArea(
		{pos.x - m_fDetectRange ,pos.y,pos.z- m_fDetectRange,1.f },
		{pos.x + m_fDetectRange ,pos.y,pos.z+ m_fDetectRange,1.f }
	);

	for (size_t i = 0; i < Neighbor.size(); i++)
	{
		_uint Flag = TileSys->Get_TileFlagByIndex(Neighbor[i]);

		if ((TILE_FLAG::ONPLAYER & Flag) != 0) {
			_float4 PlayerPos  = TileSys->Get_PositionByIndex(Neighbor[i], ANCHOR::Center);

			_vector playerV = XMLoadFloat4(&PlayerPos);
			_vector MyPos = m_pTransform->Get_Pos();
			XMStoreFloat4(&m_vMoveDir, XMVector4Normalize(MyPos - playerV));
			m_vMoveDir.y = 0.1f;
			m_eState = RUNAWAY;
			return;
		}
	}

	m_eState = RUN;
}

CInsect_Object* CInsect_Object::Create()
{
	CInsect_Object* instance = new CInsect_Object();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CInsect_Object");
		Safe_Release(instance);
	}

	return instance;
}
CGameObject* CInsect_Object::Clone(INIT_DESC* pArg)
{
	CInsect_Object* instance = new CInsect_Object(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CInsect_Object");
		Safe_Release(instance);
	}

	return instance;
}

void CInsect_Object::Free()
{
	__super::Free();
}
