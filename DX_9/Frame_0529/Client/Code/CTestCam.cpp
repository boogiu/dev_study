#include "pch.h"
#include "Engine_Define.h"
#include "CTestCam.h"
#include "CGameObject.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLight.h"
#include "CSkyBoxRenderer.h"
#include "CInputMgr.h"
#include "CRenderMgr.h"

CTestCam::CTestCam()
	: m_pCamera(nullptr), m_pTransform(nullptr),xMove(0),yMove(0)
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

	m_pTransform->Set_Pos({ 0.f,5.f,5.f });
	m_pLight = Add_Component<CLight>(LIGHT_TYPE::SPOTLIGHT);

	m_pLight->Set_LightDesc(
		D3DXCOLOR(1.f, 1.f, 1.f, 0.0f),
		D3DXCOLOR(1.f, 1.f, 1.f, 0.6f),
		D3DXCOLOR(1.f, 1.f, 1.f, 0.7f),
		80.f,
		0.5f
	);

	m_pLight->Set_Angle(40,60);

	m_pRenderer = Add_Component<CSkyBoxRenderer>();
	m_pRenderer->Set_SkyBox(L"burger2.dds");
	m_pRenderer->Set_Transform();
	m_pTransform->Set_Scale({ 300.f, 300.f, 300.f });

	m_tPivot = { WINCX / 2 , WINCY / 2 };

	return S_OK;
}

void CTestCam::Update_GameObject(_float&dt)
{
	__super::Update_Component(dt);
	//
	m_pLight->Set_Dir(m_pCamera->Get_Dir());
	Key_Check(dt);
}

void CTestCam::LateUpdate_GameObject(_float&dt)
{
	__super::LateUpdate_Component(dt);

}

void CTestCam::Key_Check(_float&dt)
{
	CCamera* camera = Get_Component<CCamera>();
	
	CTransform* transform = Get_Component<CTransform>();
	_vec3 pos = transform->Get_Pos();

	_long mouseX = CInputMgr::GetInstance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X);
	_long mouseY = CInputMgr::GetInstance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMD_Y);
	_long mouseWheel = CInputMgr::GetInstance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMD_Z);

	const float rotSpeed = 90.f; // deg/sec
	const float unit = 10.f;
	
	_vec3 Look = camera->Get_Dir();
	_vec3 up = camera->Get_Up();
	_vec3 Right;

	D3DXVec3Cross(&Right,&up, &Look);

	if (GetAsyncKeyState('A') & 0x8000)
		pos -= Right * dt * unit;

	if (GetAsyncKeyState('D') & 0x8000)
		pos += Right * dt * unit;

	if (GetAsyncKeyState('W') & 0x8000)
		pos += Look * dt * unit;

	if (GetAsyncKeyState('S') & 0x8000)
		pos -= Look *dt*unit;
	
	pos+= mouseWheel * dt * Look;

	transform->Set_Pos(pos);


	xMove += mouseX;
	yMove += mouseY;

	camera->Add_Yaw(mouseX * dt * 150);
	camera->Add_Pitch(mouseY * dt * 150);

	POINT pt = { WINCX / 2, WINCY / 2 }; // 클라이언트 중심
	ClientToScreen(g_hWnd, &pt);           // 클라이언트 → 화면 좌표로 변환
	//GetCursorPos(&pt);
	SetCursorPos(pt.x,pt.y);
	ShowCursor(FALSE); // 커서 숨김

	wstring txt = L"x무브 : " + to_wstring(xMove) + L" / y무브" + to_wstring(yMove);
}


void CTestCam::Free()
{
	CGameObject::Release_Component();
}