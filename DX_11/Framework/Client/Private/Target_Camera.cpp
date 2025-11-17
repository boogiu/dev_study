#include "Client_Defines.h"
#include "Target_Camera.h"
#include "Camera.h"
#include "Light.h"
#include "GameInstance.h"
#include "EventSystem.h"
#include "Level.h"
#include "Helper_Func.h"
CTarget_Camera::CTarget_Camera()
{
}

CTarget_Camera::CTarget_Camera(const CTarget_Camera& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTarget_Camera::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CCamera>();
	Add_Component<CLight>();
	return S_OK;
}

HRESULT CTarget_Camera::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	TARGET_CAM_DESC* pDesc = static_cast<TARGET_CAM_DESC*>(pArg);
	m_pTarget = pDesc->pTarget;
	_float4 TagetPos = m_pTarget->Get_Position();

	m_pTransform->Set_Pos({ TagetPos.x, 30,	TagetPos.z + 50 });
	m_pTransform->LookAt({ TagetPos.x, 0,	TagetPos.z + 10 });
	m_vOffset = pDesc->vOffset;

	m_vZoomInOffset = { 0,15,50,0 };
	m_fCurrentLookY = 0;


	LIGHT_DESC desc = {};
	desc.vLightPosition = { 0,20,0,0 };
	desc.fLightRange = 150.0f;
	desc.vLightDirection = _float4(-1.f, -1.f, -1.f, 0.f);
	desc.vLightDiffuse = _float4(.8f, .8f, .8f, 1.f);
	desc.vLightAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	desc.vLightSpecular = _float4(0.f, 1.f, 0.f, 1.f);

	Get_Component<CLight>()->Set_Desc(desc, LIGHT_TYPE::DIRECTIONAL);
	return S_OK;
}

void CTarget_Camera::Awake()
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	auto evtSys = nowLevel->Get_LevelObject<CEventSystem>();

	evtSys->Add_Listner<CTarget_Camera, BaseEvent>(this,&CTarget_Camera::Event_Listen);
}

void CTarget_Camera::Priority_Update(_float dt)
{
	switch (m_eState)
	{
	case Client::CTarget_Camera::FOLLOW:
		Follow_Target(dt);
		break;
	case Client::CTarget_Camera::ZOOM_IN:
		Zoom_In(dt);
		break;
	case Client::CTarget_Camera::ZOOM_OUT:
		Zoom_Out(dt);
		break;
	case Client::CTarget_Camera::TALKING:
		Zoom_Talking(dt);
		break;
	case Client::CTarget_Camera::TALK_OUT:
		Zoom_TalkingOut(dt);
		break;
	case Client::CTarget_Camera::SHAKE:
		Shake_Cam(dt);
		break;
	default:
		break;
	}

}

void CTarget_Camera::Update(_float dt)
{
	_float4 targetPos = m_pTarget->Get_Position();
}

void CTarget_Camera::Late_Update(_float dt)
{
}

void CTarget_Camera::Event_Listen(const BaseEvent& event)
{
	if (event.eType == EVENT_TYPE::CameraMove) {
		const auto& evt = static_cast<const CAM_MOVE&>(event);
		m_prevState = m_eState;
			if (evt.moveTag == "Shake")
				m_eState = SHAKE;
	}
}

void CTarget_Camera::Execute_ZoomIn()
{
	m_eState = ZOOM_IN;
}

void CTarget_Camera::Release_ZoomIn()
{
	m_pSubject = nullptr;
	m_eState = ZOOM_OUT;
}

void CTarget_Camera::Execute_Talking(CGameObject* subject)
{
	m_pSubject = subject;
	m_eState = TALKING;
}

void CTarget_Camera::Release_Talking(CGameObject* subject)
{
	m_pSubject = subject;
	m_eState = TALK_OUT;
}

void CTarget_Camera::Zoom_In(_float dt)
{
	_vector target_Pos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
	_vector Offset = XMLoadFloat4(&m_vZoomInOffset);
	_vector cam_Pos = m_pTransform->Get_Pos(); //Now Pso

	//Move Lerp
	_vector MoveDir = XMVectorLerp(cam_Pos, target_Pos + Offset, dt * 5);

	_float3 DstPos;
	XMStoreFloat3(&DstPos, MoveDir);

	_float3 LookPos;
	XMStoreFloat3(&LookPos, target_Pos);
	//보는 방향은 그대로 유지하되, y축만 올려야함. (룩앳포즈는 올리고, 현재 포즈는 내린다)

	if (m_fCurrentLookY < 15)
	{
		m_fCurrentLookY += dt * 25;
	}
	if (m_fCurrentLookY > 15)
	{
		m_fCurrentLookY = 15;
	}
	m_pTransform->LookAt({ LookPos.x, m_fCurrentLookY,	LookPos.z + 10 });

	m_pTransform->Set_Pos(DstPos);

	Get_Component<CCamera>()->Lerp_FOV(40, dt * 1.5);
}

void CTarget_Camera::Zoom_Out(_float dt)
{
	auto targetTransform = m_pTarget->Get_Component<CTransform>();
	_vector target_Pos = targetTransform->Get_Pos();
	_vector Origin_Pos = XMLoadFloat4(&m_vBasePos);
	_vector cam_Pos = m_pTransform->Get_Pos();

	// 위치 보간
	_vector MoveDir = XMVectorLerp(cam_Pos, Origin_Pos, dt * 5.f);

	_float3 DstPos;
	XMStoreFloat3(&DstPos, MoveDir);

	_float3 LookPos;
	XMStoreFloat3(&LookPos, target_Pos);

	// LookY를 0으로 천천히 복귀
	if (XMVectorGetX(XMVector3Length(Origin_Pos - MoveDir)) < 0.1f)
	{
		m_pTransform->Set_Pos(m_vBasePos);
		m_fCurrentLookY = 0.f;
		m_eState = FOLLOW;
		Get_Component<CCamera>()->Lerp_FOV(60.f, dt * 5);
		return;
	}

	m_pTransform->LookAt(XMLoadFloat4(&m_vBaseLookPos));

	m_pTransform->Set_Pos(DstPos);

	// FOV 복귀
	Get_Component<CCamera>()->Lerp_FOV(60.f, dt * 5);
}


void CTarget_Camera::Zoom_Talking(_float dt)
{
	/*나와 타겟의 사이를 벡터로 연결 (카메라가 나를 보는 ㄴ상황)*/
	/*화자 , 청자의 벡터가 모두 필요함.*/
	_vector ConnectVector = (
		m_pTarget->Get_Component<CTransform>()->Get_Pos() +
		m_pSubject->Get_Component<CTransform>()->Get_Pos()
		) * 0.5f;

	_vector SubjectLookPlayer = {
		 m_pTarget->Get_Component<CTransform>()->Get_Pos() - m_pSubject->Get_Component<CTransform>()->Get_Pos() };

	_vector right = XMVector3Cross({ 0,1,0 }, SubjectLookPlayer);
	_float playerDeltaX = XMVectorGetX(m_pTarget->Get_Component<CTransform>()->Get_Pos()) - XMVectorGetX(m_pSubject->Get_Component<CTransform>()->Get_Pos());

	if (playerDeltaX > 0.f)
	{
		right = right * -1.f;
	}

	_vector target_Pos = ConnectVector + (SubjectLookPlayer * 0.6f + right * 1.1f) * 4.f;
	target_Pos = XMVectorSetY(target_Pos, 35);

	_vector cam_Pos = m_pTransform->Get_Pos(); //Now Pso
	//Move Lerp
	_vector MoveDir = XMVectorLerp(cam_Pos, target_Pos, dt * 3);

	_float3 DstPos;
	XMStoreFloat3(&DstPos, MoveDir);

	m_pTransform->LookAt(ConnectVector);
	m_pTransform->Set_Pos(DstPos);
	Get_Component<CCamera>()->Lerp_FOV(45, dt * 1.5);
}

void CTarget_Camera::Zoom_TalkingOut(_float dt)
{

}

void CTarget_Camera::Follow_Target(_float dt)
{
	_vector target_Pos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
	_vector Offset = XMLoadFloat4(&m_vOffset);
	_vector cam_Pos = m_pTransform->Get_Pos(); //Now Pso

	//Move Lerp
	_vector MoveDir = XMVectorLerp(cam_Pos, target_Pos + Offset, dt * 10);

	_float3 DstPos;
	XMStoreFloat3(&DstPos, MoveDir);

	m_pTransform->Set_Pos(DstPos);
	XMStoreFloat4(&m_vBasePos, target_Pos + Offset);
	XMStoreFloat4(&m_vBaseLookPos, XMVectorSet(XMVectorGetX(target_Pos), 0.f, XMVectorGetZ(target_Pos) + 10.f, 0.f));
}
void CTarget_Camera::Shake_Cam(_float dt)
{
	_float duration = 0.9f;				// 흔들림 지속시간
	_float amplitude = 0.45f;		// 진폭 크기
	_float frequency = 30.0f;		// 흔들림 속도
	_float damping = 3.0f;			// 감쇠 정도

	if (m_fShakeTime == 0.f)
	{
		m_vShakeBasePos = Get_Position();
		m_vShakePhase = {
			Helper::Get_Random_Float(0.f, XM_2PI),
			Helper::Get_Random_Float(0.f, XM_2PI),
			Helper::Get_Random_Float(0.f, XM_2PI)
		};
	}

	m_fShakeTime += dt;

	if (m_fShakeTime > duration)
	{
		m_eState = m_prevState;
		m_fShakeTime = 0.f;
		m_vShakeOffset = { 0, 0, 0, 0 };
		m_pTransform->Set_Pos(m_vShakeBasePos);
		return;
	}

	float attenuate = expf(-damping * m_fShakeTime); /*지수 함수*/

	float offsetX = sinf(m_fShakeTime * frequency					+ m_vShakePhase.x);
	float offsetY = cosf(m_fShakeTime * frequency * 0.9f	+ m_vShakePhase.y);
	float offsetZ = sinf(m_fShakeTime * frequency * 1.1f		+ m_vShakePhase.z);

	_vector offset = XMVectorSet(offsetX, offsetY, offsetZ, 0.f);
	_vector newPos = XMLoadFloat4(&m_vShakeBasePos)+offset * amplitude * attenuate;

	_float4 pos = {};
	XMStoreFloat4(&pos, newPos);
	m_pTransform->Set_Pos(pos);
}


void CTarget_Camera::Render_GUI()
{
	__super::Render_GUI();
	ImGui::InputFloat3("Offset", reinterpret_cast<_float*>(&m_vOffset));
}

CTarget_Camera* CTarget_Camera::Create()
{
	CTarget_Camera* instance = new CTarget_Camera();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CTarget_Camera");
		Safe_Release(instance);
	}

	return instance;
}


CGameObject* CTarget_Camera::Clone(INIT_DESC* pArg)
{
	CTarget_Camera* instance = new CTarget_Camera(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CTarget_Camera");
		Safe_Release(instance);
	}

	return instance;
}

void CTarget_Camera::Free()
{
	__super::Free();
}