#include "Client_Defines.h"
#include "Item_Fruit.h"

#include "StaticModel.h"
#include "Material.h"
#include "AABB_Collider.h"

#include "BoneFollower.h"

#include "Child.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "ITileService.h"

#include "IObjectService.h"


CItem_Fruit::CItem_Fruit()
{
}

CItem_Fruit::CItem_Fruit(const CItem_Fruit& rhs)
	:CItem_Object(rhs)
{
}

HRESULT CItem_Fruit::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CBoneFollower>();
	return S_OK;
}

HRESULT CItem_Fruit::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_InstanceTag += "Fruit";
	return S_OK;
}

void CItem_Fruit::Priority_Update(_float dt)
{
}

void CItem_Fruit::Update(_float dt)
{
	if (Get_Component<CBoneFollower>()->HasOwner())
		Get_Component<CBoneFollower>()->Sync_Transform(dt, m_pTransform);
	else
		Update_ByState(dt);
}

void CItem_Fruit::Late_Update(_float dt)
{
}

void CItem_Fruit::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		if (context.EventTag == "Pick_Up") {
			m_eState = PICKED;
			m_pOwnerMatrix = context.Owner->Get_WorldMatrix();
		}
	}
}

void CItem_Fruit::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CItem_Fruit::OnCollisionExit(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		m_eState = DESTROY;
	}
}

void CItem_Fruit::Render_GUI()
{
	__super::Render_GUI();
}
void CItem_Fruit::Dangle_Fruit(const string& boneName, _float3 offset)
{
	m_Offset = offset;
	m_eState = DANGLE;
	CGameObject* pObj = Get_Component<CChild>()->Get_Parent();
	Get_Component<CBoneFollower>()->Link_Bone(pObj->Get_Component<CAnimator3D>(), boneName);
	Get_Component<CBoneFollower>()->Set_Offset(XMMatrixTranslation(offset.x, offset.y, offset.z));
}

void CItem_Fruit::Update_ByState(_float dt)
{
	switch (m_eState) {
	case DROP: {
		m_pTransform->Translate({ 0,-dt * 35,0 });
		if (Get_Position().y <= m_MarginY) {
			OnGround();
		}
	}
			 break;
	case  BOUND:
		MoveToIndex(dt);
		break;
	case  PICKED:
		FollowHand(dt);
		break;
	case  DESTROY: {
		CGameInstance::GetInstance()->Get_ObjectMgr()->Remove_Object(this);
	}
				 m_eState = IDLE;
				 break;
	}
}


void CItem_Fruit::OnGround()
{
	auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	_uint Flag = TileSystem->Get_TileFlagByIndex(m_SyncedIndex);

	/*현재 인덱스에 들어갈 수 있음*/
	if ((CantDrop & Flag) == 0)
	{

		/*위치를 찾은 상태*/
		if (m_eState == FINDED) {
			TileSystem->Add_TileFlagByIndex(m_SyncedIndex, static_cast<_uint>(TILE_FLAG::FLAG_ONITEM));
			Get_Component<CAABB_Collider>()->Make_MinMaxCollider({ {-2,-2,-2},{2,4,2} });
			m_eState = IDLE; /*여러번 튕기는게 잘 안된다 점검하자.*/
			return;
		}

		/*위치를 찾자 -> 빠운드로 현재 인덱스의 중심 위치 검색*/
		m_eState = BOUND;
		m_DstPosition = TileSystem->Get_PositionByIndex(m_SyncedIndex, ANCHOR::Center);
		return;
	}

	/*현재 인덱스에 들어갈 수 없음*/
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


void CItem_Fruit::MoveToIndex(_float dt)
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
		OnGround();
		return;
	}

	_float4 movePos;
	XMStoreFloat4(&movePos, vMove);
	movePos.y += 3 * sinf(XMConvertToRadians(45)) - m_fBoundingTime * 6.8f;
	if (movePos.y <= m_MarginY) {
		movePos.y = m_MarginY;
	}
	m_pTransform->Set_Pos(_float3(movePos.x, movePos.y, movePos.z));

}

void CItem_Fruit::FollowHand(_float dt)
{
	_vector trans, scale, rot;
	XMMatrixDecompose(&scale, &rot, &trans, XMLoadFloat4x4(m_pOwnerMatrix));
	_float3 pos = {};
	XMStoreFloat3(&pos, trans);
	m_pTransform->Set_Pos(pos);
	//m_pTransform->TranslateMatrix(XMLoadFloat4x4(m_pOwnerMatrix));
	m_pTransform->AddScale({ -dt * 1.5f,-dt * 1.5f,-dt * 1.5f });
}

CItem_Fruit* CItem_Fruit::Create()
{
	CItem_Fruit* instance = new CItem_Fruit();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CPlant_Fruit");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CItem_Fruit::Clone(INIT_DESC* pArg)
{
	CItem_Fruit* instance = new CItem_Fruit(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPlant_Fruit");
		Safe_Release(instance);
	}

	return instance;
}

void CItem_Fruit::Free()
{
	__super::Free();
}
