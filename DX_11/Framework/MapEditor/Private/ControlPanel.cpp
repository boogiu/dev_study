#include "ControlPanel.h"
#include "Editor_Defines.h"
#include "GridObject.h"

#include "EditorSystem.h"


CControlPanel::CControlPanel(GUI_CONTEXT* context)
	:CBasePanel{ context }
{
}

CControlPanel::~CControlPanel()
{
}

HRESULT CControlPanel::Initialize()
{
	return S_OK;
}

void CControlPanel::Set_Grid(CGridObject* pGrid)
{
	m_pGrid = pGrid;
	Safe_AddRef(pGrid);
}

void CControlPanel::Render_GUI()
{
	if (m_pGrid)
		m_pGrid->Render_GUI();

	ImVec2 windowPos = ImVec2((float)650, 0);
	ImGui::SetNextWindowPos(ImVec2(windowPos), ImGuiCond_Once);
	ImGui::Begin("EditMode", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

	if (ImGui::Button("BasePlane")) {
		CEditorSystem::GetInstance()->Create_Base();
	}

	/*어떤 오브젝트 생성 중인지 확인*/
	ImGui::Button("Terrain Blocks"); 
	ImGui::SameLine();
	ImGui::Button("Structure Blocks");
	
	if (ImGui::Button("SaveMapDatas")) {
		CEditorSystem::GetInstance()->Save_MapData();
	}

	ImGui::End();

	if (m_pContext->pSelectedObject) {
		ImGui::Begin("Object Inspector");
		if (ImGui::Button("Delete")) {
			CEditorSystem::GetInstance()->Delete_Object(m_pContext->pSelectedObject);
			m_pContext->pSelectedObject = nullptr;
		}
		ImGui::End();
	}
}

CControlPanel* CControlPanel::Create(GUI_CONTEXT* context)
{
	CControlPanel* instance = new CControlPanel(context);
	if (FAILED(instance->Initialize()))
		Safe_Release(instance);

	return instance;
}
void CControlPanel::Free()
{
	Safe_Release(m_pGrid);
}
