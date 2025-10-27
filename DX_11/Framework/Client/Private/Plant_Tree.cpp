#include "Plant_Tree.h"
#include "Client_Defines.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "AABB_Collider.h"

#include"TileBlock.h"
#include "GameInstance.h"
#include "ITileService.h"
#include "IResourceService.h"

#include "Texture.h"
#include "Animator3D.h"

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
	if (m_eState == CUTTED && Get_Component<CAnimator3D>()->isCurrentAnimEnd()) {
		Get_Component<CModel>()->Link_Model("GamePlay_Level", m_StumpName);
		auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();

		tileSystem->Remove_TileFlagByIndex(m_Index, static_cast<_uint>(
			TILE_FLAG::FLAG_HANDINTERACT
			| TILE_FLAG::FLAG_TREE));
		m_eState == IDLE;
	}
}

void CPlant_Tree::Update(_float dt)
{
	Get_Component<CAnimator3D>()->Update_Animation(dt);

}

void CPlant_Tree::Late_Update(_float dt)
{
}

void CPlant_Tree::Render_GUI()
{
	__super::Render_GUI();
	if (ImGui::Button("Change_State")) {
	}
}

HRESULT CPlant_Tree::Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{
	/*00. Base*/
	m_ModelName = modelMapTable[0];
	m_BaseName = modelMapTable[1];
	
	/*01. Node*/
	m_NodeName = m_ModelName + "Node.model";

	/*02. Stump*/
	m_StumpName = m_ModelName + "Stump.model";

	CMaterial* pMaterial = Get_Component<CMaterial>();
	if (!pMaterial)
		return E_FAIL;

	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));
	Obj_Type = objHeader.Object_type;

	HRESULT hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	Get_Component<CModel>()->Link_Model("GamePlay_Level", m_NodeName);

	Get_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", m_NodeName);

	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName+"CutL0.anim", "OakTree", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName+"CutR0.anim", "OakTree", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName+"ShakeS.anim", "OakTree", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName+"ShakeM.anim", "OakTree", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName+"ShakeL.anim", "OakTree", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", m_ModelName+"ShakeLWait.anim", "OakTree", true);
	
	Get_Component<CAABB_Collider>()->Make_MinMaxCollider({{-5,0,-5}, {5,10,5}});

	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_Index = tileSystem->Get_IndexByPosition(Get_Position());

	tileSystem->Add_TileFlagByIndex(objHeader.Index, static_cast<_uint>(
		TILE_FLAG::FLAG_BLOCKED 
		| TILE_FLAG::FLAG_HANDINTERACT
		| TILE_FLAG::FLAG_TOOLINTERACT
		| TILE_FLAG::FLAG_TREE));
	tileSystem->Set_Material_ID(objHeader.Index, { 1,1,0,0 });

	for (auto instance : pMaterial->Get_Material_Instance())
	{
		instance->Override_Pass("Tree");
	}

	//auto RcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	//
	//auto trunkMat = pMaterial->Get_MaterialInstanceByName("mPltTreeOakTrunk");
	//auto leafMat = pMaterial->Get_MaterialInstanceByName("mTreeOakLeaf");
	//auto backLeafMat = pMaterial->Get_MaterialInstanceByName("mTreeOakLeafBack");
	//
	//SHADER_PARAM leafParam = {};
	//leafParam.iSize = 0;
	//leafParam.typeName = "Texture2D";
	//leafParam.pData = RcsMgr->Load_Texture("GamePlay_Level", "mPltTreeOakLeaf_Grd.0.dds")->Get_SRV();
	//
	//SHADER_PARAM trunkParam = {};
	//trunkParam.iSize = 0;
	//trunkParam.typeName = "Texture2D";
	//trunkParam.pData = RcsMgr->Load_Texture("GamePlay_Level", "mPltTreeOakTrunkColor_Grd.0.dds")->Get_SRV();
	//
	//if(leafMat)
	//	leafMat->Set_Param("g_PaletteTexture", leafParam);
	//
	//
	//
	//
	//
	//
	//if (trunkMat) {
	//	//trunkMat->Override_Pass("TreeCut");
	//	trunkMat->Set_Param("g_PaletteTexture", trunkParam);
	//}

	return S_OK;
}

void CPlant_Tree::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Axe")) {
		m_AxeHitCount++;
		isReadyToAnimate = true;
		if (m_AxeHitCount >= 3) {
			_float RLCheck = context.Owner->Get_Position().x;
			if (Get_Position().x < RLCheck) {
				Get_Component<CAnimator3D>()->Chane_Animation(m_ModelName + "CutR0.anim", true);
			}
			else {
				Get_Component<CAnimator3D>()->Chane_Animation(m_ModelName + "CutL0.anim", true);
			}
			m_eState = CUTTED;
		}
		else {
			HRESULT hr = Get_Component<CAnimator3D>()->Chane_Animation(m_ModelName + "ShakeL.anim", true);
			if (FAILED(hr)) {
				Get_Component<CAnimator3D>()->Chane_Animation(m_ModelName + "ShakeM.anim", true);
			}
		}
	}
	else if (context.Owner->Has_Tag("None"))
		HRESULT hr = Get_Component<CAnimator3D>()->Chane_Animation(m_ModelName + "ShakeM.anim", true);

	else if (context.Owner->Has_Tag("Player"))
		HRESULT hr = Get_Component<CAnimator3D>()->Chane_Animation(m_ModelName + "ShakeS.anim", true);
}

void CPlant_Tree::OnCollisionStay(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("None")) {
		if (context.EventTag == "KeepShake") {
			HRESULT hr = Get_Component<CAnimator3D>()->Chane_Animation(m_ModelName + "ShakeLWait.anim", false);
		}
	}

}

void CPlant_Tree::OnCollisionExit(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("None")) {
		HRESULT hr = Get_Component<CAnimator3D>()->Stop_Animation();
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
}
