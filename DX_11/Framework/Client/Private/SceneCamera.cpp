#include "Client_Defines.h"
#include "SceneCamera.h"

#include "Camera.h"
#include "AudioSource.h"

CSceneCamera::CSceneCamera()
{
}

CSceneCamera::CSceneCamera(const CSceneCamera& rhs)
	:CGameObject(rhs)
{
}
HRESULT CSceneCamera::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CCamera>();
	Add_Component<CAudioSource>();
	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Bobet-Tau.wav", "Ending_Song", true, SOUND_GROUP::SFX);

	Get_Component<CAudioSource>()->Set_3DAttribute("Ending_Song", false);
	Get_Component<CAudioSource>()->Set_SlotVolume("Ending_Song", 0.05f);
	return S_OK;
}

HRESULT CSceneCamera::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CCamera>()->Set_FOV(150.f);
	return S_OK;
}

void CSceneCamera::Awake()
{
}

void CSceneCamera::Priority_Update(_float dt)
{
}

void CSceneCamera::Update(_float dt)
{
	switch (m_eState)
	{
	case Client::CSceneCamera::NONE:
		break;

	case Client::CSceneCamera::START:{
		_bool ZoomIn = Get_Component<CCamera>()->Lerp_FOV(60, dt);
		if (ZoomIn)
			m_eState = LIVE; 
	}
		break;

	case Client::CSceneCamera::LIVE:
		break;
	default:
		break;
	}
	if (m_eState != NONE) {
		Get_Component<CAudioSource>()->Play("Ending_Song");
		Get_Component<CAudioSource>()->FadeIn_Volume("Ending_Song", dt*0.25f, 0.4f);
	}

}

void CSceneCamera::Late_Update(_float dt)
{
}

void CSceneCamera::Execute()
{
	m_eState = START;
}

CSceneCamera* CSceneCamera::Create()
{
	CSceneCamera* instance = new CSceneCamera();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CSceneCamera");
		Safe_Release(instance);
	}

	return instance;
}


CGameObject* CSceneCamera::Clone(INIT_DESC* pArg)
{
	CSceneCamera* instance = new CSceneCamera(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CSceneCamera");
		Safe_Release(instance);
	}

	return instance;
}

void CSceneCamera::Free()
{
	__super::Free();
}