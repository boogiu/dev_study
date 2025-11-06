#include "Client_Defines.h"
#include "NonPlayer.h"
#include "SkeletalModel.h"
#include "Material.h"
#include "Animator3D.h"
#include "ObjectContainer.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"
#include "MaterialAnimator.h"


#include "NpcState_Machine.h"
#include "LayerState.h"
#include "NpcState.h"
#include "NpcState_Move.h"

CNonPlayer::CNonPlayer()
{
}

CNonPlayer::CNonPlayer(const CNonPlayer& rhs)
	:CGameObject(rhs)
{
}

HRESULT CNonPlayer::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	Add_Component<CAnimator3D>();
	Add_Component<CObjectContainer>();
	Add_Component<CMaterialAnimator>();

	return S_OK;
}

HRESULT CNonPlayer::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_pMachine = CNpcState_Machine::Create(this);
	m_InstanceTag = "NPC";
	return S_OK;
}

void CNonPlayer::Awake()
{
	Add_BaseAnimClip();
	Get_Component<CAnimator3D>()->Change_Animation("Base_Wait.anim", false);


	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Get_MaterialData()->Link_Shader("GamePlay_Level", "PlayerShader.hlsl");
	}

	if (CMaterialInstance* EyeInstance = Get_Component<CMaterial>()->Find_MaterialByName("mEye")) {
		EyeInstance->Override_Pass("EyeShader");
	}
	
	Get_Component<CAnimator3D>()->Change_Animation("Base_Wait.anim");
}

void CNonPlayer::Render_GUI()
{
	ImGui::Begin("Npc_State");
	m_pMachine->Render_State(this);
	ImGui::End();
}

void CNonPlayer::Add_BaseAnimClip()
{
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Base_Wait.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Move_Walk_L.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Act_SmellStart.anim",  "NPC", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Act_SmellKeep.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Feel_AngryWait.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Feel_AngryWalk.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Hit.anim",  "NPC", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Hit_Behind.anim",  "NPC", false);
}

void CNonPlayer::Add_Parts()
{
}

void CNonPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pMachine);
}