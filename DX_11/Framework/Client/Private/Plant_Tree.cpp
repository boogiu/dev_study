#include "Client_Defines.h"
#include "Plant_Tree.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "AABB_Collider.h"
#include "ObjectContainer.h"

#include"TileBlock.h"
#include "GameInstance.h"
#include "ITileService.h"
#include "IResourceService.h"
#include "IObjectService.h"
#include "ILevelService.h"
#include "Level.h"
#include "ItemSpawner.h"

#include "Texture.h"
#include "Animator3D.h"

#include "Item_Object.h"
#include "Builder.h"
#include "Helper_Func.h"
#include "DebugRender.h"
#include "EventSystem.h"

#include "AudioSource.h"

CPlant_Tree::CPlant_Tree()
{

}

CPlant_Tree::CPlant_Tree(const CPlant_Tree& rhs)
	:CFieldObject(rhs)
{
	m_InstanceTag = "Tree";
}

HRESULT CPlant_Tree::Initialize_Prototype()
{
	auto RcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	RcsMgr->Add_ResourcePath("mPltTreeOakLeaf_Grd.0.dds", "../../Resources/Palette/mPltTreeOakLeaf_Grd.0.dds");
	RcsMgr->Add_ResourcePath("mPltTreeOakTrunkColor_Grd.0.dds", "../../Resources/Palette/mPltTreeOakTrunkColor_Grd.0.dds");

	__super::Initialize_Prototype();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	Add_Component<CAABB_Collider>();
	Add_Component<CAnimator3D>();
	Add_Component<CObjectContainer>();
	Add_Component<CDebugRender>();

	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Tree_Shake_Oak_Dry.wav", "Shake", false, SOUND_GROUP::SFX);
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Tree_Shake_Oak_Dry.wav", "ShakeKeep", true, SOUND_GROUP::SFX);
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Tree_Shake_Oak_Dry_HitAxe.wav", "Hit", false, SOUND_GROUP::SFX);
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Tree_Shake_Oak_Dry_DownLand4.wav", "Cut", false, SOUND_GROUP::SFX);
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Tree_Shake_Oak_Dry_Small.wav", "Encount", false, SOUND_GROUP::SFX);

	Get_Component<CAudioSource>()->Set_SlotVolume("Shake", 0.2f);
	Get_Component<CAudioSource>()->Set_SlotVolume("Hit", 0.2f);
	Get_Component<CAudioSource>()->Set_SlotVolume("Cut", 0.3f);
	Get_Component<CAudioSource>()->Set_SlotVolume("Encount", 0.2f);
	Get_Component<CAudioSource>()->Set_SlotVolume("ShakeKeep", 0.2f);

	return S_OK;
}

HRESULT CPlant_Tree::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	m_pTransform->Rotation({ 0,XMConvertToRadians(180),0,0 });

	return S_OK;
}

void CPlant_Tree::Priority_Update(_float dt)
{
	Check_State(dt);
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CPlant_Tree::Update(_float dt)
{
	if (m_eState == STUMP)return;
	m_fLifeTime += dt;
	m_fWind += dt;

	XMStoreFloat4x4(&m_WindMat, XMMatrixRotationZ(sinf(m_fWind)));
	Make_Fruits();
	Get_Component<CAnimator3D>()->Update_Animation(dt);
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CPlant_Tree::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CPlant_Tree::Render_GUI()
{
	__super::Render_GUI();
	if (ImGui::Button("Regenerate_Apple")) {
		Regenerate_Items();
	}

	ImGui::Text("Type : %d", m_iObjType);
}

HRESULT CPlant_Tree::Sync_MapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{
	/*00. Base*/
	Normalize_Name(modelMapTable[0]);

	char GrownLevel = m_ModelName.back();
	if (isdigit(m_ModelName.back())) {
		m_iGrownLevel = m_ModelName.back() - '0';
		if (m_iGrownLevel < 4) {
			int i = 0;
		}
		m_TypeName = m_ModelName.substr(0, m_ModelName.size() - 1);
	}

	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));
	m_iObjType = objHeader.Object_type;
	m_AdditionalData = objHeader.AdditionalData;

	HRESULT hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", m_ModelName + "Node.mat");

	hr = Get_Component<CModel>()->Link_Model("GamePlay_Level", m_ModelName + "Node.model");
	if (FAILED(hr)) {
		return E_FAIL;
	}


	Get_Component<CAABB_Collider>()->Make_MinMaxCollider({ {-5,0,-5}, {5,10,5} });

	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_Index = tileSystem->Get_IndexByPosition(Get_Position());

	tileSystem->Add_TileFlagByIndex(objHeader.Index, static_cast<_uint>(
		TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_TREE));

	tileSystem->Set_Material_ID(objHeader.Index, { 1,1,0,0 });

	Add_Animation();
	Adjust_Material();

	if (!m_AdditionalData.empty()) {
		auto eventSys = CGameInstance::GetInstance()->Get_CurrentLevel()->Get_LevelObject<CEventSystem>();
		eventSys->Add_Listner<CPlant_Tree, BaseEvent>(this, &CPlant_Tree::Remove_Additional);
	}
	return S_OK;
}

void CPlant_Tree::Remove_Additional(const BaseEvent& event)
{
	if (event.eType == EVENT_TYPE::Quest_Msg_Responese) {
		const auto& evt = static_cast<const QUEST_RESPONSE&>(event);
		if (evt.pEventPublisher != this)
			return;

		if (evt.EvtConsumed == true) {
			if(m_AdditionalData == "Event01_TreeBlock")
				m_AdditionalData = {"Event02_TreeChoped"};
			else
				m_AdditionalData = {};
		}
	}
}

void CPlant_Tree::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Axe")) {
		m_eState = HITTED;
		if (m_AxeHitCount >= m_iGrownLevel)/*성장 단계 1->2->3->4*/
		{
			m_eState = CUTTED;
			_float RLCheck = context.Owner->Get_Position().x;
			m_isTargetRight = Get_Position().x < RLCheck;
			m_isCutted = true;
			Get_Component<CAudioSource>()->Play("Cut");
		}
		else {
			Get_Component<CAudioSource>()->Play("Hit");
		}
	}

	if (context.Owner->Has_Tag("Scoop")) {
		if (context.EventTag == "Digged")
			m_eState = DIGGED;
	}

	else if (context.Owner->Has_Tag("Player_Hand")) {
		m_eState = SHAKE;
		Get_Component<CAudioSource>()->Play("Shake");
	}

	else if (context.Owner->Has_Tag("Player")) {
		m_eState = ENCOUNTERED;
		Get_Component<CAudioSource>()->Play("Encount");
		if (m_AdditionalData=="Event01_TreeBlock") {
			auto eventSys  = CGameInstance::GetInstance()->Get_CurrentLevel()->Get_LevelObject<CEventSystem>();
			eventSys->OnBroadCast<BaseEvent>(QUEST_MSG{ EVENT_TYPE::Quest_Msg, this, m_AdditionalData });
		}
	}
}

void CPlant_Tree::OnCollisionStay(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		if (context.EventTag == "KeepShake") {
			m_eState = SHAKING;
		}
	}
}

void CPlant_Tree::OnCollisionExit(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		Get_Component<CAnimator3D>()->Stop_Animation();
		Get_Component<CAudioSource>()->Set_SlotPuase("ShakeKeep",true);
		m_eState = IDLE;
	}
}

void CPlant_Tree::Check_State(_float dt)
{
	switch (m_eState) {
	case IDLE:
		break;
	case HITTED:
		m_AxeHitCount++;
		PlayAnim_Hit();
		m_eState = IDLE;
		break;
	case CUTTED:
		PlayAnim_Cut();
		break;
	case SHAKE:
		PlayAnim_Shake();
		break;
	case SHAKING:
		m_fShakeTime += dt;
		PlayAnim_Shaking();
		Get_Component<CAudioSource>()->Play("ShakeKeep");
		break;
	case ENCOUNTERED:
		PlayAnim_Encounter();
		m_eState = IDLE;
		break;
	case DIGGED:
		Digged_Self(dt);
		break;
	}
}

void CPlant_Tree::PlayAnim_Cut()
{
	if (m_isCutted) {
		if (m_isTargetRight) {
			Get_Component<CAnimator3D>()->Change_Animation(m_ModelName + "CutR0.anim", false);
		}
		else {
			Get_Component<CAnimator3D>()->Change_Animation(m_ModelName + "CutL0.anim", false);
		}
		m_isCutted = false;
	}

	if (Get_Component<CAnimator3D>()->isCurrentAnimEnd()) {
		Get_Component<CModel>()->Link_Model("GamePlay_Level", m_ModelName + "Stump.model");
		auto spawner = CGameInstance::GetInstance()->Get_LevelMgr()->Get_CurrentLevel()->Get_LevelObject<CItemSpawner>();
		spawner->ThrowItem("UnitIconPltWood", m_pTransform->Get_Pos(), m_pTransform->Dir(STATE::LOOK));
		spawner->ThrowItem("UnitIconPltWood", m_pTransform->Get_Pos(), m_pTransform->Dir(STATE::RIGHT));
		m_eState = STUMP;
		m_InstanceTag = "Stump";
	}

}

void CPlant_Tree::PlayAnim_Hit()
{
	HRESULT hr = Get_Component<CAnimator3D>()->Change_Animation(m_ModelName + "ShakeM.anim", true);
	if (FAILED(hr)) {
		hr = Get_Component<CAnimator3D>()->Change_Animation(m_ModelName + "ShakeS.anim", true);
	}
	if (m_HasFruit) {
		Drop_Items();
	}
	else {
		_int rnd = Helper::Get_Random_Int(0, 5);
		if(rnd >4)
			Drop_Items();
	}
}

void CPlant_Tree::PlayAnim_Shake()
{
	if (m_iGrownLevel >= 2) {
		Get_Component<CAnimator3D>()->Change_Animation(m_ModelName + "ShakeM.anim", false);
	}
	else {
		Get_Component<CAnimator3D>()->Change_Animation(m_ModelName + "ShakeS.anim", false);
	}
}

void CPlant_Tree::PlayAnim_Shaking()
{

	if (m_fShakeTime > 1.5)
	{
		Drop_Items();
		m_fShakeTime = 0;
	}
	if (m_fShakeTime > 0.033) {
		return;
	}
	if (m_iGrownLevel >= 2) {
		Get_Component<CAnimator3D>()->Change_Animation(m_ModelName + "ShakeLWait.anim", false);
	}
	else {
		Get_Component<CAnimator3D>()->Change_Animation(m_ModelName + "ShakeS.anim", false);
	}
}

void CPlant_Tree::PlayAnim_Encounter()
{
	HRESULT hr = Get_Component<CAnimator3D>()->Change_Animation(m_ModelName + "ShakeS.anim", true);
}

void CPlant_Tree::Make_Fruits()
{
	if (m_iGrownLevel < 4) return;
	if (m_isCutted == true) return;
	if (m_eState != IDLE) return;
	if (m_HasFruit == true) return;
	if (m_fLifeTime < 15.f) return;
	if (m_InstanceTag =="Stump") return;

	if (m_pFruits[0] == nullptr) {
		for (size_t i = 0; i < 3; i++)
		{
			if (m_pFruits[i] != nullptr) continue;
			auto spawner = CGameInstance::GetInstance()->Get_LevelMgr()->Get_CurrentLevel()->Get_LevelObject<CItemSpawner>();
			CItem_Object* pFruit = spawner->SpawnItem("UnitIconPltFruitApple");
			if (pFruit) {
				m_pFruits[i] = pFruit;
				Get_Component<CObjectContainer>()->Add_Child(pFruit, false);
			}
		}

		m_pFruits[0]->Dangle_Item("Armature_PlantTop", { 0,25,-4 });
		m_pFruits[1]->Dangle_Item("Armature_Plant01", { 5,15,-8 });
		m_pFruits[2]->Dangle_Item("Armature_Plant02", { -5,15,-8 });
	}
	else {
		Regenerate_Items();
	}
	m_fLifeTime = 15.f;
	m_HasFruit = true;
}

void CPlant_Tree::Adjust_Material()
{
	Get_Component<CSkeletalModel>()->Hide_MehsByName("Shadow");

	auto pMaterial = Get_Component<CMaterial>();
	if (!pMaterial) return;
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	auto TruckInstance = pMaterial->Get_MaterialInstanceByName("m" + m_TypeName + "Trunk");

	if (TruckInstance) {
		SHADER_PARAM palette = {};
		palette.iSize = 0;
		palette.typeName = "Texture2D";
		palette.pData = pRcsMgr->Load_Texture("GamePlay_Level", "Palette_mPltTreeOakTrunkColor_Grd.png")->Get_SRV();
		TruckInstance->Set_Param("GradationTexture", palette);
		TruckInstance->Override_Pass("Tree");
	}

	SHADER_PARAM Leaf = {};
	Leaf.iSize = sizeof(_float2);
	Leaf.typeName = "float2";
	Leaf.pData = &LeafPalette;

	auto LeafInstance = pMaterial->Get_MaterialInstanceByName("mTreeOakLeaf");

	if (LeafInstance) {
		SHADER_PARAM palette = {};
		palette.iSize = 0;
		palette.typeName = "Texture2D";
		palette.pData = pRcsMgr->Load_Texture("GamePlay_Level", "Palette_mPltTreeOakLeafColor_Grd.png")->Get_SRV();

		LeafInstance->Set_Param("GradationTexture", palette);
		SHADER_PARAM wind = {&m_WindMat, "float4x4",sizeof(_float4x4)};
		LeafInstance->Set_Param("fWind_Matrix", wind);
		LeafInstance->Override_Pass("Leaf");
	}

	auto BackLeafInstance = pMaterial->Get_MaterialInstanceByName("mTreeOakLeafBack");

	if (BackLeafInstance) {
		SHADER_PARAM palette = {};
		palette.iSize = 0;
		palette.typeName = "Texture2D";
		palette.pData = pRcsMgr->Load_Texture("GamePlay_Level", "Palette_mPltTreeOakLeafColor_Grd.png")->Get_SRV();
		BackLeafInstance->Set_Param("GradationTexture", palette);
		BackLeafInstance->Override_Pass("Leaf");
	}
}

void CPlant_Tree::Drop_Items()
{
	if (m_iGrownLevel <= 3) return;

	if (m_HasFruit) {
		for (size_t i = 0; i < 3; i++)
		{
			m_pFruits[i]->Get_Component<CModel>()->Set_CompActive(false);

			_float4 pos = m_pFruits[i]->Get_Position();

			auto spawner = CGameInstance::GetInstance()->Get_LevelMgr()->Get_CurrentLevel()->Get_LevelObject<CItemSpawner>();
			spawner->ThrowItem("UnitIconPltFruitApple", XMLoadFloat4(&pos), {0,-1,0,0});

		}
		m_fLifeTime = 0;
		m_HasFruit = false;
	}
	else {
		auto spawner = CGameInstance::GetInstance()->Get_LevelMgr()->Get_CurrentLevel()->Get_LevelObject<CItemSpawner>();
		spawner->ThrowItem("UnitIconPltBranch", m_pTransform->Get_Pos(), m_pTransform->Dir(STATE::LOOK));
		m_fLifeTime = 0;
	}
}

void CPlant_Tree::Regenerate_Items()
{
	m_HasFruit = true;

	for (size_t i = 0; i < 3; i++)
	{
		m_pFruits[i]->Get_Component<CModel>()->Set_CompActive(true);
	}
}

void CPlant_Tree::Digged_Self(_float dt)
{
	_float speed = dt * -4;
	m_pTransform->AddScale({ speed,speed,speed });
	_float scale = XMVectorGetX(XMVector3Length(m_pTransform->Get_Scale()));
	if (scale < 0.1f) {
		CGameInstance::GetInstance()->Get_ObjectMgr()->Remove_Object(this);
		auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
		tileSystem->Remove_TileFlagByIndex(m_Index, static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_TREE));
		if (m_AdditionalData == "Event02_TreeChoped") {
			
			for (int i = 0; i < 8; ++i)
			{
				TILE_INDEX ni = { m_Index.IndexX + NEIGHBOR_OFFSET[i].IndexX,
								  m_Index.IndexZ + NEIGHBOR_OFFSET[i].IndexZ };

				tileSystem->Remove_TileFlagByIndex(ni,
					static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED)
				);
			}
			auto eventSys = CGameInstance::GetInstance()->Get_CurrentLevel()->Get_LevelObject<CEventSystem>();
			eventSys->OnBroadCast<BaseEvent>(QUEST_MSG{ EVENT_TYPE::Quest_Msg, this, m_AdditionalData });
		}
	}
}

void CPlant_Tree::Normalize_Name(const string& modelName)
{
	string BaseModel = modelName;

	// "Node" 제거
	size_t pos = BaseModel.find("Node");
	if (pos != string::npos) {
		BaseModel.erase(pos, 4);   // "Node" 길이 = 4
	}

	// "Stump" 제거
	pos = BaseModel.find("Stump");
	if (pos != string::npos) {
		BaseModel.erase(pos, 5);   // "Stump" 길이 = 5
	}

	m_ModelName = BaseModel;
}



void CPlant_Tree::Add_Animation()
{
	Get_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", m_ModelName + "Node.model");

	if (m_iGrownLevel >= 1) {
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName + "CutL0.anim", "OakTree", false);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName + "CutR0.anim", "OakTree", false);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName + "ShakeS.anim", "OakTree", false);
	}
	if (m_iGrownLevel >= 2) {
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName + "ShakeM.anim", "OakTree", false);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName + "ShakeL.anim", "OakTree", false);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName + "ShakeLWait.anim", "OakTree", true);
	}
	if (m_iGrownLevel >= 3) {
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName + "CutL1.anim", "OakTree", false);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName + "CutR1.anim", "OakTree", false);
	}
}

CPlant_Tree* CPlant_Tree::Create()
{
	CPlant_Tree* instance = new CPlant_Tree();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CPlant_Tree");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CPlant_Tree::Clone(INIT_DESC* pArg)
{
	CPlant_Tree* instance = new CPlant_Tree(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPlant_Tree");
		Safe_Release(instance);
	}

	return instance;
}

void CPlant_Tree::Free()
{
	__super::Free();
	auto eventSys = CGameInstance::GetInstance()->Get_CurrentLevel()->Get_LevelObject<CEventSystem>();
	eventSys->UnregisterAll(this);
}
