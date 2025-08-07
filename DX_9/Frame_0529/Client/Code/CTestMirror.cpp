#include"pch.h"
#include "Engine_Define.h"
#include "CTestMirror.h"
#include "CMeshRenderer.h"
#include "CTransform.h"
#include "CReflectRenderer.h"

CTestMirror::CTestMirror()
{
}

CTestMirror::~CTestMirror()
{
}

CTestMirror* CTestMirror::Create()
{
	CTestMirror* instance = new CTestMirror;

	if (FAILED(instance->Ready_GameObject())) {

		Safe_Release(instance);
		instance = nullptr;

	}
	return instance;
}

HRESULT CTestMirror::Ready_GameObject()
{
	m_pTransform = Add_Component<CTransform>();
	m_pTransform->Set_Pos({ -5,0,-0.f });
	m_pTransform->Set_Scale({ 5.f,5.f,1.f });
	if (!m_pTransform) return E_FAIL;

	m_pRenderer = Add_Component<CMeshRenderer>();
	m_pRenderer->Set_Transform();
	m_pRenderer->Set_Mesh(L"MirrorCube");

	m_pMirror = Add_Component<CReflectRenderer>();
	m_pMirror->Set_Transform();
	m_pMirror->Set_Mesh(L"MirrorCube",{1});

	return S_OK;
}

void CTestMirror::Update_GameObject(_float& dt)
{
	__super::Update_Component(dt);

	_vec3 rot = m_pTransform->Get_Rotate();
	_vec3 pos = m_pTransform->Get_Pos();

	const float speed = 50.f;
	if (GetAsyncKeyState('C') & 0x8000)
		rot.y -= dt * speed;
	if (GetAsyncKeyState('V') & 0x8000)
		rot.y += dt * speed;

	m_pTransform->Set_Rotate(rot);
	m_pTransform->Set_Pos(pos);
}

void CTestMirror::LateUpdate_GameObject(_float& dt)
{
	__super::LateUpdate_Component(dt);
}

void CTestMirror::Set_Target(CGameObject* obj)
{
}

void CTestMirror::Free()
{
	__super::Release_Component();
}
