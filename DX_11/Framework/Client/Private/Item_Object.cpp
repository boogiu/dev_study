#include "Client_Defines.h"
#include "Item_Object.h"

#include "StaticModel.h"
#include "Material.h"
#include "AABB_Collider.h"
#include "GameInstance.h"

CItem_Object::CItem_Object()
{
}

CItem_Object::CItem_Object(const CItem_Object& rhs)
	:CGameObject(rhs)
{
}

HRESULT CItem_Object::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	Add_Component<CAABB_Collider>();

	return S_OK;
}

HRESULT CItem_Object::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	DROP_ITEM_DESC* pDesc = dynamic_cast<DROP_ITEM_DESC*>(pArg);

	if (pDesc != nullptr)
	{
		Get_Component<CStaticModel>()->Link_Model("GamePlay_Level", pDesc->itemDesc.modelName);
		Get_Component<CMaterial>()->Link_Material("GamePlay_Level", pDesc->itemDesc.materialName);
	}
	else {
		Get_Component<CStaticModel>()->Link_Model("GamePlay_Level", "UnitIconPltFruitApple.model");
		Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "UnitIconPltFruitApple.mat");
	}
	
	auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_SyncedIndex = TileSystem->Get_IndexByPosition(Get_Position());
	m_MarginY = TileSystem->Get_TileHeightByPosition(Get_Position());

	m_InstanceTag = "Item";

	return S_OK;
}

void CItem_Object::Priority_Update(_float dt)
{
}

void CItem_Object::Update(_float dt)
{
}

void CItem_Object::Late_Update(_float dt)
{
}

void CItem_Object::Render_GUI()
{
	__super::Render_GUI();
	ImGui::Text("nowIndex X : %d, Z : %d", m_SyncedIndex.IndexX, m_SyncedIndex.IndexZ);
}

void CItem_Object::OnCollisionEnter(COLLISION_CONTEXT context)
{

}

void CItem_Object::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CItem_Object::OnCollisionExit(COLLISION_CONTEXT context)
{

}


void CItem_Object::Find_Ground()
{

	//땅을 찾는거야.
	auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	_uint Flag = TileSystem->Get_TileFlagByIndex(m_SyncedIndex);

	/*현재 인덱스에 들어갈 수 있음*/
	if ((CantDrop & Flag) == 0)
	{
		/*위치를 찾은 상태*/
		if (m_eState == FINDED) {
			TileSystem->Add_TileFlagByIndex(m_SyncedIndex, static_cast<_uint>(TILE_FLAG::FLAG_ONITEM));
			Get_Component<CAABB_Collider>()->Make_MinMaxCollider({ {-2,-2,-2},{2,4,2} });
			m_eState = IDLE;
			return;
		}
		else {	//현재 인덱스를 아직 찾지 못한 상태
			/*위치를 찾자 -> 빠운드로 현재 인덱스의 중심 위치 검색*/
			m_eState = BOUND;
			m_DstPosition = TileSystem->Get_PositionByIndex(m_SyncedIndex, ANCHOR::Center);
			return;
		}

	}
	/*현재 인덱스 중심으로 주변 타일 검색 시작*/
	vector<TILE_INFO> infos;
	TILE_INDEX searchIndex = m_SyncedIndex;
	_uint neighborValid = TileSystem->Get_NeighborInfoByIndex(searchIndex, infos);

	/*주변 8방향 순회*/
	for (_uint i = 0; i < infos.size(); ++i)
	{
		if ((neighborValid & (1 << i)) == 0)
			continue;

		if ((CantDrop & infos[i].TileFlag) == 0)
		{
			/*이웃 중에서 찾아냄*/
			TILE_INDEX neighborIndex = {
				searchIndex.IndexX + NEIGHBOR_OFFSET[i].IndexX,
				searchIndex.IndexZ + NEIGHBOR_OFFSET[i].IndexZ
			};

			/*그 쪽으로 이동하자*/
			m_SyncedIndex = neighborIndex;
			m_eState = BOUND;
			m_DstPosition = TileSystem->Get_PositionByIndex(m_SyncedIndex, ANCHOR::Center);
			return;
		}
	}

	/*이웃 중에서도 못찾음*/
	auto TileInfo = TileSystem->Get_TileSystemInfo();

	TILE_INDEX newIndex = m_SyncedIndex;
	newIndex.IndexX += 1;
	/*X이동 불가하면 Z이동*/
	if (!TileInfo.Check_ValidIndex(newIndex)) {
		newIndex = m_SyncedIndex;
		newIndex.IndexZ += 1;
	}

	if (TileInfo.Check_ValidIndex(newIndex)) {
		/*너도 일단 이동해보자*/
		m_SyncedIndex = newIndex;
		m_eState = BOUND;
		m_DstPosition = TileSystem->Get_PositionByIndex(m_SyncedIndex, ANCHOR::Center);
	}
	else {
		m_eState = IDLE;
	}
}


void CItem_Object::MoveToIndex(_float dt)
{
	/*Bound State*/
	m_fBoundingTime += dt;

	_float4 nowPos = Get_Position();
	_vector vNow = XMLoadFloat4(&nowPos);
	_vector vDst = XMLoadFloat4(&m_DstPosition);

	// Lerp Position
	_vector vMove = XMVectorLerp(vNow, vDst, dt * 10);

	// Compare Position vDst-vNow
	_vector vDiff = XMVector3Length(XMVectorSubtract(vDst, vNow));
	_float distance = 0.f;
	XMStoreFloat(&distance, vDiff);

	//Near Position
	if (distance < 0.01f)
	{
		auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
		m_eState = FINDED; /*Change _To FInded*/
		m_fBoundingTime = 0.f;
		Find_Ground();
		return;
	}

	_float4 movePos;
	XMStoreFloat4(&movePos, vMove);
	movePos.y += 3 * sinf(XMConvertToRadians(45)) - m_fBoundingTime * 9.8f;

	if (movePos.y <= m_MarginY) {
		movePos.y = m_MarginY;
	}

	m_pTransform->Set_Pos(_float3(movePos.x, movePos.y, movePos.z));
}

void CItem_Object::FollowHand(_float dt)
{
	_vector trans, scale, rot;
	XMMatrixDecompose(&scale, &rot, &trans, XMLoadFloat4x4(m_pOwnerMatrix));
	_float3 pos = {};
	XMStoreFloat3(&pos, trans);
	m_pTransform->Set_Pos(pos);
	m_pTransform->AddScale({ -dt * 1.5f,-dt * 1.5f,-dt * 1.5f });
}

void CItem_Object::Remove_Item()
{
	auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	TileSystem->Remove_TileFlagByIndex(m_SyncedIndex, static_cast<_uint>(TILE_FLAG::FLAG_ONITEM));
}

void CItem_Object::Free()
{
	__super::Free();
}
