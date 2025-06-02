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
	CCameraActor* instance = new CCameraActor;

	if(FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		return nullptr;
	}

	return instance;
}

HRESULT CCameraActor::Ready_GameObject()
{
	m_pCamera = Add_Component<CCamera>();
	if (m_pCamera == nullptr)
		return E_FAIL;

	m_pTransform = Add_Component<CTransform>();
	if (m_pTransform == nullptr)
		return E_FAIL;

	return S_OK;
}

void CCameraActor::Update_GameObject(float dt)
{
	__super::Update_Component(dt);
}

void CCameraActor::LateUpdate_GameObject(float dt)
{
	__super::LateUpdate_Component(dt);
}

void CCameraActor::Free()
{

}