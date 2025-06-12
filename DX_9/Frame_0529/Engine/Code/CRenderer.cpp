#include "Engine_Define.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CTerrainRenderer.h"
#include "CGameObject.h"
#include "CSkyBoxRenderer.h"

CRenderer::CRenderer()
	:m_pTransform(nullptr)
{
	m_eUpdate = COM_UPDATE::DYNAMIC;
}

CRenderer::~CRenderer()
{
}

CRenderer* CRenderer::Create(RENDERER_TYPE eType)
{
	CRenderer* instance = nullptr;

	switch (eType)
	{
	case Engine::CRenderer::RENDERER_TYPE::Mesh:
		instance = CMeshRenderer::Create();
		break;
	case Engine::CRenderer::RENDERER_TYPE::Terrain:
		instance = CTerrainRenderer::Create();
		break;	
	case Engine::CRenderer::RENDERER_TYPE::SkyBox:
		instance = CSkyBoxRenderer::Create();
		break;
	default:
		break;
	}

	return instance;
}

void CRenderer::Set_Transform()
{
	if(m_pOwner)
		m_pTransform = m_pOwner->Get_Component<CTransform>();
}
