#include "Client_Defines.h"
#include "Free_Camera.h"
#include "Camera.h"
#include "GameInstance.h"
#include "IRenderService.h"
#include "IInputService.h"

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
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down('W'))
		m_pTransform->Translate(m_pTransform->Dir(STATE::LOOK) * 15 * dt);
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down('S'))
		m_pTransform->Translate(m_pTransform->Dir(STATE::LOOK) * 15 * -dt);
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down('D'))
		m_pTransform->Translate(m_pTransform->Dir(STATE::RIGHT) * 15 * dt);
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down('A'))
		m_pTransform->Translate(m_pTransform->Dir(STATE::RIGHT) * 15 * -dt);

	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_SHIFT))
		m_pTransform->Translate(_vector{ 0,1,0,0 }*15 * dt);

	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_CONTROL) && CGameInstance::GetInstance()->Get_InputDev()->Mouse_Hold(MOUSE_BTN::LB)) {

		_vector targetVelocity = {
			CGameInstance::GetInstance()->Get_InputDev()->Mouse_DeltaX() * fMouseSensitive,
			CGameInstance::GetInstance()->Get_InputDev()->Mouse_DeltaY() * fMouseSensitive
		};

		_vector RotationVelocity = XMLoadFloat2(&m_vRotationVelocity);

		RotationVelocity = XMVectorLerp(RotationVelocity, targetVelocity, fLerpPercent * dt);
		XMStoreFloat2(&m_vRotationVelocity, RotationVelocity);

		if (abs(m_vRotationVelocity.x) > 0.001f)
			m_pTransform->Rotation({ 0, XMConvertToRadians(-m_vRotationVelocity.x),0,0 });
		if (abs(m_vRotationVelocity.y) > 0.001f)
			m_pTransform->Rotation({ 1,0,0,0 }, XMConvertToRadians(m_vRotationVelocity.y));
	}
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
