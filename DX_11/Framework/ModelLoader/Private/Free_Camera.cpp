#include "Loader_Defines.h"
#include "Free_Camera.h"
#include "Camera.h"
#include "GameInstance.h"
#include "IRenderService.h"
#include "IInputService.h"
#include "IGuiService.h"

CFree_Camera::CFree_Camera()
{
}

CFree_Camera::CFree_Camera(const CFree_Camera& rhs)
	: CGameObject(rhs)
{
}

HRESULT CFree_Camera::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CCamera>();

	return S_OK;
}

HRESULT CFree_Camera::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CFree_Camera::Priority_Update(_float dt)
{
	
	m_pTransform->LookAt(XMLoadFloat3(&m_vPivot));

	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down('W'))
		m_fPitch += dt * m_fSpeed;
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down('S'))
		m_fPitch -= dt * m_fSpeed;
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down('D'))
		m_fYaw += dt * m_fSpeed;
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down('A'))
		m_fYaw -= dt * m_fSpeed;
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_SHIFT))
		m_vPivot.y += dt ;


	m_fPitch = min(max(m_fPitch, (-89.f)), (89.f));
	m_fYaw = min(max(m_fYaw, (-89.f)), (89.f));

	if (CGameInstance::GetInstance()->Get_GUISystem()) {
		if (CGameInstance::GetInstance()->Get_GUISystem()->UsingUI()) {
			return;
		}
	}
	m_fDistance += CGameInstance::GetInstance()->Get_InputDev()->Mouse_DeltaW() * -dt * 4 * m_fSpeed;

	if (m_fDistance < 1.f) {
		m_fDistance = 1.f;
	}

	_float3 CamPos = {};
	CamPos.x = cosf(XMConvertToRadians(m_fPitch)) * cosf(XMConvertToRadians(m_fYaw)) * m_fDistance;
	CamPos.y = sinf(XMConvertToRadians(m_fPitch)) * m_fDistance;
	CamPos.z = cosf(XMConvertToRadians(m_fPitch)) * sinf(XMConvertToRadians(m_fYaw)) * m_fDistance;

	m_pTransform->Set_Pos(CamPos);
}

void CFree_Camera::Update(_float dt)
{
}

void CFree_Camera::Late_Update(_float dt)
{
}

CFree_Camera* CFree_Camera::Create()
{
	CFree_Camera* instance = new CFree_Camera();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFree_Camera");
		Safe_Release(instance);
	}

	return instance;
}

void CFree_Camera::Free()
{
	__super::Free();
}

CGameObject* CFree_Camera::Clone(INIT_DESC* pArg)
{
	CFree_Camera* instance = new CFree_Camera(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CFree_Camera");
		Safe_Release(instance);
	}

	return instance;
}
