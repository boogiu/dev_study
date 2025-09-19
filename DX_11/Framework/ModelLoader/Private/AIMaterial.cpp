#include "AIMaterial.h"
#include "Texture.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "Helper_Func.h"

CAIMaterial::CAIMaterial(const string& MaterialKey)
	:CMaterialData(MaterialKey)
{
}

HRESULT CAIMaterial::Initialize(ID3D11Device* pDevice, const aiMaterial* pAIMaterial, const string& fileDirectory)
{
	for (size_t i = 0; i < MAX_TEXTURE_TYPE_VALUE; i++)
	{
		size_t texCount = pAIMaterial->GetTextureCount(static_cast<aiTextureType>(i));

		for (size_t j = 0; j < texCount; j++)
		{
			aiString     strTexturePath;
			pAIMaterial->GetTexture(static_cast<aiTextureType>(i), j, &strTexturePath);

			string extention = filesystem::path(strTexturePath.C_Str()).extension().string(); //".png"
			string fileName = Helper::GetFileNameWithOutExtension(strTexturePath.C_Str()); //"avsAlv" or "avsALv.0"
			string BaseName = Helper::GetFileBaseName(fileName);

			string filePath = fileDirectory + "\\" + BaseName + extention;
			/*일단 베이스 네임으로(인덱스 제외 후) 검색*/
			if (filesystem::exists(filePath)) { //있으면 로드
				CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(BaseName + extention, filePath);
				Link_Texture(G_GlobalLevelKey, BaseName + extention, static_cast<TEXTURE_TYPE>(i));
			}

			/*순회 검색 시작*/
			_uint Index = {};

			while (true) {
				string targetName = BaseName + "." + to_string(Index);
				string IndexedPath = fileDirectory + "\\" + targetName + extention;

				if (filesystem::exists(IndexedPath)) { //있으면 로드
					CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(targetName + extention, IndexedPath);
					Link_Texture(G_GlobalLevelKey, targetName + extention, static_cast<TEXTURE_TYPE>(i));
					Index++;
				}
				else {
					break;
				}
			}
		}
	}

	m_passConstant = "Opaque";

	__super::CreateCBuffer(pDevice);
	return S_OK;
}

void CAIMaterial::Save_MaterialData(ID3D11DeviceContext* pContext, ofstream& ofs, const string& directory)
{
	MATERIAL_INFO_HEADER infoHead = {};
	strcpy_s(infoHead.passConstant, sizeof(infoHead.passConstant), m_passConstant.c_str());
	strcpy_s(infoHead.ShaderKey, sizeof(infoHead.ShaderKey), m_pShader->Get_Key().c_str());
	memcpy(&infoHead.materialConstant, &m_MaterialConstant, sizeof(MaterialConstants));

	infoHead.TextureTypeCount = m_Textures.size();
	ofs.write(reinterpret_cast<const char*>(&infoHead), sizeof(infoHead));

	for (auto pair : m_Textures) {
		TEXTURE_FILE_HEADER texHeader = {};
		texHeader.TextureCount = pair.second.size();
		texHeader.typeID = static_cast<_uint>(pair.first);
		ofs.write(reinterpret_cast<const char*>(&texHeader), sizeof(TEXTURE_FILE_HEADER));

		for (auto& tex : pair.second) {
			TEXTURE_INFO_HEADER texInfo = {};
			string textureKey = Helper::GetFileNameWithOutExtension(tex->Get_Key()) + ".dds";
			strcpy_s(texInfo.TextureKey, sizeof(texInfo.TextureKey), textureKey.c_str());
			ofs.write(reinterpret_cast<const char*>(&texInfo), sizeof(texInfo));
			if (FAILED(Helper::SaveTextureToDDs(pContext, directory + "\\" + textureKey, tex->Get_SRV()))) {
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

	__super::Render_GUI();

}

void CAIMaterial::LinkShader(const string& shader)
{
	Link_Shader(G_GlobalLevelKey, shader);
}

CAIMaterial* CAIMaterial::Create(ID3D11Device* pDevice, const aiMaterial* pAIMaterial, const string& MaterialKey, const string& fileDirectory)
{
	CAIMaterial* instance = new CAIMaterial(MaterialKey);
	if (FAILED(instance->Initialize(pDevice, pAIMaterial, fileDirectory))) {
		Safe_Release(instance);
	}
	return instance;
}

void CAIMaterial::Free()
{
	__super::Free();
}
