#include "Collider.h"
#include "GameInstance.h"
#include "ICollisionService.h"
#include "GameObject.h"
CCollider::CCollider()
{
}

CCollider::CCollider(const CCollider& rhs)
	:CComponent(rhs)
{
}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize(COMPONENT_DESC* pArg)
{
	m_CollisionContext.Owner = m_pOwner;
	return S_OK;
}

void CCollider::Render_GUI()
{

	ImGui::SeparatorText("Collider");
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight * 5) + (ImGui::GetStyle().WindowPadding.y * 2);

	ImGui::BeginChild("##ColliderChild", ImVec2{ 0, childHeight }, true);
	ImGui::Text("System Index : %d", m_SystemIndex);
	string ownerTag = "Owner Tag : " + m_CollisionContext.Owner->Get_Tag();
	ImGui::Text(ownerTag.c_str());
	ImGui::Text(m_CollisionContext.EventTag.c_str());
	ImGui::EndChild();

}

void CCollider::Set_Dead(_bool bActive)
{
	m_bActive = bActive; 
	if(bActive == false)
		m_CollisionContext.Owner = nullptr;
}

_bool CCollider::Compare_Same(COLLIDER_SLOT* prev, COLLIDER_SLOT* current)
{
	if (prev->bActive == false) {
		return false;
	}
	if (current->bActive == false) {
		return false;
	}
	if (prev->pCollider->Get_Active() == false) {
		return false;
	}
	if (current->pCollider->Get_Active() == false) {
		return false;
	}

	if (prev->iGeneration != current->iGeneration)
		return false;
	return true;
}

void CCollider::Free()
{
	__super::Free();

	m_CurrentCollider.clear();
	m_prevCollider.clear();

	//CGameInstance::GetInstance()->Get_CollisionSystem()->UnregisterCollider(this,m_SystemIndex);
}

void CCollider::Set_ColliderActive(_bool Active)
{
	if (m_SystemIndex < 0)
		return;

	else if(Active == false){
		CGameInstance::GetInstance()->Get_CollisionSystem()->DeActiveCollider(this, m_SystemIndex);
	}
	else if (Active == true && Has_Desc()) {
		CGameInstance::GetInstance()->Get_CollisionSystem()->ActiveCollider(this, m_SystemIndex);
	}
}
