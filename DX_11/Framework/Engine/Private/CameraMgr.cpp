#include "CameraMgr.h"
#include "Camera.h"
#include "IAudioService.h"
#include "GameInstance.h"
#include"GameObject.h"

CCameraMgr::CCameraMgr()
{
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_InversedProjMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_InversedViewMatrix, XMMatrixIdentity());
}

void CCameraMgr::Set_MainCam(CCamera* pCamCom)
{

	Safe_Release(m_pMainCam);
	m_pMainCam = pCamCom;

	CGameInstance::GetInstance()->Get_AudioDev()->Set_Listener(
		m_pMainCam->Get_Owner()->Get_Component<CTransform>()
	);

	Safe_AddRef(m_pMainCam);
}

void CCameraMgr::Update(_float dt)
{
	if (m_pMainCam) {
		m_ViewMatrix = m_pMainCam->Get_ViewMatrix();
		XMStoreFloat4x4(&m_ProjMatrix, m_pMainCam->Get_ProjMatrix());
		XMStoreFloat4x4(&m_InversedViewMatrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ViewMatrix)));
		XMStoreFloat4x4(&m_InversedProjMatrix, XMMatrixInverse(nullptr, m_pMainCam->Get_ProjMatrix()));
		XMStoreFloat4(&m_vCamPos, m_pMainCam->Get_Owner()->Get_Component<CTransform>()->Get_Pos());
	}
}


const _float4 CCameraMgr::Get_CameraPos()
{
	return m_vCamPos;
}

CCameraMgr* CCameraMgr::Create()
{
	return new CCameraMgr;
}

void CCameraMgr::Free()
{
	Safe_Release(m_pMainCam);
}
