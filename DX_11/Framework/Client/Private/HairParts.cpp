#include "Client_Defines.h"
#include "HairParts.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "BoneFollower.h"
#include "Animator3D.h"

#include "Player.h"
CHairParts::CHairParts()
{
}

CHairParts::CHairParts(const CHairParts& rhs)
	:CGameObject(rhs)
{
}

CHairParts::~CHairParts()
{
}

HRESULT CHairParts::Initialize_Prototype()
{
	HRESULT hr = Add_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", "PlayerHair13.model");
	hr = Add_Component<CMaterial>()->Link_Material("GamePlay_Level", "PlayerHair13.mat");
	Add_Component<CBoneFollower>();

	return hr;
}

HRESULT CHairParts::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	CPlayer::PLAYER_PARTS_DESC* pDesc = static_cast<CPlayer::PLAYER_PARTS_DESC*>(pArg);
	Get_Component<CBoneFollower>()->Link_Bone(
		pDesc->pPlayer->Get_Component<CAnimator3D>(),
		"Armature_Head"
	);
	_matrix matPos = XMMatrixTranslation(0, 9, -0.5f);
	_matrix matRot = XMMatrixRotationY(XMConvertToRadians(180));

	Get_Component<CBoneFollower>()->Set_Offset(
		matPos*matRot
	);
	return S_OK;
}

void CHairParts::Priority_Update(_float dt)
{
}

void CHairParts::Update(_float dt)
{
	Get_Component<CBoneFollower>()->Sync_Transform(dt, m_pTransform);
}

void CHairParts::Late_Update(_float dt)
{
}

void CHairParts::Render_GUI()
{
	__super::Render_GUI();

	//static _float3 vOffsetPos = { 0.f, 10.f, 0.f };      // 이동값
	//static _float3 vOffsetRot = { 0.f, 180.f, 0.f };     // 회전값 (도 단위)
	//
	//ImGui::SeparatorText("HairCap Offset Control");
	//
	//ImGui::Text("Position Offset");
	//ImGui::DragFloat3("##OffsetPos", reinterpret_cast<float*>(&vOffsetPos), 0.1f, -50.f, 50.f, "%.2f");
	//
	//ImGui::Text("Rotation Offset (deg)");
	//ImGui::DragFloat3("##OffsetRot", reinterpret_cast<float*>(&vOffsetRot), 0.5f, -180.f, 180.f, "%.1f");
	//
	//// 변환 행렬 계산
	//_matrix matPos = XMMatrixTranslation(vOffsetPos.x, vOffsetPos.y, vOffsetPos.z);
	//
	//_matrix matRot =
	//	XMMatrixRotationX(XMConvertToRadians(vOffsetRot.x)) *
	//	XMMatrixRotationY(XMConvertToRadians(vOffsetRot.y)) *
	//	XMMatrixRotationZ(XMConvertToRadians(vOffsetRot.z));
	//
	//_matrix matOffset = matRot * matPos; // 회전 → 이동 순서
	//
	//Get_Component<CBoneFollower>()->Set_Offset(matOffset);
}

CHairParts* CHairParts::Create()
{
	CHairParts* instance = new CHairParts();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CHairParts");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CHairParts::Clone(INIT_DESC * pArg)
{
	CHairParts* instance = new CHairParts(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CHairParts");
		Safe_Release(instance);
	}

	return instance;
}

void CHairParts::Free()
{
	__super::Free();
}
