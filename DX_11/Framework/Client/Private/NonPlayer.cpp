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

#include "GameInstance.h"
#include "Level.h"
#include "EventSystem.h"
#include "Helper_Func.h"

#include"ILevelService.h"
#include "Level.h"
#include "EventSystem.h"
#include "UI_Responcer.h"
#include "Player.h"

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
	Add_Parts();
	Add_EventListen();
	Get_Component<CModel>()->ShadowCast(true);
	//m_MovementPack.vMoveAxis = { 1.f,0.f };

	m_EventPack.eventSystem = CGameInstance::GetInstance()->Get_CurrentLevel()->Get_LevelObject<CEventSystem>();

}

void CNonPlayer::Priority_Update(_float dt)
{
}

void CNonPlayer::Update(_float dt)
{
	Update_Movement(dt);
	Update_TileInfo(dt);
}

void CNonPlayer::Late_Update(_float dt)
{
}

void CNonPlayer::Update_Movement(_float dt)
{
	_float2 moveAxis = m_MovementPack.vMoveAxis;

	if (fabs(moveAxis.x) > 0.01f || fabs(moveAxis.y) > 0.01f)
		m_MovementPack.fTargetDegree = XMConvertToDegrees(atan2(moveAxis.x, moveAxis.y));
	
	_float DeltaDegree = m_MovementPack.fTargetDegree - m_MovementPack.fCurrentDegree;
	
	// -180~180 범위로 정규화
	while (DeltaDegree > 180.f) DeltaDegree -= 360.f;
	while (DeltaDegree < -180.f) DeltaDegree += 360.f;
	
	_float RotSpeed = dt * 350;
	
	if (fabs(DeltaDegree) > 6.f) {
		m_MovementPack.fCurrentDegree += (DeltaDegree > 0 ? RotSpeed : -RotSpeed);
	}
	else {
		m_MovementPack.fCurrentDegree = m_MovementPack.fTargetDegree;
	}
	
	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	m_MovementPack.fCharacterHeight = (TileSys->Get_TileHeightByPosition(Get_Position()) - Get_Position().y);
	m_pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(m_MovementPack.fCurrentDegree));
}

void CNonPlayer::Update_TileInfo(_float dt){
	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	m_TileInfoPack.NowIndex = TileSys->Get_IndexByPosition(Get_Position());
	TileSys->Get_NeighborInfoByIndex(m_TileInfoPack.NowIndex, m_TileInfoPack.infos);
	m_TileInfoPack.infos.resize(9);

	vector<TILE_INFO> worldInfo = {};
	_uint ValidIndex = TileSys->Get_NeighborInfoByIndex(m_TileInfoPack.NowIndex, worldInfo);

	_float yaw = fmodf(m_MovementPack.fCurrentDegree, 360.f);

	if (yaw < 0.f)
		yaw += 360.f;

	_uint RotCount = static_cast<_uint>(yaw / 45.f) % 8;

	for (size_t i = 0; i < 9; i++)
	{
		NEIGHBOR_INDEX neighbor = Get_Negihbor(i);
		if ((ValidIndex & neighbor) != 0) {
			m_TileInfoPack.infos[Get_Index(Rotate45_CCW(neighbor, RotCount))] = worldInfo[Get_Index(neighbor)];
			m_TileInfoPack.neighboValidFlag |= static_cast<_uint>(Rotate45_CCW(neighbor, RotCount));
		}
	}
}

void CNonPlayer::Render_GUI()
{
	__super::Render_GUI();
	ImGui::Begin("Npc_State");
	ImGui::Text("MoveAxis : %.2f ,  %.2f", m_MovementPack.vMoveAxis.x, m_MovementPack.vMoveAxis.y);

	ImGui::Checkbox("Has Agenda", &m_EventPack.HasAgenda);
	m_pMachine->Render_State(this);
	ImGui::End();
}

void CNonPlayer::Add_BaseAnimClip()
{
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Base_Wait.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Move_Walk_F.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Act_Head_Base.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Act_PlaneRun.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Act_SmellStart.anim",  "NPC", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Act_SmellKeep.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Feel_AngryWait.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Feel_AngryWalk.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Hit.anim",  "NPC", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Hit_Behind.anim",  "NPC", false);
	
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Act_Rhythm03.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Act_WatchCStd.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Act_WatchL2Std.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Act_WatchL3Std.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Act_Yoga01.anim",  "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Greeting_Bow.anim", "NPC", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Npc_Act_Jogging.anim", "NPC", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "MaRe_Clapping.anim", "NPC", true);


	Get_Component<CAnimator3D>()->Change_Animation("Base_Wait.anim", false);
}

void CNonPlayer::Add_Parts()
{

	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Get_MaterialData()->Link_Shader("GamePlay_Level", "PlayerShader.hlsl");
	}

	if (CMaterialInstance* EyeInstance = Get_Component<CMaterial>()->Find_MaterialByName("mEye")) {
		EyeInstance->Override_Pass("EyeShader");
	}
}

void CNonPlayer::Add_EventListen()
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	if (nowLevel)
	{

		auto EventSys = nowLevel->Get_LevelObject<CEventSystem>();

		EventSys->Add_Listner<PLAYER_POS>(
			[this](const PLAYER_POS& pos) {
				PLAYER_POS Playerpos = pos;
				_float fDistance = XMVectorGetX(XMVector4Length(m_pTransform->Get_Pos() - XMLoadFloat4(&Playerpos.playerPos)));
				m_TracePack.pPlayer = pos.pPlayer;
				if (fabs(fDistance) <11.f)
					m_TracePack.Player_Near = true;
				else
					m_TracePack.Player_Near = false;

				m_TracePack.Player_distance = fDistance;
				m_TracePack.Player_Pos = pos.playerPos;
				XMStoreFloat4(&m_TracePack.vLook_Player, 
					XMVector4Normalize(XMLoadFloat4(&Playerpos.playerPos) - m_pTransform->Get_Pos()));
			});
	};
}

void CNonPlayer::LookToPlayer(_float dt)
{
	auto& trace = m_TracePack;
	auto Animator = Get_Component<CAnimator3D>();

	auto Lerp = [&](_float a, _float b,_float t) ->_float{
			return a + (b - a) * t;
		};

	if (trace.Player_distance < 50)
	{
		_vector playerLook = XMLoadFloat4(&trace.Player_Pos) - m_pTransform->Get_Pos();
		_vector MyLook = m_pTransform->Dir(STATE::LOOK);
		playerLook = XMVector3Normalize(playerLook);
		MyLook = XMVector3Normalize(MyLook);

		_float dot = XMVectorGetX(XMVector3Dot(MyLook, playerLook));

		if(dot >0.3f){
		_float target_Radian = 
			atan2(XMVectorGetX(playerLook), XMVectorGetZ(playerLook)) -
			atan2(XMVectorGetX(MyLook), XMVectorGetZ(MyLook));
		if (target_Radian > XM_PI)
			target_Radian -= XM_2PI;
		else if (target_Radian < -XM_PI)
			target_Radian += XM_2PI;
			trace.traceBone_Radian = Lerp(trace.traceBone_Radian, target_Radian,dt*5);
			Animator->Control_Bone("Armature_Neck", XMMatrixRotationX(trace.traceBone_Radian));
		}
		else {
			trace.traceBone_Radian = Lerp(trace.traceBone_Radian, 0, dt);
			Animator->Control_Bone("Armature_Neck", XMMatrixRotationX(trace.traceBone_Radian));
		}
		
	}
	else {
		trace.traceBone_Radian = Lerp(trace.traceBone_Radian, 0, dt);
		Animator->Control_Bone("Armature_Neck", XMMatrixRotationX(trace.traceBone_Radian));
	}
}

void CNonPlayer::Open_Dialogue(const string tag, void* pArg)
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	auto UI_Responder = nowLevel->Get_LevelObject<CUI_Responcer>();
	UI_Responder->Active_UI(tag, pArg);
}

void CNonPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pMachine);
}