#include "pch.h"
#include "Engine_Define.h"
#include "CTestScene.h"
#include "CRenderMgr.h"
#include "CStateCache.h"
#include "CCameraMgr.h"
#include "CLayer.h"

#include "CGameObject.h"

#include "CCamera.h"
#include "CTestObj.h"
#include "CTestCam.h"
#include "CTestLigh.h"
#include "CTestTerrain.h"
#include "CTestMirror.h"

HRESULT CTestScene::Ready_Scene()
{
	m_pCache = CRenderMgr::GetInstance()->Get_Cache();

	if (!m_pCache) {
		return E_FAIL;
	}

	Create_Layer(L"Camera");
	Create_Layer(L"Object");
	Create_Layer(L"Light");
	Create_Layer(L"Terrain");

	m_pCam = CTestCam::Create();
	m_pCam->AddRef();

	m_mapLayer[L"Camera"]->Add_Object(m_pCam);
	m_mapLayer[L"Object"]->Add_Object(CTestObj::Create());
	m_mapLayer[L"Object"]->Add_Object(CTestMirror::Create());
	m_mapLayer[L"Light"]->Add_Object(CTestLigh::Create());
	m_mapLayer[L"Terrain"]->Add_Object(CTestTerrain::Create());
}

HRESULT CTestScene::Enter_Scene()
{
	CCameraMgr::GetInstance()->Set_ViewTarget(m_pCam->Get_Component<CCamera>());
	return S_OK;
}

HRESULT CTestScene::Exit_Scene()
{
	return S_OK;
}

CTestScene::CTestScene()
	:m_pCam(nullptr)
{
}

CTestScene::~CTestScene()
{
}

CTestScene* CTestScene::Create()
{
	CTestScene* instance = new CTestScene;

	if (FAILED(instance->Ready_Scene())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

void CTestScene::Update_Scene(_float& dt)
{
	m_mapLayer[L"Camera"]->Update_Layer(dt);
	m_mapLayer[L"Light"]->Update_Layer(dt);
	m_mapLayer[L"Terrain"]->Update_Layer(dt);
	m_mapLayer[L"Object"]->Update_Layer(dt);
}

void CTestScene::LateUpdate_Scene(_float& dt)
{
	m_mapLayer[L"Camera"]->LateUpdate_Layer(dt);
	m_mapLayer[L"Light"]->LateUpdate_Layer(dt);
	m_mapLayer[L"Terrain"]->LateUpdate_Layer(dt);
	m_mapLayer[L"Object"]->LateUpdate_Layer(dt);
}

void CTestScene::Render_Scene(LPDIRECT3DDEVICE9 pDevice)
{
}

void CTestScene::Free()
{
	Safe_Release(m_pCam);
	Free_AllLayer();
}