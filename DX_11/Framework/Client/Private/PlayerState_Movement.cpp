#include "Client_Defines.h"
#include "PlayerState_Movement.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Movement::CPlayerState_Movement()
{
}

void CPlayerState_Movement::OnEnter()
{
	m_bEnter = true;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

}

void CPlayerState_Movement::OnUpdate(_float dt)
{
	m_bEnter = false;

	/*Run Animation*/
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Update_Animation(dt);

	/*Check Radian*/
	Check_Radian(dt);

	/*Check y Height*/
	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	_float4 myPos = m_pPlayer->Get_Position();
	m_fPlayerHeight =  tileSystem->Get_TileHeightByPosition(myPos);
	m_pPlayer->Get_Component<CTransform>()->Set_Y(m_fPlayerHeight);

	
}

void CPlayerState_Movement::OnExit()
{
}

CState* CPlayerState_Movement::HandleTransition()
{
	auto inpuDev = CGameInstance::GetInstance()->Get_InputDev();
	if (m_bEnter)
		return nullptr;

	return nullptr;
}

_bool CPlayerState_Movement::CheckMovable(_float2& InputAxis)
{
	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	_float2 tmpAxis = InputAxis;

	_float4  NextPos = m_pPlayer->Get_Position();
	NextPos.x += tmpAxis.x;
	NextPos.z += tmpAxis.y;

	TILE_INDEX nextIndex = CGameInstance::GetInstance()->Get_TileSystem()->Get_IndexByPosition(NextPos);

	_uint Flag = tileSystem->Get_TileFlagByIndex(nextIndex);

	if ((Flag & static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED)) == 0)
		return true; /*막혀 있지 않음*/

	/*움직일 수 없음*/
	_bool blockX = false;
	_bool blockZ = false;

	/*X축 검사*/
	_float4 testX = m_pPlayer->Get_Position();
	testX.x += tmpAxis.x;

	TILE_INDEX testIdxX = tileSystem->Get_IndexByPosition(testX);

	if (tileSystem->Get_TileFlagByIndex(testIdxX) & static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED))
		blockX = true;

	/*Z축 검사*/
	_float4 testZ = m_pPlayer->Get_Position();
	testZ.z += tmpAxis.y;
	TILE_INDEX testIdxZ = tileSystem->Get_IndexByPosition(testZ);

	if (tileSystem->Get_TileFlagByIndex(testIdxZ) & static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED))
		blockZ = true;

	if (blockX)
	{ 
		tmpAxis.x = 0.f;
		tmpAxis.y = 0.1f *(InputAxis.x <0? -1 : 1);
	}
	if (blockZ) { 
		tmpAxis.x = 0.1f * (InputAxis.y < 0 ? -1 : 1);
		tmpAxis.y = 0.0f;
	}

	InputAxis = tmpAxis;

	if (tmpAxis.x != 0.f || tmpAxis.y != 0.f)
		return true;

	return false;
}

void CPlayerState_Movement::Render_State()
{
	_float2 InputAxis = m_pPlayer->Get_InputAxis();
	ImGui::Text("Axis : X : %.1f, Z : %.1f", InputAxis.x, InputAxis.y);
	ImGui::Text("Target Degree : %.2f", m_fTargetDegree);
	ImGui::Text("Current Degree : %.2f", m_fTurnDegree);
	ImGui::Checkbox("Flipping", &m_bFliping);
	ImGui::Text("Now Height : Y : %.1f", m_fPlayerHeight);
}

void CPlayerState_Movement::Check_Radian(_float dt)
{
	_float2 InputAxis = m_pPlayer->Get_InputAxis();
	if (fabs(InputAxis.x) > 0.01f || fabs(InputAxis.y) > 0.01f)
		m_fTargetDegree = XMConvertToDegrees(atan2(InputAxis.x, InputAxis.y));

	_float DeltaDegree = m_fTargetDegree - m_fTurnDegree;

	// -180~180 범위로 정규화
	while (DeltaDegree > 180.f) DeltaDegree -= 360.f;
	while (DeltaDegree < -180.f) DeltaDegree += 360.f;

	_float RotSpeed = dt * 480;

	if (fabs(DeltaDegree) > 150.f)
		m_bFliping = true;

	if (fabs(DeltaDegree) > 5.f) {
		if (m_bFliping)
			m_fTurnDegree -= RotSpeed;
		else
			m_fTurnDegree += (DeltaDegree > 0 ? RotSpeed : -RotSpeed);
	}

	else {
		m_bFliping = false;
		m_fTurnDegree = m_fTargetDegree;
	}
}

CPlayerState_Movement* CPlayerState_Movement::Create()
{
	return new CPlayerState_Movement;
}

void CPlayerState_Movement::Free()
{
	__super::Free();
}
