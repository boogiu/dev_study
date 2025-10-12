#include "Loader_Defines.h"
#include "OptionUI.h"
#include "Helper_Func.h"

#include "LoadMaterial.h"
#include "AIMaterial.h"
#include "LoadStaticModel.h"
#include	"LoadSkeletalModel.h"

COptionUI::COptionUI()
{
}

COptionUI::COptionUI(const COptionUI& rhs)
	:CGameObject(rhs)
{
}

HRESULT COptionUI::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT COptionUI::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	m_pSkeletalModelComp = CLoadSkeletalModel::Create();
	m_pStaticModelComp = CLoadStaticModel::Create();
	m_pMaterial = CLoadMaterial::Create();
	return S_OK;
}

void COptionUI::Priority_Update(_float dt)
{
}

void COptionUI::Update(_float dt)
{
	if (isReadyToSave)
	{
		LookUp_Folder();
	}
}

void COptionUI::Late_Update(_float dt)
{
}

void COptionUI::Render_GUI()
{
	if (ImGui::Button("OpenTab"))
		isTabOpen = !isTabOpen;

	if (isTabOpen) {
		Render_ImporterTab();
	}

	Render_Logs();
}

void COptionUI::Render_ImporterTab()
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("Import_Option", &isTabOpen, ImGuiWindowFlags_NoCollapse);
	if (ImGui::Button("Reset")) {
		m_Pathes.clear();
		m_ParentPath.clear();
		m_SavePath.clear();
		m_iSaving_Index = {};
	}
	/*옵션 1 폴더 경로 일괄 선택*/
	ImGui::SeparatorText("Parent Folder");
	if (m_ParentPath.empty()) {
		if (ImGui::Button("Select Folder")) {
			m_ParentPath = Helper::OpenFolder_Dialogue();
		}
	}
	else {
		ImGui::Text(m_ParentPath.c_str());
	}

	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight * (m_Pathes.size() + 2)) + (ImGui::GetStyle().WindowPadding.y * 2);

	ImGui::SeparatorText("Selected Model Folders");
	if (ImGui::Button("Select Model Folders")) {
		m_Pathes = Helper::OpenMultiFolders();
	}

	ImGui::BeginChild("##SelectedModels", ImVec2{ 0, childHeight }, true);
	for (auto& Pathes : m_Pathes)
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text(Pathes.c_str());
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("This Folder Has Multiple DAE files"); //각 폴더 안에 모든 DAE를 순회할 것임.
			ImGui::EndTooltip();
		}
	}
	ImGui::EndChild();

	/*옵션 2 추출 모델 옵션 -> 스킨드? 스태틱?*/
	ImGui::Checkbox("Save Model", &bSaveModel);
	ImGui::Checkbox("Save Material", &bSaveMaterial);

	if (bSaveModel) {
		if (ImGui::RadioButton("is Skinned", !isStaticModel))
			isStaticModel = false;
		if (ImGui::RadioButton("is Static", isStaticModel))
			isStaticModel = true;
	}

	/*옵션 2.5 셰이더 특정해서 넣어버리기*/
	static string OverrideKey;

	if (OverrideKey.capacity() < 256)
		OverrideKey.reserve(256);

	if (ImGui::InputText("Override Shader Key",
		OverrideKey.data(),
		OverrideKey.capacity() + 1,
		ImGuiInputTextFlags_CallbackResize,
		[](ImGuiInputTextCallbackData* data)
		{
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
			{
				auto str = reinterpret_cast<std::string*>(data->UserData);
				str->resize(data->BufTextLen);
				data->Buf = str->data();
			}
			return 0;
		},
		&OverrideKey))
	{
		overrideShaderKey = OverrideKey;
	}

	/*옵션 3 저장할 폴더 */
	ImGui::SeparatorText("Save Folder");
	if (ImGui::Button("Select Folder##Save")) {
		m_SavePath = Helper::OpenFolder_Dialogue();
	}
	ImGui::Text(m_SavePath.c_str());

	if (ImGui::Button("Start Exporting")) {
		if (m_ParentPath.empty() || m_Pathes.empty() || m_SavePath.empty()) {
			MSG_BOX("Please Fill All Pathes");
		}
		else {
			isReadyToSave = true;
		}
	}

	ImGui::End();
}

void COptionUI::Render_Logs()
{
	ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("Import Logs", 0, ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("Clear Logs")) {
		m_Logs.clear();
	}
	for (auto& log : m_Logs)
	{
		ImGui::Text(log.c_str());
	}

	ImGui::End();
}

HRESULT COptionUI::LookUp_Folder()
{
	if (m_iSaving_Index >= m_Pathes.size()) {
		isReadyToSave = false;
		return S_OK;
	}

	string Path = m_ParentPath + "\\" + m_Pathes[m_iSaving_Index];

	for (const auto& files : filesystem::recursive_directory_iterator(Path))
	{
		string logMsg = {};
		if (!files.exists() || !files.is_regular_file())
			continue;

		const string ext = filesystem::weakly_canonical(files.path()).extension().string();

		if (ext != ".dae") {
			continue;
		}

		const string filePath = filesystem::weakly_canonical(files.path()).string();
		string file = files.path().filename().string();
		string fileName = Helper::GetFileNameWithOutExtension(filePath);

		if (FAILED(Load_AIScene(filePath))) {
			logMsg = m_Pathes[m_iSaving_Index] + ": Load AIScene FAILED";
			m_Logs.push_back(logMsg);
			continue;
		}

		HRESULT Load = {};
		if (bSaveModel) {
			if (isStaticModel)
				Load = m_pStaticModelComp->Load_Model(m_pAIScene, fileName);
			else
				Load = m_pSkeletalModelComp->Load_Model(m_pAIScene, fileName);
		}

		if (bSaveMaterial) {
			Load = m_pMaterial->Load_Material(m_pAIScene->mNumMaterials, m_pAIScene->mMaterials, filePath);
			m_pMaterial->Override_ShaderKey(overrideShaderKey);
		}
		

		if (FAILED(Load)) {
			logMsg = fileName + ": import File FAILED";
			m_Logs.push_back(logMsg);
			continue;
		}


		if (bSaveModel) {
			if (isStaticModel)
				Load = m_pStaticModelComp->Save_Model(m_SavePath);
			else
				Load = m_pSkeletalModelComp->Save_Model(m_SavePath);
		}

		if (bSaveMaterial)
			Load = m_pMaterial->Save_Material(m_SavePath, fileName);

		if (FAILED(Load)) {
			logMsg = fileName + ": Save File FAILED";
			m_Logs.push_back(logMsg);
			continue;
		}

		logMsg = fileName + ": Completely Successed!!";
		m_Logs.push_back(logMsg);
	}
	m_iSaving_Index++;
	return S_OK;
}

HRESULT COptionUI::Load_AIScene(const string& filePath)
{
	m_Importer.FreeScene();
	unsigned int iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	m_pAIScene = m_Importer.ReadFile(filePath.c_str(), iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	return S_OK;
}

COptionUI* COptionUI::Create()
{
	COptionUI* instance = new COptionUI();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : COptionUI");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* COptionUI::Clone(INIT_DESC* pArg)
{
	COptionUI* instance = new COptionUI(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : COptionUI");
		Safe_Release(instance);
	}

	return instance;
}

void COptionUI::Free()
{
	__super::Free();
	m_Importer.FreeScene();
	Safe_Release(m_pSkeletalModelComp);
	Safe_Release(m_pStaticModelComp);
	Safe_Release(m_pMaterial);
}