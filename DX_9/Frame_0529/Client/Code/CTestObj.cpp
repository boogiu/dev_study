#include"pch.h"
#include "Engine_Define.h"
#include "CTestObj.h"

CTestObj::CTestObj()
	:m_pTransform(nullptr),m_pRenderer(nullptr), m_pMesh(nullptr)
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

	m_pMesh = Add_Component<CMesh>();
	if (m_pMesh == nullptr)
		return E_FAIL;
	m_pMesh->Set_MeshType(Engine::CMesh::MeshType::CUBE);

	m_pRenderer = Add_Component<CRenderer>();
	if (m_pRenderer == nullptr)
		return E_FAIL;

	m_pRenderer->Set_Mesh();
	m_pTransform->Set_Pos({1.f,1.f,1.f});
	m_pTransform->Set_Scale({1.f,1.f,1.f});

	return S_OK;
}

void CTestObj::Update_GameObject(float dt)
{
	__super::Update_Component(dt);
	Key_Check(dt);
}

void CTestObj::LateUpdate_GameObject(float dt)
{
	__super::LateUpdate_Component(dt);
}
void CTestObj::Key_Check(float dt)
{
	CTransform* transform = Get_Component<CTransform>();
	_vec3 rot = transform->Get_Rotate();
	_vec3 pos = transform->Get_Pos();

	const float speed = 50.f;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		rot.x -= dt * speed;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		rot.x += dt * speed;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		rot.y += dt * speed;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		rot.y -= dt * speed;

	transform->Set_Rotate(rot);
	transform->Set_Pos(pos);
}

void CTestObj::Free()
{
}
