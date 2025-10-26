#include "Editor_Defines.h"
#include "ControlPanel.h"
#include "GridObject.h"

#include "EditorSystem.h"
#include "TileObject.h"
#include "GameInstance.h" 
#include "ITileService.h"
#include "MapObject.h"
vector<string> tileType = {"Base_0","RoadDarkSoil","RoadSoil","RoadSand","RoadBrick","RoadFanPattern","RoadStone","RoadTile", "RoadWood","River"};

CControlPanel::CControlPanel(GUI_CONTEXT* context)
	:CBasePanel{ context }
{
	m_eMode = NONE;
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

	ImGui::BeginChild("##Mode Btn", ImVec2{ 200, textLineHeight*4 }, true);
	if (ImGui::Button("EditMode")) {
		m_eMode = EditObj;
	}
	ImGui::SameLine();
	if (ImGui::Button("BrushMode")) {
		m_eMode = BrushTile;
	}
	if (ImGui::Button("TileSystemMode")) {
		m_eMode = TileSystem;
	}
	ImGui::EndChild();
	ImGui::BeginChild("##Save Btn", ImVec2{ 200, textLineHeight * 2 }, true);
	if (ImGui::Button("SaveMapDatas")) {
		SaveTab = true;
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
	CEditorSystem::GetInstance()->Get_Context()->eMode = m_eMode;

	switch (m_eMode)
	{
	case NONE:
		break;
	case EditObj:
		Render_EditTab();
		break;
	case BrushTile:
		Render_BrushTab();
		break;
	case TileSystem:
		Render_TileEdit();
		break;
	default:
		break;
	}

	if (SaveTab)
		Render_SaveTab();
}

void CControlPanel::Render_BrushTab()
{
	CEditorSystem::Editor_Context* pContext = CEditorSystem::GetInstance()->Get_Context();
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	ImVec2 windowPos = ImVec2((float)550, 0);

	ImGui::Begin("Brush Control");
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

	ImGui::End();
}

void CControlPanel::Render_EditTab()
{
	CEditorSystem::Editor_Context* pContext = CEditorSystem::GetInstance()->Get_Context();
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	ImVec2 windowPos = ImVec2((float)550, 0);

	ImGui::Begin("Anchor Control");
	float childWidth = ImGui::GetContentRegionAvail().x;//->이건 넓이 설정
	ImGui::SetNextItemWidth(childWidth);
	ImGui::CheckboxFlags("Center", &pContext->eAnchor, static_cast<_uint>(ANCHOR::Center));
	ImGui::CheckboxFlags("Left", &pContext->eAnchor, static_cast<_uint>(ANCHOR::Left));
	ImGui::CheckboxFlags("Right", &pContext->eAnchor, static_cast<_uint>(ANCHOR::Right));
	ImGui::CheckboxFlags("Bottom", &pContext->eAnchor, static_cast<_uint>(ANCHOR::Bottom));
	ImGui::CheckboxFlags("Top", &pContext->eAnchor, static_cast<_uint>(ANCHOR::Top));

	ImGui::End();
}

void CControlPanel::Render_TileEdit()
{
	CEditorSystem::Editor_Context* pContext = CEditorSystem::GetInstance()->Get_Context(); 
	ImGui::SetNextWindowSize(ImVec2(300, 600));
	ImVec2 windowPos = ImVec2((float)550, 0);
	ImGui::Begin("TileSystem Control");
	if (ImGui::Button("Clear_Index")) {
		CEditorSystem::GetInstance()->Clear_Index();
	}
	ImGui::SeparatorText("Corner Height");
	ImGui::Text("LeftTopHeight");
	ImGui::InputFloat("##LeftTopHeight",&pContext->m_fConerHeight.x, 1, 5,"%.2f");
	ImGui::Text("RightTopHeight");
	ImGui::InputFloat("##RightTopHeight",&pContext->m_fConerHeight.y, 1, 5,"%.2f");
	ImGui::Text("RightBottomHeight");
	ImGui::InputFloat("##RightBottomHeight",&pContext->m_fConerHeight.z, 1, 5,"%.2f");
	ImGui::Text("LeftBottomHeight");
	ImGui::InputFloat("##LeftBottomHeight",&pContext->m_fConerHeight.w,1,5,"%.2f");
	if (ImGui::Button("Adjust Height")) {
		CEditorSystem::GetInstance()->Adjust_Height();
	}
	ImGui::SeparatorText("Flag");



	ImGui::CheckboxFlags("Walkable", &pContext->tileFlag, static_cast<_uint>(TILE_FLAG::FLAG_WALKABLE));
	ImGui::CheckboxFlags("Blocked", &pContext->tileFlag, static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED));
	ImGui::CheckboxFlags("Swimmable", &pContext->tileFlag, static_cast<_uint>(TILE_FLAG::FLAG_SWIMMABLE));
	if (ImGui::Button("Adjust Flag")) {
		CEditorSystem::GetInstance()->Adjust_Flag();
	}
	ImGui::End();
}

void CControlPanel::Render_SaveTab()
{
	CEditorSystem::Editor_Context* pContext = CEditorSystem::GetInstance()->Get_Context();
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	ImVec2 windowPos = ImVec2((float)550, 0);

	ImGui::Begin("Save Panel", &SaveTab);
	float childWidth = ImGui::GetContentRegionAvail().x;//->이건 넓이 설정
	ImGui::SetNextItemWidth(childWidth);
	if (ImGui::Button("Save Rule File")) {
		CTileObject::Save_RuleFile();
	}
	if (ImGui::Button("Save Map Objects")) {
		CEditorSystem::GetInstance()->Save_MapData();
	}
	if (ImGui::Button("Save TileSystem")) {
		CGameInstance::GetInstance()->Get_TileSystem()->Save_TileSystemData("../../Resources/Data/TileSystemData.dat");
	}
	if (ImGui::Button("Save ModelMap")) {
		CMapObject::Save_ModelMap();
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
