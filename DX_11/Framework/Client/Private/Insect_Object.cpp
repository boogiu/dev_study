#include "Client_Defines.h"
#include "Insect_Object.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "Animator3D.h"

#include "AABB_Collider.h"

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
	return S_OK;
}

HRESULT CInsect_Object::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	HRESULT hr = Add_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", "InsectAkiakane.model");
	hr = Add_Component<CMaterial>()->Link_Material("GamePlay_Level", "InsectAkiakane.mat");
	Add_Component<CAABB_Collider>()->Make_MinMaxCollider(Get_Component<CSkeletalModel>()->Get_LocalBoundingBox());

	Add_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", "InsectAkiakane.model");
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Fly.anim", "Akiakane", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Capture.anim", "Akiakane", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "FlyWait.anim", "Akiakane", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Get.anim", "Akiakane", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Wait.anim", "Akiakane", true);
	Get_Component<CAnimator3D>()->Change_Animation("Fly.anim");

	m_vPivotPos = Get_Position();
	return S_OK;
}

void CInsect_Object::Priority_Update(_float dt)
{
}

void CInsect_Object::Update(_float dt)
{
	Get_Component<CAnimator3D>()->Update_Animation(dt);
}

void CInsect_Object::Late_Update(_float dt)
{
	m_fLifeTime += dt;

	MoveFly(dt);
}

void CInsect_Object::Render_GUI()
{
	__super::Render_GUI();
}

void CInsect_Object::OnCollisionEnter(COLLISION_CONTEXT context)
{
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
	_float MoveSpeed = { 7.f }; /*움직임 속도*/
	_float RotSpeed = { 30.f }; /*움직임 속도*/

	/*좌우 움직임 -> 현재 로컬 rigth 방향으로 사인 파*/
	_float Ampitude = { 3 }; /*좌우 반복 속도*/
	_vector RightVector = m_pTransform->Dir(STATE::RIGHT) * MoveSpeed;
	m_pTransform->Translate(RightVector * dt * sinf(m_fLifeTime * Ampitude / XM_PI));

	/*그리고 앞으로 조금씩 나아가줌*/
	m_pTransform->Translate(LookVector * dt * MoveSpeed);
	m_pTransform->Translate({ 0,dt,0 });
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
