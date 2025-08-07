#include "Engine_Define.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CRenderMgr.h"
#include "CStateCache.h"

CRenderer::CRenderer()
	:m_pTransform(nullptr),m_pCache(CRenderMgr::GetInstance()->Get_Cache())
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::Set_Transform()
{
	if(m_pOwner)
		m_pTransform = m_pOwner->Get_Component<CTransform>();
}
