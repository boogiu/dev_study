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

	return S_OK;
}

void CPlant_Tree::Priority_Update(_float dt)
{
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
}

HRESULT CPlant_Tree::Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{

	HRESULT hr = Get_Component<CModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	CMaterial* pMaterial = Get_Component<CMaterial>();
	if (!pMaterial)
		return E_FAIL;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));

	/*트리의 경우 다르게 로딩 필요, -> 대표 이름 가져오고, 그 이름 별로 트렁크, 노드. 그냥 로드해버리기.*/
	if (modelMapTable[0] =="PltTreeOak4Node") {
	Get_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", modelMapTable[1]);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "PltTreeOak4CutR0.anim", "OakTree", true);
	Get_Component<CAnimator3D>()->Chane_Animation("PltTreeOak4CutR0.anim");
	}
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	Obj_Type = objHeader.Object_type;

	/*나무 기둥의 콜라이더를 따로 가져와야함. 트렁크로?*/
	Get_Component<CAABB_Collider>()->Make_MinMaxCollider(
		{
				{-5,0,-5}, {5,10,5}
		}
	);

	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	TILE_INDEX index = tileSystem->Get_IndexByPosition(Get_Position());

	tileSystem->Add_TileFlagByIndex(objHeader.Index, static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_TOOLINTERACT | TILE_FLAG::FLAG_TREE));
	tileSystem->Set_Material_ID(objHeader.Index, { 1,1,0,0 });


	auto RcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	auto trunkMat = pMaterial->Get_MaterialInstanceByName("mPltTreeOakTrunk");
	auto leafMat = pMaterial->Get_MaterialInstanceByName("mTreeOakLeaf");
	auto backLeafMat = pMaterial->Get_MaterialInstanceByName("mTreeOakLeafBack");

	SHADER_PARAM leafParam = {};
	leafParam.iSize = 0;
	leafParam.typeName = "Texture2D";
	leafParam.pData = RcsMgr->Load_Texture("GamePlay_Level", "mPltTreeOakLeaf_Grd.0.dds")->Get_SRV();

	SHADER_PARAM trunkParam = {};
	trunkParam.iSize = 0;
	trunkParam.typeName = "Texture2D";
	trunkParam.pData = RcsMgr->Load_Texture("GamePlay_Level", "mPltTreeOakTrunkColor_Grd.0.dds")->Get_SRV();

	if(leafMat)
		leafMat->Set_Param("g_PaletteTexture", leafParam);
	

	for (auto instance : pMaterial->Get_Material_Instance())
	{
		instance->Override_Pass("Tree");
	}
	if (trunkMat) {
		//trunkMat->Override_Pass("TreeCut");
		trunkMat->Set_Param("g_PaletteTexture", trunkParam);
	}
	return S_OK;
}

void CPlant_Tree::OnCollisionEnter(CGameObject* pObj)
{
	if (pObj->Has_Tag("Axe")) {
 		int i = 0;
	}
}


void CPlant_Tree::Override_Pass()
{
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
