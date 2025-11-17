#include "Client_Defines.h"
#include "NonPlayer.h"
#include "SkeletalModel.h"
#include "Material.h"
#include "Animator3D.h"
#include "ObjectContainer.h"

#include "MaterialInstance.h"
#include "MaterialData.h"
#include "MaterialAnimator.h"


#include "NpcState_Machine.h"

#include "GameInstance.h"
#include "Level.h"
#include "Helper_Func.h"

#include "EventSystem.h"
#include "NpcSpawner.h"
#include "ItemSpawner.h"
#include "UI_Responcer.h"
#include "Player.h"
#include "ClientHelper.h"
#include "AABB_Collider.h"

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
	Add_Component<CAABB_Collider>();

	return S_OK;
}

HRESULT CNonPlayer::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	NPC_SPAWN_DATA* pDesc = static_cast<NPC_SPAWN_DATA*>(pArg);
	m_CharacterDesc = pDesc->characterDesc;

	m_pMachine = CNpcState_Machine::Create(this);

	m_InstanceTag = "NPC";
	return S_OK;
}

void CNonPlayer::Awake()
{
	Make_Model(m_CharacterDesc);
	/*경로 실수 .Model 들어감*/
	ClientHelper::Add_AllClipsByFile("../../Resources/Data/CharacterAnim.json","GamePlay_Level", m_CharacterDesc.NpcKey,
	Get_Component<CAnimator3D>());

	Get_Component<CMaterialAnimator>()->LinkAnimate_Material(Get_Component<CMaterial>());

	Get_Component<CAnimator3D>()->Change_Animation("Base_Wait.anim",false);
	Get_Component<CAABB_Collider>()->Make_MinMaxCollider({ {-5,0,-5}, {5,5,5} });

	Add_Parts();
	
	Add_EventListen();

	Add_MatAnimator();

	m_EventPack.eventSystem = CGameInstance::GetInstance()->Get_CurrentLevel()->Get_LevelObject<CEventSystem>();
}

void CNonPlayer::Priority_Update(_float dt)
{
	m_EventPack.m_InteractCoolDown += dt;
}

void CNonPlayer::Update(_float dt)
{
	Update_Movement(dt);
	Update_TileInfo(dt);
	m_pMachine->Update(dt);
	Get_Component<CMaterialAnimator>()->Update_Animation(dt);
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

	TILE_INDEX currIndex =TileSys->Get_IndexByPosition(Get_Position());

	if (false==m_TileInfoPack.NowIndex.isSame(currIndex)) {
		prevIndex = m_TileInfoPack.NowIndex;
		m_TileInfoPack.NowIndex = currIndex;
		TileSys->Add_TileFlagByIndex(m_TileInfoPack.NowIndex, static_cast<_uint>(TILE_FLAG::ONCHARACTER));
		TileSys->Remove_TileFlagByIndex(prevIndex, static_cast<_uint>(TILE_FLAG::ONCHARACTER));
	}
}

void CNonPlayer::Render_GUI()
{
	__super::Render_GUI();
	
	m_pMachine->Render_State(this);
}

HRESULT CNonPlayer::Make_Model(NPC_DATA_DESC desc)
{
	HRESULT hr = Get_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", desc.ModelName);
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", desc.MaterialName);
	Get_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", desc.ModelName);
	Get_Component<CModel>()->ShadowCast(true);

	return hr;
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
		EventSys->Add_Listner<CNonPlayer, BaseEvent>(this, &CNonPlayer::EventAction);
	};
}

void CNonPlayer::Add_MatAnimator()
{
	MATERIAL_CLIP clip = {};
	clip.AnimationKeyFrame = { 10,10,10,11,11,11,11,11,11,11 ,11,11 ,11,11 ,11,11 ,};
	clip.fDuration = 50.f;
	clip.TickperSecond = 15.f;
	clip.isLoop = false;
	Get_Component<CMaterialAnimator>()->RegisterKeyFrame("mEye", "Smile", clip);

	clip.AnimationKeyFrame = {6,6,6,6,6,6,6,7,7,8,8,8,8,8,7,7, 6,6,6,6,6,};
	clip.isLoop = true;
	Get_Component<CMaterialAnimator>()->RegisterKeyFrame("mEye", "IDLE", clip);

	Get_Component<CMaterialAnimator>()->Change_Animation("mEye", "IDLE");
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

void CNonPlayer::LookTo(_fvector pos)
{
	_vector vLook = m_pTransform->Dir(STATE::LOOK);
	vLook = XMVector3Normalize(vLook);

	// 현재 위치
	_float4 vNowPos = Get_Position();

	_vector vTargetDir = pos - XMLoadFloat4(&vNowPos);
	vTargetDir = XMVector3Normalize(vTargetDir);
	vLook = XMVectorSetY(vLook, 0.f);
	vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	// 방향 각도 계산 (Y축 기준 평면 상)
	_float angle =
		atan2(
			XMVectorGetX(vTargetDir),  // x 성분
			XMVectorGetZ(vTargetDir)   // z 성분
		) -
		atan2(

			XMVectorGetX(vLook),
			XMVectorGetZ(vLook)
		);

	if (angle > XM_PI) angle -= XM_2PI;
	if (angle < -XM_PI) angle += XM_2PI;

	m_MovementPack.fTargetDegree = m_MovementPack.fCurrentDegree + XMConvertToDegrees(angle);
}


CItem_Object* CNonPlayer::Spawn_Item(const string tag, _float3 pos)
{
	auto itemSpawner = CGameInstance::GetInstance()->Get_CurrentLevel()->Get_LevelObject<CItemSpawner>();
	return itemSpawner->SpawnItem(tag, pos);
}


void CNonPlayer::Set_Animation(const string tag)
{
	HRESULT hr = Get_Component<CAnimator3D>()->Change_Animation(tag);
	//if (FAILED(hr))
	//	Get_Component<CAnimator3D>()->Change_Animation("Base_Wait.anim");
}

void CNonPlayer::Set_Emotion(const string tag)
{
	Get_Component<CMaterialAnimator>()->Change_Animation("mEye", tag);
}

void CNonPlayer::Set_Voice(const string tag)
{
}

void CNonPlayer::Set_Camera(const string tag)
{
	if (tag.empty())
		return;

	CAM_MOVE move; move.
	eType = EVENT_TYPE::CameraMove;
	move.moveTag = tag;
	m_EventPack.eventSystem->OnBroadCast<BaseEvent>(move);
}

void CNonPlayer::Set_Closed(OnEndDialogue endMsg)
{
	if (m_EventPack.isReservedAction()) {
		m_EventPack.Reset();
	}

	m_EventPack.reservedMsg = endMsg.msg;
	m_EventPack.nextSequenceID = endMsg.msg.NextSequenceID;

	if (endMsg.NextSequenceID >=0)
		m_EventPack.nextSequenceID = endMsg.NextSequenceID;

	m_EventPack.externalCondition = endMsg.NextCondition;
}

void CNonPlayer::Serve_Order(const string& order, _uint orderer)
{
}

void CNonPlayer::EventAction(const BaseEvent& event)
{
	if (event.eType == EVENT_TYPE::Player_Pos) {
		const auto& evt = static_cast<const PLAYER_POS&>(event);
		_float fDistance = XMVectorGetX(XMVector4Length(m_pTransform->Get_Pos() - XMLoadFloat4(&evt.playerPos)));
		m_TracePack.pPlayer = evt.pPlayer;

		if (fabs(fDistance) < 11.f)
			m_TracePack.Player_Near = true;
		else
			m_TracePack.Player_Near = false;

		m_TracePack.Player_distance = fDistance;
		m_TracePack.Player_Pos = evt.playerPos;
		XMStoreFloat4(&m_TracePack.vLook_Player,
			XMVector4Normalize(XMLoadFloat4(&evt.playerPos) - m_pTransform->Get_Pos()));
	}
	if (event.eType == EVENT_TYPE::NoticeDialogue) {
		const auto& evt = static_cast<const OnNoticeDialogue&>(event);
		if (evt.pSubject != this) return; //다이얼로그 시작
		LookTo(
			Get_TracePack().pPlayer->Get_Component<CTransform>()->Get_Pos()
		);

		m_EventPack.reservedMsg.Type = "Talking";
	}

	if (event.eType == EVENT_TYPE::DialougueEnd) {
		const auto& evt = static_cast<const OnEndDialogue&>(event);
		if (evt.pSpeaker != this) return;
		Set_Closed(evt); //끝
	}

	if (event.eType == EVENT_TYPE::TransItem_Response) {
		const auto& evt = static_cast<const RESPONSE_TRANS_ITEM&>(event);
		if (evt.pSenderID != this->m_ObjectID) return;
		m_ActionPack.NextPhase();
	}

	if (event.eType == EVENT_TYPE::Npc_To_Npc) {
		const auto& evt = static_cast<const EVNET_NPC_TO_NPC&>(event);
		if (evt.Server_NPCID != this->m_CharacterDesc.NpcID) return;
		Serve_Order(evt.OrderMsg, evt.Orderer_NPCID);
	}
}

_float4x4 CNonPlayer::Get_SocketMatrix(string socketName)
{
	return Get_Component<CAnimator3D>()->Get_BoneMatrix(socketName);
}

CNonPlayer* CNonPlayer::Create()
{
	CNonPlayer* instance = new CNonPlayer();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CNonPlayer");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CNonPlayer::Clone(INIT_DESC* pArg)
{
	CNonPlayer* instance = new CNonPlayer(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CNonPlayer");
		Safe_Release(instance);
	}

	return instance;
}


void CNonPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pMachine);
}