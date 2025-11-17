#include "Field_Stone.h"
#include "Client_Defines.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "AABB_Collider.h"

#include"TileBlock.h"
#include "GameInstance.h"
#include "LevelMgr.h"
#include "GamePlayLevel.h"
#include "ItemSpawner.h"
#include "ITileService.h"
#include "Item_Object.h"
#include "Helper_Func.h"
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
	if (m_HitCount > 3)
		m_eState = READY_TO_DESTROY;
}

void CField_Stone::Update(_float dt)
{
	ItemSpawnCoolTime += dt;
	if (m_eState == HITTED)
		HittedMove(dt);
	else if (m_eState == READY_TO_DESTROY) {
		CGameInstance::GetInstance()->Get_ObjectMgr()->Remove_Object(this);
		auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
		tileSystem->Remove_TileFlagByIndex(m_SyncedIndex, static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED | TILE_FLAG::FLAG_STONE));
		m_eState = END;
	}
}

void CField_Stone::Late_Update(_float dt)
{

}

void CField_Stone::Render_GUI()
{
	__super::Render_GUI();
	ImGui::Text("Type : %d", m_iObjType);

}

HRESULT CField_Stone::Sync_MapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{

	HRESULT hr = Get_Component<CModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	m_iObjType = objHeader.Object_type;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));

	auto vector = Get_Component<CModel>()->Get_MeshBoundingBox();

	Get_Component<CAABB_Collider>()->Make_MinMaxCollider(
		{
				{-5,0,-5}, {5,10,5}
		}
	);
	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	TILE_INDEX index = tileSystem->Get_IndexByPosition(Get_Position());
	
	tileSystem->Add_TileFlagByIndex(objHeader.Index, static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED| TILE_FLAG::FLAG_STONE));
	m_SyncedIndex = objHeader.Index;
	tileSystem->Set_Material_ID(objHeader.Index, {1,1,0,0});

	return S_OK; 
}

void CField_Stone::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Scoop")) {
		if (context.EventTag == "Digged")
		{
			if (m_eState != HITTED) {

			m_eState = HITTED;
			m_vHittedPos =Get_Position();

			_vector HitterPos = context.Owner->Get_Component<CTransform>()->Get_WorldPos();
			_vector MyPos = m_pTransform->Get_Pos();
			HitterPos =XMVectorSetY(HitterPos, 0);
			MyPos = XMVectorSetY(MyPos, 0);

			_vector MoveVector = MyPos - HitterPos;
			MoveVector = XMVector4Normalize(MoveVector);
			XMStoreFloat4(&m_vMoveVector, MoveVector);

			if (ItemSpawnCoolTime > 1.f) {
				_float4 pos = Get_Position();
				_int rnd = Helper::Get_Random_Int(0, 4);
				auto spawner = CGameInstance::GetInstance()->Get_LevelMgr()->Get_CurrentLevel()->Get_LevelObject<CItemSpawner>();
				switch (rnd)
				{
				case 0 :
					 spawner->SpawnItem("UnitIconStone", { pos.x,pos.y,pos.z });
					break;
				case 1:
					spawner->SpawnItem("UnitIconStone", { pos.x,pos.y,pos.z });
					break;
				case 2:
					spawner->SpawnItem("UnitIconStone", { pos.x,pos.y,pos.z });
					break;
				case 3:
					spawner->SpawnItem("UnitIconClay", { pos.x,pos.y,pos.z });
					break;
				case 4:
					spawner->SpawnItem("UnitIconIron", { pos.x,pos.y,pos.z });
					break;
				default:
					break;
				}
				ItemSpawnCoolTime = 0;
				m_isJustHitted = true;
				m_HitCount++;
			}
			}
		}
	}
}

void CField_Stone::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CField_Stone::OnCollisionExit(COLLISION_CONTEXT context)
{
}

void CField_Stone::Override_Pass()
{
}

void CField_Stone::HittedMove(_float dt)
{
	if (m_isJustHitted) {
		/*처음 부딪히면 5만 큼 이동함*/
		m_pTransform->Translate(XMLoadFloat4(&m_vMoveVector)*20*dt);
	}

	/*맞은 곳에서부터 거리 체크*/
	_vector HittedPos = XMLoadFloat4(&m_vHittedPos);
	_vector movedDistance = m_pTransform->Get_Pos() - HittedPos;
	_vector distance = XMVector3Length(movedDistance);

	if (XMVectorGetX(distance) > 3.f) {
		m_isJustHitted = false;
	}

	if (!m_isJustHitted) {
		m_pTransform->Translate(XMLoadFloat4(&m_vMoveVector) * -20 * dt);
		if (XMVectorGetX(distance) < 0.3f) {
			m_eState = IDLE;
			m_pTransform->Set_Pos({ m_vHittedPos.x, m_vHittedPos.y, m_vHittedPos.z });
		}
	}

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
