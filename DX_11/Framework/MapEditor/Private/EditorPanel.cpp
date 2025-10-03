#include "Editor_Defines.h"
#include "EditorPanel.h"
#include "GridObject.h"
#include "GameInstance.h"
#include "ResourceMgr.h"
#include "Texture.h"
#include "EditorSystem.h"

CEditorPanel::CEditorPanel(GUI_CONTEXT* context)
	:CBasePanel{ context }
{
}

CEditorPanel::~CEditorPanel()
{
}

HRESULT CEditorPanel::Initialize()
{
	m_pResource = CGameInstance::GetInstance()->Get_ResourceMgr();
	Safe_AddRef(m_pResource);
	m_pResource->Add_ResourcePath("Icon_Folder", "../../Resources/Folder_Icon.png");
	m_pFolderIcon = m_pResource->Load_Texture(G_GlobalLevelKey, "Icon_Folder");

	string resourcPath = "../../Resources/Models";
	for (const auto& entry : filesystem::directory_iterator(resourcPath)) {
		if (entry.is_directory()) {
			m_FolderNames.push_back(entry.path().stem().string());
		}
	}
	return S_OK;
}

void CEditorPanel::Render_GUI()
{
	if (m_pGrid)
		m_pGrid->Render_GUI();

	ImVec2 windowPos = ImVec2((float)200, g_iWinSizeY - 300);
	ImVec2 windowSize = ImVec2((float)g_iWinSizeX - 450, 300);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
	ImGui::Begin("Model Files", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	
	int iRowSize = (int)(windowSize.x / (m_fIconSize * 0.5 + m_fIconSize)); ///몇개 한줄에
	for (size_t i = 0; i < m_FolderNames.size(); i++)
	{
		ImGui::BeginGroup();
		if (ImGui::ImageButton(
			m_FolderNames[i].c_str(),
			(ImTextureID)m_pFolderIcon->Get_SRV(),
			ImVec2(m_fIconSize, m_fIconSize)
		))
		{
			
		}
		if (ImGui::IsItemActive() && ImGui::IsItemHovered()&&!m_bItemCreated) {
			payload = m_FolderNames[i];
			if (SUCCEEDED(CEditorSystem::GetInstance()->Create_Object(payload))) {
				m_bItemCreated = true;
			};
		}
		ImGui::TextWrapped("%s", m_FolderNames[i].c_str());
		ImGui::EndGroup();

		if ((i + 1) % iRowSize != 0) {
			ImGui::SameLine();
		}
	}
	ImGui::Dummy(ImVec2(0.0f, m_fIconSize));
	ImGui::End();
}

void CEditorPanel::Set_Grid(CGridObject* pGrid)
{
	m_pGrid = pGrid;
	Safe_AddRef(pGrid);
}

CEditorPanel* CEditorPanel::Create(GUI_CONTEXT* context)
{
	CEditorPanel* instance = new CEditorPanel(context);
	if (FAILED(instance->Initialize()))
		Safe_Release(instance);

	return instance;
}

void CEditorPanel::Free()
{
	__super::Free();
	Safe_Release(m_pGrid);
	Safe_Release(m_pResource);
}
