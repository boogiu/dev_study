#include "pch.h"
#include "Engine_Define.h"
#include "CTestLigh.h"
#include "CTransform.h"
#include "CLight.h"

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

	transform->Set_Scale({1.f,1.f,1.f});
	transform->Set_Pos({15.f,15.f,5.f});

	light->Set_LightDesc(
		D3DXCOLOR(1.f,1.f,1.f,1.0f),
		D3DXCOLOR(1.f,1.f,1.f, 1.0f),
		D3DXCOLOR(1.f,1.f,1.f, 1.0f),
		80.f,
		0.2f
	);

	return S_OK;
}

void CTestLigh::Update_GameObject(_float&dt)
{
	__super::Update_Component(dt);
	Key_Check(dt);
}

void CTestLigh::LateUpdate_GameObject(_float&dt)
{
	__super::LateUpdate_Component(dt);
}

void CTestLigh::Key_Check(float dt)
{
	CTransform* transform = Get_Component<CTransform>();
	_vec3 pos = transform->Get_Pos();

	const float speed = 50.f;

	
	transform->Set_Pos(pos);
}

void CTestLigh::Free()
{
	CGameObject::Release_Component();
}
