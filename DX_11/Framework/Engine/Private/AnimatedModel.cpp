#include "AnimatedModel.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "IRenderService.h"
#include "ModelData.h"

CAnimatedModel::CAnimatedModel()
{
}

CAnimatedModel::CAnimatedModel(const CAnimatedModel& rhs)
	: CModel(rhs)
{
}

HRESULT CAnimatedModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimatedModel::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}

HRESULT CAnimatedModel::Link_Model(const string& levelKey, const string& MeshKey)
{
	m_pData = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_ModelData(levelKey, MeshKey);
	m_DrawableMeshes.resize(m_pData->Get_MeshCount(), true);

	return S_OK;
}

HRESULT CAnimatedModel::Render_Model(ID3D11DeviceContext* pContext, _uint Index)
{
	/*모델은 렌더의 역할만 수행할 것임*/
	return m_pData->Render_Mesh(pContext, Index);
}

void CAnimatedModel::Render_GUI()
{
	ImGui::SeparatorText("Animate Model");
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight * 2) + (ImGui::GetStyle().WindowPadding.y * 2);

	ImGui::BeginChild("##Animate ModelChild", ImVec2{ 0, childHeight }, true);
		m_pData->Render_GUI();
	ImGui::EndChild();
}


CAnimatedModel* CAnimatedModel::Create()
{
	CAnimatedModel* instance = new CAnimatedModel();

	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("CStaticModel Create Failed : CStaticModel");
		Safe_Release(instance);
	}

	return instance;
}

CComponent* CAnimatedModel::Clone()
{
	CAnimatedModel* instance = new CAnimatedModel(*this);
	return instance;
}

void CAnimatedModel::Free()
{
	__super::Free();
}
