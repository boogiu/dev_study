#include "pch.h"
#include "Engine_Define.h"
#include "CTestCam.h"
#include "CGameObject.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLight.h"
#include "CMesh.h"
#include "CRenderer.h"

CTestCam::CTestCam()
	: m_pCamera(nullptr), m_pTransform(nullptr)
{
}

CTestCam::~CTestCam()
{
}

CTestCam* CTestCam::Create()
{
	CTestCam* instance = new CTestCam;

	if (FAILED(instance->Ready_GameObject())) {
		Engine::Safe_Release(instance);
		return nullptr;
	}

	return instance;
}

HRESULT CTestCam::Ready_GameObject()
{
	m_pCamera = Add_Component<CCamera>();
	if (m_pCamera == nullptr)
		return E_FAIL;

	m_pTransform = Add_Component<CTransform>();
	if (m_pTransform == nullptr)
		return E_FAIL;

	m_pTransform->Set_Pos({ 0.f,0.f,-10.f });
	m_pTransform->Set_Scale({ 15.f,15.f,15.f });

	CMesh* m_pMesh = Add_Component<CMesh>();

	if (m_pMesh == nullptr)
		return E_FAIL;
	m_pMesh->Set_MeshType(Engine::CMesh::MeshType::SKYBOX);

	CRenderer* m_pRenderer = Add_Component<CRenderer>();
	if (m_pRenderer == nullptr)
		return E_FAIL;

	CLight* light = Add_Component<CLight>(LIGHT_TYPE::POINT);

	m_pRenderer->Set_Mesh();
	light->Set_LightDesc(
		D3DXCOLOR(1.f, 1.f, 1.f, 0.0f),
		D3DXCOLOR(1.f, 1.f, 1.f, 1.0f),
		D3DXCOLOR(1.f, 1.f, 1.f, 1.0f),
		80.f,
		0.2f
	);

	return S_OK;
}

void CTestCam::Update_GameObject(float dt)
{
	__super::Update_Component(dt);
	Key_Check(dt);
}

void CTestCam::LateUpdate_GameObject(float dt)
{
	__super::LateUpdate_Component(dt);
}

void CTestCam::Set_Target(CGameObject* obj)
{
	//m_pTarget = obj;
	//CTransform* myTransform = Get_Component<CTransform>();
	//CTransform* targetTrans = m_pTarget->Get_Component<CTransform>();
	//_vec3 worldPos;
	//_vec3 targetPos = targetTrans->Get_Pos();
	//
	//D3DXVec3TransformCoord(&worldPos, &targetPos, &targetTrans->Get_WorldMatrix());
	//myTransform->Set_Pivot(worldPos);
}

void CTestCam::Key_Check(float dt)
{
	CTransform* transform = Get_Component<CTransform>();
	_vec3 pos = transform->Get_Pos();
	_vec3 look = transform->Get_Look();
	const float speed = 120.f;
	//
	//if (GetAsyncKeyState(VK_UP) & 0x8000) {
	//	pos.y += dt * speed;
	//	look.y = dt * speed;
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
	//	pos.y -= dt * speed;
	//	look.y -= dt * speed;
	//}
	//
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
	//	pos.x -= dt * speed;
	//	look.x -= dt * speed;
	//}
	//
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
	//	pos.x += dt * speed;
	//	look.x += dt * speed;
	//}
	//
	//if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) {
	//	pos.z-= dt * speed;
	//	look.z -= dt * speed;
	//}
	//
	//if (GetAsyncKeyState(VK_RSHIFT) & 0x8000) {
	//	pos.z  += dt * speed;
	//	look.z += dt * speed;
	//}
	//
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
	//	pos.x += dt * speed;
	//	look.x += dt * speed;
	//}

	//D3DXVec3Normalize(&look, &look);

	transform->Set_Pos(pos);
	transform->Set_Look(look);
}


void CTestCam::Free()
{
}
