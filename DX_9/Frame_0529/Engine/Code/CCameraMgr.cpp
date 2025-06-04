#include "Engine_Define.h"
#include "CCameraMgr.h"
#include "CCamera.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
	:m_pCurCam(nullptr)
{
}

CCameraMgr::~CCameraMgr()
{
}


void CCameraMgr::Set_ViewTarget(CCamera* pCam)
{
	m_pCurCam = pCam;
}

void CCameraMgr::Apply_Camera(LPDIRECT3DDEVICE9 pDevice)
{

	if (m_pCurCam == nullptr)
		return;

	const _matrix& matView = m_pCurCam->Get_ViewMatrix();
	const _matrix& matProj = m_pCurCam->Get_ProjMatrix();

	pDevice->SetTransform(D3DTS_VIEW ,&matView);
	pDevice->SetTransform(D3DTS_PROJECTION ,&matProj);
}

void CCameraMgr::Free()
{
}

