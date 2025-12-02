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
	if (m_bEndingStart)
		m_fElapsedTime += dt;

	if (m_fElapsedTime > 3.f) {
		m_pOwner->Get_LevelObject<CUI_Responcer>()->Active_UI("Screen_FX");
		m_bEndingStart = false;
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
	m_Player->Get_Component<CTransform>()->Set_Pos({ 1105,0,685,1 });

	for (size_t i = 0; i < m_NonPlayers.size(); i++)
	{
		m_NonPlayers[i]->Take_Controll();
		m_NonPlayers[i]->Get_Component<CAnimator3D>()->Change_Animation("MaRe_Clapping.anim");
		m_Player->Get_Component<CTransform>()->Set_Pos({ 1105 - (10.f * i),0,685 ,1});

		if(m_NonPlayers[i]->Get_NpcData().NpcID == 3)
			m_NonPlayers[i]->Get_Component<CTransform>()->Set_Pos({ 1105,0,638,1 });
	}
}

void CEndingController::Execute_Scene()
{
}

void CEndingController::Set_EnvCam()
{
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
