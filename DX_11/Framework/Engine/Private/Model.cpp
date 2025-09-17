#include "Model.h"
#include "GameInstance.h"
#include "VIBuffer.h"
#include "IResourceService.h"
#include "IRenderService.h"
#include "Mesh.h"

CModel::CModel()
{
}

CModel::CModel(const CModel& rhs)
	: m_Buffers(rhs.m_Buffers),
	m_DrawableMeshes(rhs.m_DrawableMeshes)
{
	for (auto& mesh : m_Buffers)
		Safe_AddRef(mesh);
}

HRESULT CModel::Link_Buffer(const string& levelKey, const string& MeshKey)
{
	return S_OK;
}


_uint CModel::Get_MaterialIndex(_uint Index)
{
	if (Index >= m_Buffers.size()) return 0;
	
	return m_Buffers[Index]->Get_MaterialIndex();
}

_bool CModel::isDrawable(_uint Index)
{
	if (Index >= m_DrawableMeshes.size()) return false;

	return m_DrawableMeshes[Index];
}

const string& CModel::Get_BufferKey(_uint Index)
{
	if (m_Buffers.size() <= Index) return string();
	return m_Buffers[Index]->Get_Key();
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
	for (auto& mesh : m_Buffers)
		Safe_Release(mesh);
}
