#include "pch.h"
#include "Engine_Define.h"
#include "CTestTerrain.h"
#include "CTransform.h"
#include "CRenderer.h"
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
	m_pTransform->Set_Pos({ 0.f,-100.f,0.f });
	m_pRenderer = static_cast<CTerrainRenderer*>(Add_Component<CRenderer>(CRenderer::RENDERER_TYPE::Terrain));
	m_pRenderer->Set_Mesh("coastMountain64");
	m_pRenderer->Set_Transform();
	m_pRenderer->Set_Terrain(256, 256, 15, 0.9f);
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
