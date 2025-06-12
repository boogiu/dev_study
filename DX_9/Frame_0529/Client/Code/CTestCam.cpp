#include "pch.h"
#include "Engine_Define.h"
#include "CTestCam.h"
#include "CGameObject.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLight.h"

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

	m_pTransform->Set_Pos({ 0.f,5.f,-10.f });
	m_pTransform->Set_Scale({ 15.f,15.f,15.f });

	m_pLight = Add_Component<CLight>(LIGHT_TYPE::SPOTLIGHT);

	m_pLight->Set_LightDesc(
		D3DXCOLOR(1.f, 1.f, 1.f, 0.0f),
		D3DXCOLOR(1.f, 1.f, 1.f, 1.0f),
		D3DXCOLOR(1.f, 1.f, 1.f, 1.0f),
		180.f,
		0.3f
	);

	m_pLight->Set_Angle(30,60);
	return S_OK;
}

void CTestCam::Update_GameObject(_float&dt)
{
	__super::Update_Component(dt);
	m_pLight->Set_Dir(m_pCamera->Get_Dir());
	Key_Check(dt);
}

void CTestCam::LateUpdate_GameObject(_float&dt)
{
	__super::LateUpdate_Component(dt);
}

void CTestCam::Key_Check(_float&dt)
{
	CTransform* transform = Get_Component<CTransform>();
	CCamera* camera = Get_Component<CCamera>();
	_vec3 pos = transform->Get_Pos();

	const float rotSpeed = 90.f; // deg/sec
	const float unit = 100.f;
	
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) 
		camera->Add_Yaw(-rotSpeed * dt);

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) 
		camera->Add_Yaw(rotSpeed * dt);

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		pos -= camera->Get_Dir()*dt*unit;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		pos += camera->Get_Dir() * dt * unit;
	
	if (GetAsyncKeyState('W') & 0x8000)
		camera->Add_Pitch(-rotSpeed * dt);
	if (GetAsyncKeyState('S') & 0x8000)
		camera->Add_Pitch(rotSpeed * dt);

	transform->Set_Pos(pos);
}


void CTestCam::Free()
{
	CGameObject::Release_Component();
}
