#include "Engine_Define.h"
#include "CCameraMgr.h"
#include "CCameraActor.h"
#include "CCamera.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
	:m_pCurCam(nullptr)
{
}

CCameraMgr::~CCameraMgr()
{
}


void CCameraMgr::Set_ViewTarget(CCameraActor* pCamActor)
{
	m_pCurCam = pCamActor;
}

void CCameraMgr::Apply_Camera(LPDIRECT3DDEVICE9 pDevice)
{

	if (m_pCurCam == nullptr)
		return;

	CCamera* pCam = m_pCurCam->Get_Component<CCamera>();
	
	if (pCam == nullptr)
		return;

	const _matrix& matView = pCam->Get_ViewMatrix();
	const _matrix& matProj = pCam->Get_ProjMatrix();

	pDevice->SetTransform(D3DTS_VIEW ,&matView);
	pDevice->SetTransform(D3DTS_PROJECTION ,&matProj);

}

void CCameraMgr::Free()
{
}

