#include "Model.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "IRenderService.h"
#include "ModelData.h"

CModel::CModel()
{
}

CModel::CModel(const CModel& rhs)
	:	m_pData(rhs.m_pData),
		m_DrawableMeshes(rhs.m_DrawableMeshes)
{
		Safe_AddRef(m_pData);
}

const D3D11_INPUT_ELEMENT_DESC* CModel::Get_ElementDesc(_uint DrawIndex)
{
	return m_pData->Get_ElementDesc(DrawIndex);
}

const _uint CModel::Get_ElementCount(_uint DrawIndex)
{
	return m_pData->Get_ElementCount(DrawIndex);
}

const string_view CModel::Get_ElementKey(_uint DrawIndex)
{
	return m_pData->Get_ElementKey(DrawIndex);
}

_uint CModel::Get_MeshCount()
{
	return m_pData->Get_MeshCount();
}

_uint CModel::Get_MaterialIndex(_uint Index)
{
	return m_pData->Get_MaterialIndex(Index);
}

_bool CModel::isDrawable(_uint Index)
{
	if (Index >= m_DrawableMeshes.size()) return false;

	return m_DrawableMeshes[Index];
}

void CModel::Render_GUI()
{
	ImGui::SeparatorText("Model");
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight * 2) + (ImGui::GetStyle().WindowPadding.y * 4);
	ImGui::BeginChild("##ModelChild", ImVec2{ childWidth, childHeight }, true);

	ImGui::EndChild();
}

void CModel::Free()
{
	__super::Free();
	Safe_Release(m_pData);
}
