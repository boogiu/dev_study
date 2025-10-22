#include "ControlPanel.h"
#include "Editor_Defines.h"
#include "GridObject.h"

#include "EditorSystem.h"
#include "TileObject.h"

vector<string> tileType = {"Base_0","RoadDarkSoil","RoadSoil","RoadSand","RoadBrick","RoadFanPattern","RoadStone","RoadTile", "RoadWood"};

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

	ImVec2 windowPos = ImVec2((float)250, 0);
	ImGui::SetNextWindowPos(ImVec2(windowPos), ImGuiCond_Once);
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	ImGui::Begin("Editor Control",0,ImGuiWindowFlags_NoDecoration);

	ImGui::BeginChild("##Mode Btn", ImVec2{ 200, textLineHeight*2 }, true);
	if (ImGui::Button("EditMode")) {
		m_BrushTabOpen = false;
		CEditorSystem::GetInstance()->Get_Context()->eMode = CEditorSystem::EditObj;
	}
	ImGui::SameLine();
	if (ImGui::Button("BrushMode")) {
		m_BrushTabOpen = true;
		CEditorSystem::GetInstance()->Get_Context()->eMode = CEditorSystem::BrushTile;
	}
	ImGui::EndChild();

	ImGui::BeginChild("##Save Btn", ImVec2{ 200, textLineHeight * 2 }, true);

	if (ImGui::Button("SaveMapDatas")) {
		CEditorSystem::GetInstance()->Save_MapData();
	}
	ImGui::EndChild();
	ImGui::End();

	if (m_pContext->pSelectedObject) {
		ImGui::Begin("Object Inspector");
		if (ImGui::Button("Delete")) {
			CEditorSystem::GetInstance()->Delete_Object(m_pContext->pSelectedObject);
			m_pContext->pSelectedObject = nullptr;
		}
		ImGui::End();
	}

	if (m_BrushTabOpen) 
		Render_BrushTab();
}

void CControlPanel::Render_BrushTab()
{

	CEditorSystem::Editor_Context* pContext = CEditorSystem::GetInstance()->Get_Context();
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	ImVec2 windowPos = ImVec2((float)550, 0);

	ImGui::Begin("Brush Control", &m_BrushTabOpen);
	float childWidth = ImGui::GetContentRegionAvail().x;//->이건 넓이 설정
	string front = pContext->baseType.empty() ? tileType[0] : pContext->baseType;
	ImGui::SetNextItemWidth(childWidth);
	if (ImGui::BeginCombo("##TileTypeCombo", front.c_str()))
	{
		for (int i = 0; i < tileType.size(); ++i) {
			bool isSelected = (tileType[i] == pContext->baseType);

			if (ImGui::Selectable(tileType[i].c_str(), isSelected)) {
				pContext->baseType = tileType[i];
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Save Rule File")) {
		CTileObject::Save_RuleFile();
	}
	ImGui::End();
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
