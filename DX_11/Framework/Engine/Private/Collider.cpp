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
	m_SystemIndex = CGameInstance::GetInstance()->Get_CollisionSystem()->RegisterCollider(this, m_SystemIndex);
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

void CCollider::Free()
{
	__super::Free();
	 CGameInstance::GetInstance()->Get_CollisionSystem()->UnregisterCollider(this,m_SystemIndex);
}

void CCollider::Set_ColliderActive(_bool Active)
{
	if (m_SystemIndex < 0)
		return;
	else if(Active == false){
		CGameInstance::GetInstance()->Get_CollisionSystem()->DeActiveCollider(this, m_SystemIndex);
	}
	else if (Active == true) {
		CGameInstance::GetInstance()->Get_CollisionSystem()->ActiveCollider(this, m_SystemIndex);
	}
}
