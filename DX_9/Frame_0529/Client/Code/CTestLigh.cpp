#include "pch.h"
#include "Engine_Define.h"
#include "CTestLigh.h"
#include "CTransform.h"
#include "CLight.h"
#include "CMesh.h"
#include "CRenderer.h"

CTestLigh::CTestLigh()
{
}

CTestLigh::~CTestLigh()
{
}

CTestLigh* CTestLigh::Create()
{
	CTestLigh* instance = new CTestLigh;

	if (FAILED(instance->Ready_GameObject())) {
		Engine::Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CTestLigh::Ready_GameObject()
{
	CTransform* transform = Add_Component<CTransform>();

	if (nullptr == transform)
		return E_FAIL;

	CLight* light = Add_Component<CLight>(LIGHT_TYPE::POINT);

	if (nullptr == light)	
		return E_FAIL;

	CMesh* mesh = Add_Component<CMesh>();
	if (mesh == nullptr)
		return E_FAIL;
	mesh->Set_MeshType(Engine::CMesh::MeshType::CUBE);

	CRenderer* m_pRenderer = Add_Component<CRenderer>();
	if (m_pRenderer == nullptr)
		return E_FAIL;


	//초기 설정
	m_pRenderer->Set_Mesh();
	transform->Set_Scale({1.f,1.f,1.f});
	transform->Set_Pos({15.f,15.f,5.f});

	light->Set_LightDesc(
		D3DXCOLOR(1.f,1.f,1.f,0.0f),
		D3DXCOLOR(1.f,1.f,1.f,0.1f),
		D3DXCOLOR(1.f,1.f,1.f,0.0f),
		80.f,
		0.2f
	);

	return S_OK;
}

void CTestLigh::Update_GameObject(float dt)
{
	__super::Update_Component(dt);
	Key_Check(dt);
}

void CTestLigh::LateUpdate_GameObject(float dt)
{
	__super::LateUpdate_Component(dt);
}

void CTestLigh::Key_Check(float dt)
{
	CTransform* transform = Get_Component<CTransform>();
	_vec3 pos = transform->Get_Pos();

	const float speed = 50.f;

	if (GetAsyncKeyState('A') & 0x8000)
		pos.x -= dt * speed;
	if (GetAsyncKeyState('D') & 0x8000)
		pos.x += dt * speed;
	if (GetAsyncKeyState('W') & 0x8000)
		pos.y += dt * speed;
	if (GetAsyncKeyState('S') & 0x8000)
		pos.y -= dt * speed;
	if (GetAsyncKeyState('Q') & 0x8000)
		pos.z -= dt * speed;
	if (GetAsyncKeyState('E') & 0x8000)
		pos.z += dt * speed;
	transform->Set_Pos(pos);
}

void CTestLigh::Free()
{
}
