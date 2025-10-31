#include "Field_Tree.h"
#include "Client_Defines.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "AABB_Collider.h"

#include"TileBlock.h"
#include "GameInstance.h"
#include "ITileService.h"

CField_Tree::CField_Tree()
{
}

CField_Tree::CField_Tree(const CField_Tree& rhs)
	:CFieldObject(rhs)
{
	m_InstanceTag = "Idr_Tree";
}

HRESULT CField_Tree::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	Add_Component<CAABB_Collider>();
	return S_OK;
}

HRESULT CField_Tree::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CField_Tree::Priority_Update(_float dt)
{
}

void CField_Tree::Update(_float dt)
{
	
}

void CField_Tree::Late_Update(_float dt)
{
}

void CField_Tree::Render_GUI()
{
	__super::Render_GUI();
}

HRESULT CField_Tree::Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{

	HRESULT hr = Get_Component<CModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	m_iObjType = objHeader.Object_type;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));

	auto vector = Get_Component<CModel>()->Get_MeshBoundingBox();

	/*나무 기둥의 콜라이더를 따로 가져와야함. 트렁크로?*/
	Get_Component<CAABB_Collider>()->Make_MinMaxCollider(
		{
				{-5,0,-5}, {5,10,5}
		}
	);
	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	TILE_INDEX index = tileSystem->Get_IndexByPosition(Get_Position());
	
	tileSystem->Add_TileFlagByIndex(objHeader.Index, static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED));
	tileSystem->Set_Material_ID(objHeader.Index, {1,1,0,0});

	return S_OK;
}

void CField_Tree::OnCollisionEnter(CGameObject* pObj)
{

}


void CField_Tree::Override_Pass()
{
}

CField_Tree* CField_Tree::Create()
{
	CField_Tree* instance = new CField_Tree();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CField_Tree");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CField_Tree::Clone(INIT_DESC* pArg)
{
	CField_Tree* instance = new CField_Tree(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CField_Tree");
		Safe_Release(instance);
	}

	return instance;
}

void CField_Tree::Free()
{
	__super::Free();
}
