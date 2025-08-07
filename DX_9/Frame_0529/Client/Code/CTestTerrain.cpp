#include "pch.h"
#include "Engine_Define.h"
#include "CTestTerrain.h"
#include "CTransform.h"
#include "CTerrainRenderer.h"
#include "CTerrain.h"

CTestTerrain::CTestTerrain()
{
}

CTestTerrain::~CTestTerrain()
{
}

CTestTerrain* CTestTerrain::Create()
{
	CTestTerrain* instance = new CTestTerrain;

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}
	return instance;
}

HRESULT CTestTerrain::Ready_GameObject()
{
	m_pTransform = Add_Component<CTransform>();
	m_pTransform->Set_Pos({ 0.f,-60.f,0.f });
	m_pRenderer = Add_Component<CTerrainRenderer>();
	m_pRenderer->Set_Mesh(L"Height1");
	m_pRenderer->Set_Transform();
	m_pRenderer->Set_Terrain(1,0.3f);
	return S_OK;
}

void CTestTerrain::Update_GameObject(_float& dt)
{
	__super::Update_Component(dt);
}

void CTestTerrain::LateUpdate_GameObject(_float& dt)
{
	__super::LateUpdate_Component(dt);
}

void CTestTerrain::Free()
{
	CGameObject::Release_Component();
}
