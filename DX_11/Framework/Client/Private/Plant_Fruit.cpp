#include "Client_Defines.h"
#include "Plant_Fruit.h"
#include "StaticModel.h"
#include "Material.h"
#include "BoneFollower.h"
#include "Child.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "ITileService.h"
#include "AABB_Collider.h"
#include "IObjectService.h"

TILE_FLAG CantDrop = TILE_FLAG::FLAG_ONITEM |TILE_FLAG::FLAG_STONE |TILE_FLAG::FLAG_TREE |TILE_FLAG::FLAG_BLOCKED;

CPlant_Fruit::CPlant_Fruit()
{
}

CPlant_Fruit::CPlant_Fruit(const CPlant_Fruit& rhs)
	:CGameObject(rhs)
{
}

HRESULT CPlant_Fruit::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	Add_Component<CBoneFollower>();
	Add_Component<CAABB_Collider>();
	return S_OK;
}

HRESULT CPlant_Fruit::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CStaticModel>()->Link_Model("GamePlay_Level", "UnitIconPltFruitApple.model");
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "UnitIconPltFruitApple.mat");
	auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_SyncedIndex = TileSystem->Get_IndexByPosition(Get_Position());
	m_MarginY = TileSystem->Get_TileSystemInfo().vWorldMin.y;
	m_InstanceTag = "Fruit";
	return S_OK;
}

void CPlant_Fruit::Priority_Update(_float dt)
{
}

void CPlant_Fruit::Update(_float dt)
{
	if(Get_Component<CBoneFollower>()->HasOwner())
		Get_Component<CBoneFollower>()->Sync_Transform(dt, m_pTransform);
	else
		Update_ByState(dt);
}

void CPlant_Fruit::Late_Update(_float dt)
{
}

void CPlant_Fruit::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		if (context.EventTag == "Pick_Up") {
			m_eState = PICKED;
 			m_pOwnerMatrix = context.Owner->Get_WorldMatrix();
			//Get_Component<CCollider>()->Set_ColliderActive(false);
		}
	}
}

void CPlant_Fruit::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CPlant_Fruit::OnCollisionExit(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		m_eState = DESTROY;
	} 
}

void CPlant_Fruit::Render_GUI()
{
	__super::Render_GUI();
	//ImGui::InputFloat3("Offset", reinterpret_cast<float*>(&m_Offset));
	//Get_Component<CBoneFollower>()->Set_Offset(XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z));

}
void CPlant_Fruit::Dangle_Fruit(const string& boneName, _float3 offset)
{
	m_Offset = offset;
	m_eState = DANGLE;
	CGameObject* pObj = Get_Component<CChild>()->Get_Parent();
	Get_Component<CBoneFollower>()->Link_Bone(pObj->Get_Component<CAnimator3D>(), boneName);
	Get_Component<CBoneFollower>()->Set_Offset(XMMatrixTranslation(offset.x, offset.y, offset.z));
}

void CPlant_Fruit::Update_ByState(_float dt)
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
	case  DESTROY:
		CGameInstance::GetInstance()->Get_ObjectMgr()->Remove_Object(this);
		m_eState = IDLE;
		break;
	}
}

const TILE_INDEX NEIGHBOR_OFFSET[9] = {
	{ 0,  0}, // CENTER
	{+1,  0}, // RIGHT
	{+1, -1}, // RIGHT_BOTTOM
	{ 0, -1}, // BOTTOM
	{-1, -1}, // BOTTOM_LEFT
	{-1,  0}, // LEFT
	{-1, +1}, // LEFT_TOP
	{ 0, +1}, // TOP
	{+1, +1}, // TOP_RIGHT
};

void CPlant_Fruit::OnGround()
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
			m_eState = IDLE;
			return;
		}
		/*위치를 찾자*/
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


void CPlant_Fruit::MoveToIndex(_float dt)
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
	movePos.y += 3*sinf(XMConvertToRadians(45)) - m_fBoundingTime*6.8f;
	if (movePos.y <= m_MarginY) {
		movePos.y = m_MarginY;
	}
	m_pTransform->Set_Pos(_float3(movePos.x, movePos.y, movePos.z));
	
}

void CPlant_Fruit::FollowHand(_float dt)
{
	_vector trans,scale,rot;
	XMMatrixDecompose(&scale, &rot, &trans, XMLoadFloat4x4(m_pOwnerMatrix));
	_float3 pos = {};
	XMStoreFloat3(&pos, trans);
	m_pTransform->Set_Pos(pos);
	//m_pTransform->TranslateMatrix(XMLoadFloat4x4(m_pOwnerMatrix));
	m_pTransform->AddScale({ -dt*1.5f,-dt * 1.5f,-dt * 1.5f });
}

CPlant_Fruit* CPlant_Fruit::Create()
{
	CPlant_Fruit* instance = new CPlant_Fruit();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CPlant_Fruit");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CPlant_Fruit::Clone(INIT_DESC* pArg)
{
	CPlant_Fruit* instance = new CPlant_Fruit(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPlant_Fruit");
		Safe_Release(instance);
	}

	return instance;
}

void CPlant_Fruit::Free()
{
	__super::Free();
}
