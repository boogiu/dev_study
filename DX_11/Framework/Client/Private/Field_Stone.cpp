#include "Field_Stone.h"
#include "Client_Defines.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "AABB_Collider.h"

#include"TileBlock.h"
#include "GameInstance.h"
#include "ITileService.h"

CField_Stone::CField_Stone()
{
}

CField_Stone::CField_Stone(const CField_Stone& rhs)
	:CFieldObject(rhs)
{
	m_InstanceTag = "Stone";
}

HRESULT CField_Stone::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	Add_Component<CAABB_Collider>();
	return S_OK;
}

HRESULT CField_Stone::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CField_Stone::Priority_Update(_float dt)
{
}

void CField_Stone::Update(_float dt)
{
	static _float dg = {};
	dg += dt * 50;
	Get_Component<CSkeletalModel>()->Control_BoneByIndex(5, XMMatrixRotationY(XMConvertToRadians(dg)));
}

void CField_Stone::Late_Update(_float dt)
{
}

void CField_Stone::Render_GUI()
{
	__super::Render_GUI();
}

HRESULT CField_Stone::Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
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
	
	tileSystem->Add_TileFlagByIndex(objHeader.Index, static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED| 
		 TILE_FLAG::FLAG_STONE));

	tileSystem->Set_Material_ID(objHeader.Index, {1,1,0,0});

	return S_OK; 
}

void CField_Stone::Override_Pass()
{
}

CField_Stone* CField_Stone::Create()
{
	CField_Stone* instance = new CField_Stone();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CField_Stone");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CField_Stone::Clone(INIT_DESC* pArg)
{
	CField_Stone* instance = new CField_Stone(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CField_Tree");
		Safe_Release(instance);
	}

	return instance;
}

void CField_Stone::Free()
{
	__super::Free();
}
