#include"pch.h"
#include "Engine_Define.h"
#include "CTestObj.h"
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CRigidBody.h"
#include "CInputMgr.h"

CTestObj::CTestObj()
	:m_pTransform(nullptr),m_pRenderer(nullptr)
{
}

CTestObj::~CTestObj()
{
}

CTestObj* CTestObj::Create()
{
	CTestObj* instance = new CTestObj;
	if (FAILED(instance->Ready_GameObject())) {
		Engine::Safe_Release(instance);
		instance = nullptr;
	}
	return instance;
}

HRESULT CTestObj::Ready_GameObject()
{
	m_pTransform = Add_Component<CTransform>();
	if (m_pTransform == nullptr)
		return E_FAIL;

	m_pRenderer = Add_Component<CMeshRenderer>();
	if (m_pRenderer == nullptr)
		return E_FAIL;

	m_pRenderer->Set_Transform();
	m_pRenderer->Set_Mesh(L"tiger");

	m_pTransform->Set_Pos({0.f,0.f,1.f});
	m_pTransform->Set_Scale({2.f,2.f,2.f});

	m_pRigid = Add_Component<CRigidBody>(CRigidBody::Rigid_State::KINEMATIC);
	m_pRigid->Set_Transform();
	return S_OK;
}

void CTestObj::Update_GameObject(_float&dt)
{
	__super::Update_Component(dt);
	Key_Check(dt);
}

void CTestObj::LateUpdate_GameObject(_float&dt)
{
	__super::LateUpdate_Component(dt);

}
void CTestObj::Key_Check(float dt)
{

	const float speed = 20.f;

	if (CInputMgr::GetInstance()->Key_Down(DIK_R))
		m_pRigid->Set_State(CRigidBody::Rigid_State::DYNAMIC);
	
	if (CInputMgr::GetInstance()->Key_Down(DIK_UP))
		m_pTransform->Add_Pos({ 0.f, 0.f, dt * speed });

	if (CInputMgr::GetInstance()->Key_Down(DIK_DOWN))
		m_pTransform->Add_Pos({ 0.f, 0.f, -dt * speed });
}

void CTestObj::Free()
{
	CGameObject::Release_Component();
}
