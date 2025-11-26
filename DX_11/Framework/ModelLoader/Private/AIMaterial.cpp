#include "AIMaterial.h"
#include "Texture.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "Helper_Func.h"

CAIMaterial::CAIMaterial()
{
}


HRESULT CAIMaterial::Initialize(const aiMaterial* pAIMaterial, const string& fileDirectory)
{
	m_MaterialKey = pAIMaterial->GetName().C_Str();
	for (const auto& entry : filesystem::directory_iterator(fileDirectory))
	{
		if (!entry.is_regular_file()) continue;

		string filePath = entry.path().string();
		string fileName = entry.path().stem().string(); // 확장자 제외
		string ext = entry.path().extension().string();

		string parentFolder = filesystem::path(fileDirectory).parent_path().string();
		string ParentName = filesystem::path(parentFolder).filename().string();

		if (ext != ".png" && ext != ".jpg" && ext != ".jpeg" && ext != ".bmp" && ext != ".dds")
			continue;

		string lowerFile = Helper::ToLower(fileName);
		string lowerMat = Helper::ToLower(m_MaterialKey);

		auto StartsWith = [](const std::string& str, const std::string& prefix)
			{
				return str.rfind(prefix, 0) == 0; // prefix가 맨 앞에 있으면 true
			};

		auto EndsWith = [](const std::string& str, const std::string& suffix)
			{
				if (str.length() < suffix.length()) return false;
				return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
			};

		auto EndsWithFlexible = [](const std::string& str, const std::string& suffix)
			{
				if (str.length() < suffix.length()) return false;

				// 기본 접미사 위치 찾기
				size_t pos = str.rfind(suffix);
				if (pos == std::string::npos) return false;

				// suffix가 문자열 끝 근처에 있어야 함
				size_t suffixEnd = pos + suffix.length();

				// suffix 바로 뒤에 오는 문자가 . _ 숫자 이거나 문자열 끝이면 허용
				if (suffixEnd == str.length()) return true; // 정확히 끝

				char next = str[suffixEnd];
				if (next == '.' || next == '_' || (next >= '0' && next <= '9'))
					return true;

				return false;
			};

		if (!StartsWith(lowerFile, lowerMat + "_"))
			continue;

		TEXTURE_TYPE textureType = TEXTURE_TYPE::NONE;

		if (EndsWith(lowerFile, "_grdedge"))   textureType = TEXTURE_TYPE::GRADATION_EDGE;
		else if (EndsWith(lowerFile, "_grd"))  textureType = TEXTURE_TYPE::GRADATION;
		else if (EndsWithFlexible(lowerFile, "_mix"))  textureType = TEXTURE_TYPE::MIX;
		else if (EndsWith(lowerFile, "_spc"))  textureType = TEXTURE_TYPE::SPECULAR;
		else if (EndsWith(lowerFile, "_albgry")) textureType = TEXTURE_TYPE::ALBEDO_GRAY;
		else if (EndsWith(lowerFile, "_nrmory")) textureType = TEXTURE_TYPE::NORMAL_ORY;
		else if (EndsWithFlexible(lowerFile, "_nrm"))  textureType = TEXTURE_TYPE::NORMAL;
		else if (EndsWith(lowerFile, "_albory")) textureType = TEXTURE_TYPE::ALBEDO_ORY;
		else if (EndsWithFlexible(lowerFile, "_alb"))  textureType = TEXTURE_TYPE::ALBEDO;
		else if (EndsWith(lowerFile, "_emiory")) textureType = TEXTURE_TYPE::EMMISION_ORY;
		else if (EndsWith(lowerFile, "_emi"))  textureType = TEXTURE_TYPE::EMMISION;
		else if (EndsWith(lowerFile, "_op"))   textureType = TEXTURE_TYPE::OPACITY;
		else if (EndsWith(lowerFile, "_ind"))  textureType = TEXTURE_TYPE::INDEXMAP;
		else if (EndsWith(lowerFile, "_sclxy")) textureType = TEXTURE_TYPE::SCALEXY;
		else if (EndsWith(lowerFile, "_sclx")) textureType = TEXTURE_TYPE::SCALEX;
		else if (EndsWith(lowerFile, "_scly")) textureType = TEXTURE_TYPE::SCALEY;
		else
			continue; // 일치하는 접미어가 없으면 스킵

		CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(to_string(m_MaterialDataID)+fileName, filePath);
		Link_Texture(G_GlobalLevelKey, to_string(m_MaterialDataID) + fileName, textureType);
	}

	m_passConstant = "ForceSee";

	for (size_t i = 0; i < MAX_TEXTURE_TYPE_VALUE; i++)
	{
		if (ConvertToConstant(static_cast<TEXTURE_TYPE>(i)).empty()) continue;
		textureTypes.push_back(i);
	}

	return S_OK;
}


void CAIMaterial::Save_MaterialData(ID3D11DeviceContext* pContext, ofstream& ofs, const string& directory, const string& overrideKey)
{
	MATERIAL_INFO_HEADER infoHead = {};

	strcpy_s(infoHead.materialDataKey, sizeof(infoHead.materialDataKey), m_MaterialKey.c_str());
	strcpy_s(infoHead.passConstant, sizeof(infoHead.passConstant), m_passConstant.c_str());
	if(overrideKey.empty()&& m_pShader)
		strcpy_s(infoHead.ShaderKey, sizeof(infoHead.ShaderKey), m_pShader->Get_Key().c_str());
	else {
		strcpy_s(infoHead.ShaderKey, sizeof(infoHead.ShaderKey), overrideKey.c_str());
	}
	memcpy(&infoHead.materialConstant, &m_DefaultMaterialConstant, sizeof(MaterialConstants));

	infoHead.TextureTypeCount = m_Textures.size();
	ofs.write(reinterpret_cast<const char*>(&infoHead), sizeof(infoHead));

	for (auto pair : m_Textures) {
		TEXTURE_FILE_HEADER texHeader = {};
		texHeader.TextureCount = pair.second.size();
		texHeader.typeID = static_cast<_uint>(pair.first);
		ofs.write(reinterpret_cast<const char*>(&texHeader), sizeof(TEXTURE_FILE_HEADER));

		for (size_t i = 0; i < pair.second.size(); i++)
		{
			TEXTURE_INFO_HEADER texInfo = {};
			string textureKey = Helper::GetFileNameWithOutExtension(pair.second[i]->Get_Key()) + "."+to_string(i) + ".dds";
			strcpy_s(texInfo.TextureKey, sizeof(texInfo.TextureKey), textureKey.c_str());
			ofs.write(reinterpret_cast<const char*>(&texInfo), sizeof(texInfo));
			if (FAILED(Helper::SaveTextureToDDs(pContext, directory + "\\" + textureKey, pair.second[i]->Get_SRV()))) {
				MSG_BOX("DDS SAVE FAILED : Save_MaterialData");
			}
		}
	}
}
void CAIMaterial::Render_GUI()
{
	vector<string> passes = m_pShader->Get_PassList();
	if (passes.empty())
		return;

	float childWidth = ImGui::GetContentRegionAvail().x;//->이건 넓이 설정
	ImGui::SetNextItemWidth(childWidth);
	if (ImGui::BeginCombo(string("##shaderPass").c_str(), passes[m_currentPassIndex].c_str())) {
		for (int i = 0; i < passes.size(); ++i) {
			bool isSelected = (i == m_currentPassIndex);

			if (ImGui::Selectable(passes[i].c_str(), isSelected)) {
				m_currentPassIndex = i;
				m_passConstant = passes[i];
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Add MaterialData")) {
		MaterialTabOpened = !MaterialTabOpened;
	}

	if (MaterialTabOpened)
		Render_MaterialAdd();

	__super::Render_GUI();
}

void CAIMaterial::Render_GUI(vector<_uint>& TextureIndexes)
{
	vector<string> passes = m_pShader->Get_PassList();
	if (passes.empty())
		return;

	float childWidth = ImGui::GetContentRegionAvail().x;//->이건 넓이 설정
	ImGui::SetNextItemWidth(childWidth);

	if (ImGui::BeginCombo(string("##shaderPass").c_str(), passes[m_currentPassIndex].c_str())) {
		for (int i = 0; i < passes.size(); ++i) {
			bool isSelected = (i == m_currentPassIndex);

			if (ImGui::Selectable(passes[i].c_str(), isSelected)) {
				m_currentPassIndex = i;
				m_passConstant = passes[i];
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Add MaterialData")) {
		MaterialTabOpened = !MaterialTabOpened;
	}

	if (MaterialTabOpened)
		Render_MaterialAdd();

	__super::Render_GUI(TextureIndexes);
}

void CAIMaterial::LinkShader(const string& shader)
{
	Link_Shader(G_GlobalLevelKey, shader);
}

void CAIMaterial::Render_MaterialAdd()
{
	_uint iTextureType = textureTypes[m_currentTextureTypeIndex];
	string strTextureType = ConvertToConstant(static_cast<TEXTURE_TYPE>(iTextureType));

	ImGui::Begin("Add_Texture",0,0);
	if (ImGui::BeginCombo(string("##textureTypes").c_str(), strTextureType.c_str())) {
		for (int i = 0; i < textureTypes.size(); ++i) {
			bool isSelected = (i == m_currentTextureTypeIndex);

			if (ImGui::Selectable(ConvertToConstant(static_cast<TEXTURE_TYPE>(textureTypes[i])).c_str(), isSelected)) {
				m_currentTextureTypeIndex = i;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Find_Texture")) {
		string path = Helper::OpenFile_Dialogue();
		if (path.empty()) return;
		string fileName = filesystem::path(path).filename().string();
		CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(fileName, path);
		CTexture* pTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture(G_GlobalLevelKey, fileName);
		Safe_AddRef(pTexture); 
		m_Textures[static_cast<TEXTURE_TYPE>(textureTypes[m_currentTextureTypeIndex])].push_back(pTexture);
	}


	ImGui::End();
}

CAIMaterial* CAIMaterial::Create(const aiMaterial* pAIMaterial, const string& fileDirectory)
{
	CAIMaterial* instance = new CAIMaterial();
	if (FAILED(instance->Initialize(pAIMaterial, fileDirectory))) {
		Safe_Release(instance);
	}
	return instance;
}

void CAIMaterial::Free()
{
	__super::Free();
}
