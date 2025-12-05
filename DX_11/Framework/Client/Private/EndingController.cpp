#include "Client_Defines.h"
#include "EndingController.h"
#include "EventSystem.h"
#include "Level.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Player.h"
#include "NonPlayer.h"
#include "UI_Responcer.h"
#include "Animator3D.h"
#include "SunCam.h"
#include "SkyBox.h"
#include "LeafParticle.h"
#include "SceneCamera.h"
#include "Camera.h"
#include "AudioSource.h"
#include "Moon.h"

CEndingController::CEndingController()
{
}

CEndingController::CEndingController(const CEndingController& rhs)
	:CLevelObject(rhs)
{
}

HRESULT CEndingController::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	
	return S_OK;
}

HRESULT CEndingController::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
		return S_OK;
}

void CEndingController::Awake()
{
}

void CEndingController::Priority_Update(_float dt)
{
}

void CEndingController::Update(_float dt)
{
	if (m_bEndingStart) {
		m_fElapsedTime += dt;
	}

	if (m_fElapsedTime > 3.f) {
		m_pOwner->Get_LevelObject<CUI_Responcer>()->Active_UI("Screen_FX");
		m_bEndingStart = false; 
		m_bEndingProceed = true;
		m_fElapsedTime = 0.f;
		Batch_Character();
	}
}

void CEndingController::Late_Update(_float dt)
{
}

void CEndingController::Set_EventSystem(CEventSystem* pSystem)
{
	 m_pEventSystem = pSystem;
	 if (m_pEventSystem)
		 m_pEventSystem->Add_Listner<CEndingController, BaseEvent>(this, &CEndingController::Recieve_Ending);
}

void CEndingController::Recieve_Ending(const BaseEvent& evt)
{
	if (evt.eType == EVENT_TYPE::Ending)
	{
		auto ObjMgr = CGameInstance::GetInstance()->Get_ObjectMgr();
		auto NpcLayer = ObjMgr->Get_Layer({ m_pOwner->Get_Key(), "NonPlayer_Layer" });
		for (auto npc : NpcLayer->Get_AllObject()) {
			CNonPlayer* pNpc = dynamic_cast<CNonPlayer*>(npc);
			if (pNpc)
				m_NonPlayers.push_back(pNpc);
		}

		auto PlayerLayer = ObjMgr->Get_Layer({ m_pOwner->Get_Key(), "Player_Layer" });
		for (auto player : PlayerLayer->Get_AllObject()) {
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(player);
			if (pPlayer)
				m_Player = pPlayer;
		}

		m_bEndingStart = true;
	}
}

void CEndingController::Batch_Character()
{
	m_Player->Get_ControlPack().MsgTakeControl = true;
	m_Player->Get_Component<CTransform>()->Set_Pos({ 1115,0,685,1 });
	m_Player->Get_Component<CTransform>()->LookAt({ 1105,0,638,1 });
	m_Player->Get_Component<CAnimator3D>()->Change_Animation("Act_Rhythm03.anim");

	for (size_t i = 0; i < m_NonPlayers.size(); i++)
	{
		m_NonPlayers[i]->Take_Controll();

		if (m_NonPlayers[i]->Get_NpcData().NpcID == 3) {
			m_NonPlayers[i]->Get_Component<CTransform>()->Set_Pos({ 1105,0,638,1 });
			m_NonPlayers[i]->Get_Component<CTransform>()->LookAt({ 1105,0,645,1 });
			m_NonPlayers[i]->Get_Component<CAnimator3D>()->Change_Animation("Act_PlayMusicSp00.anim");
		}
		else {
			m_NonPlayers[i]->Get_Component<CAnimator3D>()->Change_Animation("Act_Rhythm03.anim");
			m_NonPlayers[i]->Get_Component<CTransform>()->Set_Pos({ 1105 - (10.f * i),0,685 ,1 });
			m_NonPlayers[i]->Get_Component<CTransform>()->LookAt({ 1105,0,638,1 });
		}
	}

	m_pSkyBox->Set_Night();
	m_pSun->Set_Night();
	m_pLeaf->Set_DeActive();

	m_pEndingCam->Get_Component<CTransform>()->Set_Pos({ 1105,45,725 });
	m_pEndingCam->Get_Component<CTransform>()->LookAt({ 1105,0,638,1 });
	m_pEndingCam->Execute();

	m_pMoon->Set_Alive(true);
	m_pStars->Set_Alive(true);

	CGameInstance::GetInstance()->Get_CameraMgr()->Set_MainCam(m_pEndingCam->Get_Component<CCamera>());
	CGameInstance::GetInstance()->Get_AudioDev()->Set_GroupVolume(SOUND_GROUP::ENV, 0.f);
}

void CEndingController::Execute_Scene()
{
}

void CEndingController::Set_EnvObject(CGameObject* pSky, CGameObject* pSun, CGameObject* pLeaf)
{
	m_pSkyBox = dynamic_cast<CSkyBox*>(pSky);
	m_pSun = dynamic_cast<CSunCam*>(pSun);

	m_pLeaf = dynamic_cast<CLeafParticle*>(pLeaf);

	CAMERA_DESC desc = {};
	desc.fAspect=(float)Client::g_iWinSizeX / (float)Client::g_iWinSizeY;
	desc.fFar = 500;
	desc.fNear = 3;
	desc.fFov = 150;

	CGameObject* pObj = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_SceneCamera" })
		.Camera(desc)
		.Position({ 1105,0,685})
		.Build("SceneCam");
	m_pEndingCam = dynamic_cast<CSceneCamera*>(pObj);
	CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pObj, { "GamePlay_Level","Camera_Layer" });

	CGameObject* pMoon= Builder::Create_Object({ "GamePlay_Level","GamePlay_Env_Moon" })
		.Build("Moon");
	m_pMoon = dynamic_cast<CMoon*>(pMoon);
	m_pMoon->Set_Alive(false);
	CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(m_pMoon, { "GamePlay_Level","Env_Layer" });

	m_pStars = Builder::Create_Object({ "GamePlay_Level","GamePlay_Env_Stars" }).Build("Star");
	m_pStars->Set_Alive(false);
	CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(m_pStars, { "GamePlay_Level","Env_Layer" });
}

CEndingController* CEndingController::Create()
{
	CEndingController* instance = new CEndingController();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CEndingController");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CEndingController::Clone(INIT_DESC* pArg)
{
	CEndingController* instance = new CEndingController(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CEndingController");
		Safe_Release(instance);
	}

	return instance;
}

void CEndingController::Free()
{
	__super::Free();
}
