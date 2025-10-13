#include "Editor_Defines.h"
#include "DirectoryPanel.h"
#include "GridObject.h"
#include "GameInstance.h"
#include "ResourceMgr.h"
#include "Texture.h"
#include "EditorSystem.h"

CDirectoryPanel::CDirectoryPanel(GUI_CONTEXT* context)
	:CBasePanel{ context }
{
}

CDirectoryPanel::~CDirectoryPanel()
{
}

HRESULT CDirectoryPanel::Initialize()
{
	m_pResource = CGameInstance::GetInstance()->Get_ResourceMgr();
	Safe_AddRef(m_pResource);
	m_pResource->Add_ResourcePath("Icon_Folder", "../../Resources/Folder_Icon.png");
	m_pResource->Add_ResourcePath("Icon_Model", "../../Resources/Model_Icon.png");

	m_pFolderIcon = m_pResource->Load_Texture(G_GlobalLevelKey, "Icon_Folder");
	m_pModelIcon = m_pResource->Load_Texture(G_GlobalLevelKey, "Icon_Model");

	m_strDirectory= "../../Resources/Models";

	LookUp_Directory(m_strDirectory);
	return S_OK;
}

void CDirectoryPanel::Render_GUI()
{
	ImVec2 windowPos = ImVec2((float)200, g_iWinSizeY - 300);
	ImVec2 windowSize = ImVec2((float)g_iWinSizeX - 450, 300);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
	ImGui::Begin("Model Files", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	ImGui::Separator();

	if (ImGui::RadioButton("Tile", m_eMode == TILE)) {
		m_eMode = TILE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("FieldOut", m_eMode == FIELDOUT)) {
		m_eMode = FIELDOUT;
	}	
	ImGui::SameLine();
	if (ImGui::RadioButton("Structure", m_eMode == STRUCTURE)) {
		m_eMode = STRUCTURE;
	}

	ImGui::Separator();

	Render_Folders();

	ImGui::End();
}

void CDirectoryPanel::Render_Folders()
{
	ImVec2 windowSize = ImVec2((float)g_iWinSizeX - 450, 300);
	float itemWidth = m_fIconSize + 40.f;       // 아이콘 크기 + 여백
	int iRowSize = max(1, (int)(windowSize.x / itemWidth)); // 한 줄에 몇 개
	ImVec2 itemSize = ImVec2(m_fIconSize + 30.f, m_fIconSize + 35.f);

	ImGui::BeginChild(("Group_Back"), itemSize, false,
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse);
	{
		ImGui::ImageButton("BackBtn", (ImTextureID)m_pFolderIcon->Get_SRV(), ImVec2(m_fIconSize, m_fIconSize));
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			GoTo_Parent();
		ImGui::Text(".../Back");
	}
	ImGui::EndChild();

	ImGui::Separator();
	for (size_t i = 0; i < m_ContentInfos.size(); ++i)
	{
		const string& name = m_ContentInfos[i].strName;
		FolderType type = m_ContentInfos[i].eType;
		string truncated = TruncateText(name, 12);

		ImGui::BeginChild(("Group_" + name).c_str(), itemSize, true,
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse);
		{
			ImGui::ImageButton(name.c_str(),
				(ImTextureID)(type == PARENT ?m_pFolderIcon->Get_SRV() : m_pModelIcon->Get_SRV() ),
				ImVec2(m_fIconSize, m_fIconSize) 
			);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
				if (type == PARENT) {
					m_strDirectory += ("/" + name);
					LookUp_Directory(m_strDirectory);
				}
				else {
					Create_Tile(name);
				}
			}

			ImGui::Text("%s", truncated.c_str());

			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", name.c_str());
		}
		ImGui::EndChild();

		if ((i + 1) % iRowSize != 0)
			ImGui::SameLine(0.f, 10.f);
	}
	ImGui::Dummy(ImVec2(0.0f, m_fIconSize));
}

void CDirectoryPanel::Create_Tile(const string& name)
{
	if (m_bItemCreated) return;

	string folderPath = m_strDirectory + ("/" + name);
	m_pResource->Add_ResourcePath((name+".model"),(folderPath + "/" + name + ".model"));
	m_pResource->Add_ResourcePath((name + ".mat"), (folderPath +"/"+ name + ".mat"));
	HRESULT hr;

	switch (m_eMode)
	{
	case MapEditor::CDirectoryPanel::TILE:
		hr = CEditorSystem::GetInstance()->Create_MapObject(name,CEditorSystem::TILE); break;
	case MapEditor::CDirectoryPanel::FIELDOUT:
		hr = CEditorSystem::GetInstance()->Create_MapObject(name, CEditorSystem::FIELDOUT); break;
	case MapEditor::CDirectoryPanel::STRUCTURE:
		hr = CEditorSystem::GetInstance()->Create_MapObject(name, CEditorSystem::STRUCTURE); break;
	default:
		break;
	}

	if (SUCCEEDED(hr)) {
		m_bItemCreated = true;
	}
}

void CDirectoryPanel::LookUp_Directory(const string& path)
{
	m_ContentInfos.clear();

	for (const auto& entry : filesystem::directory_iterator(path))
	{
		if (!entry.is_directory())
			continue;

		FolderInfo info = {};
		info.strName = entry.path().filename().string();
		info.eType = LEAF;

		// 하위 폴더 참색
		for (const auto& sub : filesystem::directory_iterator(entry.path()))
		{
			if (filesystem::is_directory(sub.path()))
			{
				info.eType = PARENT;
				break;
			}
		}

		m_ContentInfos.push_back(info);
	}
}

void CDirectoryPanel::GoTo_Parent()
{
	m_strDirectory=filesystem::path(m_strDirectory).parent_path().string();
	LookUp_Directory(m_strDirectory);
}

string CDirectoryPanel::TruncateText(const string& text, _uint Length)
{
	if (text.size() <= Length) return text;
	return text.substr(0, Length - 3) + "...";
}


CDirectoryPanel* CDirectoryPanel::Create(GUI_CONTEXT* context)
{
	CDirectoryPanel* instance = new CDirectoryPanel(context);
	if (FAILED(instance->Initialize()))
		Safe_Release(instance);

	return instance;
}

void CDirectoryPanel::Free()
{
	__super::Free();
	Safe_Release(m_pResource);
}
