#include "Engine_Define.h"
#include "CCameraActor.h"
#include "CTransform.h"
#include "CCamera.h"

CCameraActor::CCameraActor()
	: m_pCamera(nullptr),m_pTransform(nullptr)
{
}

CCameraActor::~CCameraActor()
{
}

CCameraActor* CCameraActor::Create()
{
	return nullptr;
}

HRESULT CCameraActor::Ready_CameraActor()
{
	m_pCamera = Add_Component<CCamera>();
	if (m_pCamera == nullptr)
		return E_FAIL;

	m_pTransform = Add_Component<CTransform>();
	if (m_pTransform == nullptr)
		return E_FAIL;

	return S_OK;
}

void CCameraActor::Update_Camera(float dt)
{
}

void CCameraActor::LateUpdate_Camera(float dt)
{
}

void CCameraActor::Free()
{
}
