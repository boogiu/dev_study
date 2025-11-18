#include "Client_Defines.h"
#include "Item_Object.h"
#include "BoneFollower.h"
#include "StaticModel.h"
#include "Material.h"
#include "AABB_Collider.h"
#include "GameInstance.h"
#include "Child.h"
#include "Animator3D.h"
#include "MaterialData.h"
#include "MaterialInstance.h"
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
	Add_Component<CBoneFollower>();

	return S_OK;
}

HRESULT CItem_Object::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CModel>()->ShadowCast(true);

	DROP_ITEM_DESC* pDesc = dynamic_cast<DROP_ITEM_DESC*>(pArg);
	//pDesc->itemDesc.
	if (pDesc != nullptr)
	{
		Get_Component<CStaticModel>()->Link_Model("GamePlay_Level", pDesc->itemDesc.modelName);
		Get_Component<CMaterial>()->Link_Material("GamePlay_Level", pDesc->itemDesc.materialName);

		if (!pDesc->itemDesc.Additionaldata.empty()) {
			Get_Component<CStaticModel>()->Link_Model("GamePlay_Level", pDesc->itemDesc.Additionaldata+".model");
			Get_Component<CMaterial>()->Link_Material("GamePlay_Level", pDesc->itemDesc.Additionaldata+".mat");
		}
		m_ItemInfo = pDesc->itemDesc;
	}

	auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_SyncedIndex = TileSystem->Get_IndexByPosition(Get_Position());
	m_MarginY = TileSystem->Get_TileHeightByPosition(Get_Position());
	m_InstanceTag = "Item";
	Check_Palette();

	return S_OK;
}

void CItem_Object::Priority_Update(_float dt)
{
}

void CItem_Object::Update(_float dt)
{
	if (IsDangled)
	{
		if (Get_Component<CBoneFollower>()->HasOwner())
			Get_Component<CBoneFollower>()->Sync_Transform(dt, m_pTransform);
	}
	else
		Update_ByState(dt);
}

void CItem_Object::Late_Update(_float dt)
{
}

void CItem_Object::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		if (context.EventTag == "Pick_Up") {
			if(!m_pOwnerMatrix){
				m_eState = PICKED;
				m_pOwnerMatrix = context.Owner->Get_WorldMatrix();
				Get_Component<CCollider>()->Set_ContextEvent("Picked");
			}
		}
	}
}

void CItem_Object::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CItem_Object::OnCollisionExit(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Player_Hand")) {
		m_eState = READY_DESTROY;
	}
}

/*Y값 보정됨. 위에서 아래로 떨어지는 형태로*/
void CItem_Object::Set_Throw(_fvector StartPos, _fvector throwDir)
{
	/*10 이하면 10으로 맞춰줄 것.*/
	_float4 pos = {};
	XMStoreFloat4(&pos, StartPos);
	if (pos.y < 10.f)
		pos.y = 10.f;
	
	m_pTransform->Set_Pos(pos);
	/*여기서 더해주니까 방향 벡터는 보정 없음*/
	XMStoreFloat4(&m_DstPosition, StartPos + XMVector4Normalize(throwDir) * 8);
	m_eState = THROW;
}

void CItem_Object::Attach_Hand(_float4x4* pOwnerMatrix)
{
	m_eState = PICKED;
	m_pOwnerMatrix = pOwnerMatrix;
}

void CItem_Object::Update_ByState(_float dt)
{
	switch (m_eState) {
	case THROW:
		Throw_Item(dt);
			  break;
	case DROP: {
		//m_pTransform->Translate({ 0,-dt * 45,0 });
		if (Get_Position().y <= m_MarginY) {
			Find_Ground();
		}
	}
			 break;
	case  BOUND:
		MoveToIndex(dt);
		break;
	case  PICKED:
		FollowHand(dt);
		break;
	case  READY_DESTROY: {
		Remove_Item();
		CGameInstance::GetInstance()->Get_ObjectMgr()->Remove_Object(this);
		m_eState = IDLE;
	}
					   break;
	}
}

void CItem_Object::Check_Palette()
{
	
}

void CItem_Object::Render_GUI()
{
	__super::Render_GUI();
	ImGui::Text("nowIndex X : %d, Z : %d", m_SyncedIndex.IndexX, m_SyncedIndex.IndexZ);
}


void CItem_Object::Dangle_Item(const string& boneName, _float3 offset)
{
	m_Offset = offset;
	IsDangled = true;
	CGameObject* pObj = Get_Component<CChild>()->Get_Parent();
	Get_Component<CBoneFollower>()->Link_Bone(pObj->Get_Component<CAnimator3D>(), boneName);
	Get_Component<CBoneFollower>()->Set_Offset(XMMatrixTranslation(offset.x, offset.y, offset.z));
}

void CItem_Object::Throw_Item(_float dt)
{
	m_fBoundingTime += dt;
	_vector DstPos = XMLoadFloat4(&m_DstPosition);
	_vector CurPos = m_pTransform->Get_Pos();

	_vector MovedPos = XMVectorLerp(CurPos, DstPos, dt * 15);

	_float4 pos = {};
	XMStoreFloat4(&pos, MovedPos);

	/*포물선 느낌으로*/
	_float4 movePos;
	XMStoreFloat4(&movePos, MovedPos);
	movePos.y += 3 * sinf(XMConvertToRadians(45)) - m_fBoundingTime * 9.8f;

	if (movePos.y <= m_MarginY) {
		movePos.y = m_MarginY;
	}

	m_pTransform->Set_Pos(_float3(movePos.x, movePos.y, movePos.z));

	if (Get_Position().y <= m_MarginY) {
		m_fBoundingTime = 0;
		m_eState = DROP;
		m_pTransform->Set_Y(m_MarginY);
		auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
		m_SyncedIndex = TileSystem->Get_IndexByPosition(Get_Position());
	}
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
			TILE_INDEX NowIndex = TileSystem->Get_IndexByPosition(Get_Position());
			m_DstPosition = TileSystem->Get_PositionByIndex(NowIndex, ANCHOR::Center);
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
	_vector vMove = XMVectorLerp(vNow, vDst, dt * 15);

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

	_vector vNowPos = m_pTransform->Get_Pos();
	_vector vTargetPos = trans;

	_vector vLerpPos = XMVectorLerp(vNowPos, vTargetPos, dt * 10.f);
	m_pTransform->Set_vectorPos(vLerpPos);//vLerpPos
}


void CItem_Object::Remove_Item()
{
	m_eState = READY_DESTROY;
	auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	TileSystem->Remove_TileFlagByIndex(m_SyncedIndex, static_cast<_uint>(TILE_FLAG::FLAG_ONITEM));
}

CItem_Object* CItem_Object::Create()
{
	CItem_Object* instance = new CItem_Object();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CItem_Object");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CItem_Object::Clone(INIT_DESC* pArg)
{
	CItem_Object* instance = new CItem_Object(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CItem_Object");
		Safe_Release(instance);
	}

	return instance;
}
void CItem_Object::Free()
{
	__super::Free();
}
