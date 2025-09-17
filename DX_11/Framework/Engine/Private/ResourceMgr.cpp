#include "ResourceMgr.h"
#include "GameInstance.h"
#include "ILevelService.h"
#include "IAudioService.h"
#include "Helper_Func.h"

#include "VIBuffer.h"
#include "VI_Rect.h"
#include "VI_Cube.h"
#include "VI_Terrain.h"

#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "SoundData.h"
#include "Mesh.h"
#include "MaterialData.h"
#include "Skeleton.h"

CResourceMgr::CResourceMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }, m_pContext{ pContext }, m_pInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pInstance);
}

HRESULT CResourceMgr::Initiallize()
{
	Load_InitialResource();
	return S_OK;
}


void CResourceMgr::Clear_Resource(const string& levelTag)
{
	int index = ValidLevel(levelTag);
	if (index == -1) {
		MSG_BOX("Wrong Level Tag. :Clear_Resource ");
		return;
	}

	for (auto& pair : m_Resources[index].m_Sounds)
		Safe_Release(pair.second);

	for (auto& pair : m_Resources[index].m_Buffers)
		Safe_Release(pair.second);

	for (auto& pair : m_Resources[index].m_Sounds)
		Safe_Release(pair.second);

	for (auto& pair : m_Resources[index].m_Textures)
		Safe_Release(pair.second);

	for (auto& pair : m_Resources[index].m_Shaders)
		Safe_Release(pair.second);

	for (auto& pair : m_Resources[index].m_Meshes)
		for(auto& mesh : pair.second)
			Safe_Release(mesh);

	for (auto& pair : m_Resources[index].m_MaterialDatas)
		for (auto& material : pair.second)
			Safe_Release(material);

	m_Resources[index] = {};
}

HRESULT CResourceMgr::Sync_To_Level()
{
	ILevelService* pLevelMgr = m_pInstance->Get_LevelMgr();

	if (!pLevelMgr) {
		MSG_BOX("There is No Level in Level Manager : CResourceMgr");
		return E_FAIL;
	}

	vector<string> LevelList = pLevelMgr->Get_LevelList();
	int iCount = 0;
	for (string& name : LevelList) {
		//m_ResourcePool.emplace(name, ResourcePool());
		auto iter = m_LevelIndex.emplace(name, iCount);
		if (iter.second == true)
			m_Resources.push_back(RS_Pool());

		iCount++;
	}
}

CSoundData* CResourceMgr::Load_Sound(const string& levelTag, const string& soundKey)
{
	int index = ValidLevel(levelTag);
	if (index == -1) {
		MSG_BOX("Wrong Level Tag. :Load_Sound ");
		return nullptr;
	}

	auto& map = m_Resources[index].m_Sounds;
	auto iter = map.find(soundKey);

	if (iter != map.end()) return iter->second;

	CSoundData* pData = CSoundData::Create(MakePath(soundKey), soundKey);
	map.emplace(soundKey, pData);

	return pData;
}

CVIBuffer* CResourceMgr::Load_VIBuffer(const string& levelTag, const string& bufferKey, BUFFER_TYPE eType)
{
	int index = ValidLevel(levelTag);
	if (index == -1) {
		MSG_BOX("Wrong Level Tag. :Load_VIBuffer ");
		return nullptr;
	}
	auto& map = m_Resources[index].m_Buffers;
	auto iter = map.find(bufferKey);
	if (iter != map.end()) return iter->second;

	CVIBuffer* buffer = nullptr;
	switch (eType)
	{
	case Engine::BUFFER_TYPE::BASIC_RECT:
		buffer = CVI_Rect::Create(m_pDevice, bufferKey);
		break;
	case Engine::BUFFER_TYPE::BASIC_CUBE:
		break;
	case Engine::BUFFER_TYPE::BASIC_SPHERE:
		break;
	case Engine::BUFFER_TYPE::TERRAIN:
		buffer = CVI_Terrain::Create(m_pDevice, bufferKey, MakePath(bufferKey));
		break;
	default:
		break;
	}

	if (buffer)
		map.emplace(bufferKey, buffer);

	return buffer;
}

const vector<CMesh*>& CResourceMgr::Load_Mesh(const string& levelTag, const string& modelKey)
{
	vector<CMesh*> meshContainer;

	int index = ValidLevel(levelTag);
	if (index == -1) {
		MSG_BOX("Wrong Level Tag. :Load_Mesh ");
		return meshContainer;
	}
	auto& map = m_Resources[index].m_Meshes;
	auto iter = map.find(modelKey);
	if (iter != map.end()) return iter->second;

	string filePath = MakePath(modelKey);
	ifstream ifs(filePath.c_str(), ios::binary);

	if (!ifs.is_open()) {
		MSG_BOX("There is No File. :Load_Mesh ");
		return vector<CMesh*>();
	}

	MESH_FILE_HEADER fileHeader = {};
	ifs.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
	for (int i = 0; i < fileHeader.MeshCount; ++i) {
		CMesh* newMesh = CMesh::Create(m_pDevice, string(fileHeader.meshKey) + to_string(i), ifs);

		if (newMesh)
			meshContainer.push_back(newMesh);
		else
		{
			ifs.close();
			for (auto& mesh : meshContainer)
				Safe_Release(mesh);
			return vector<CMesh*>();
		}
	}
	
	ifs.close();

	auto ResultIter = map.emplace(modelKey, meshContainer);
	return ResultIter.first->second;
}

const vector<CMaterialData*>& CResourceMgr::Load_MaterialData(const string& levelTag, const string& materialKey)
{
	vector<CMaterialData*> materialDataContainer;

	int index = ValidLevel(levelTag);
	if (index == -1) {
		MSG_BOX("Wrong Level Tag. :Load_Mesh ");
		return materialDataContainer;
	}
	auto& map = m_Resources[index].m_MaterialDatas;
	auto iter = map.find(materialKey);
	if (iter != map.end()) return iter->second;

	string filePath = MakePath(materialKey);
	string directory = filesystem::path(filePath).parent_path().string() + "/";
	ifstream ifs(filePath.c_str(), ios::binary);

	if (!ifs.is_open()) {
		MSG_BOX("There is No File. :Load_Mesh ");
		return vector<CMaterialData*>();
	}

	MATERIAL_FILE_HEADER fileHeader = {};
	ifs.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
	for (int i = 0; i < fileHeader.MaterialDataCount; ++i) {
		CMaterialData* newMaterial = CMaterialData::Create(m_pDevice, levelTag ,string(fileHeader.materialDataKey) + to_string(i), ifs,directory);

		if (newMaterial)
			materialDataContainer.push_back(newMaterial);
		else
		{
			ifs.close();
			for (auto& mesh : materialDataContainer)
				Safe_Release(mesh);
			return vector<CMaterialData*>();
		}
	}

	ifs.close();

	auto ResultIter = map.emplace(materialKey, materialDataContainer);
	return ResultIter.first->second;
}

CShader* CResourceMgr::Load_Shader(const string& levelTag, const string& shaderKey)
{
	int index = ValidLevel(levelTag);
	if (index == -1) {
		MSG_BOX("Wrong Level Tag. :Load_Shader ");
		return nullptr;
	}

	auto& map = m_Resources[index].m_Shaders;
	auto iter = map.find(shaderKey);

	if (iter != map.end()) return iter->second;

	CShader* pData = CShader::Create(m_pDevice, MakePath(shaderKey), shaderKey);
	map.emplace(shaderKey, pData);

	return pData;
}

CTexture* CResourceMgr::Load_Texture(const string& levelTag, const string& textureKey)
{
	int index = ValidLevel(levelTag);
	if (index == -1) {
		MSG_BOX("Wrong Level Tag. :Load_Texture ");
		return nullptr;
	}

	auto& map = m_Resources[index].m_Textures;
	auto iter = map.find(textureKey);

	if (iter != map.end()) return iter->second;

	wstring path = Helper::ConvertToWideString(MakePath(textureKey));
	CTexture* pData = CTexture::Create(m_pDevice, path, textureKey);
	map.emplace(textureKey, pData);

	return pData;
}

CSkeleton* CResourceMgr::Load_Skeleton(const string& levelTag, const string& skeletonKey)
{
	int index = ValidLevel(levelTag);
	if (index == -1) {
		MSG_BOX("Wrong Level Tag. :Load_Skeleton ");
		return nullptr;
	}

	auto& map = m_Resources[index].m_Skeletons;
	auto iter = map.find(skeletonKey);

	if (iter != map.end()) return iter->second;

	string filePath = MakePath(skeletonKey);

	CSkeleton* pData = CSkeleton::Create(filePath);
	map.emplace(skeletonKey, pData);

	return pData;
}

string CResourceMgr::Get_ResourcePath(const string& resourceKey)
{
	auto iter = m_KeyPath.find(resourceKey);
	if (iter != m_KeyPath.end()) return iter->second;
	else return string();
}

HRESULT CResourceMgr::Add_ResourcePath(const string& resourceKey, const string& resourcePath)
{
	auto iter = m_KeyPath.find(resourceKey);

	if (iter != m_KeyPath.end()) return E_FAIL;

	m_KeyPath.emplace(resourceKey, resourcePath);
	return S_OK;
}

_int CResourceMgr::ValidLevel(const string& levelKey)
{
	auto iter = m_LevelIndex.find(levelKey);

	if (iter == m_LevelIndex.end())
		return -1;
	return iter->second;
}

void CResourceMgr::Load_InitialResource()
{
	m_LevelIndex.emplace(G_GlobalLevelKey, 0);
	m_Resources.resize(1);
	Add_ResourcePath("VTX_TexPos.hlsl",  "../../EngineSDK/Inc/Engine_Shader/VTX_TexPos.hlsl");
	Add_ResourcePath("VTX_Mesh.hlsl",  "../../EngineSDK/Inc/Engine_Shader/VTX_Mesh.hlsl");
	Add_ResourcePath("VTX_NorTex.hlsl",  "../../EngineSDK/Inc/Engine_Shader/VTX_NorTex.hlsl");
	Add_ResourcePath("VTX_SkinMesh.hlsl",  "../../EngineSDK/Inc/Engine_Shader/VTX_SkinMesh.hlsl");
	Add_ResourcePath("Shader_Define.hlsl",  "../../EngineSDK/Inc/Engine_Shader/Shader_Define.hlsl");
	Add_ResourcePath("Anim.dat",  "../../Anim.dat");

	m_Resources[0].m_Buffers.emplace("Engine_Default_Rect.hlsl", CVI_Rect::Create(m_pDevice, "Engine_Default_Rect"));
	/*Load_Shader(G_GlobalLevelKey, "VTX_TexPos.hlsl");
	Load_Shader(G_GlobalLevelKey, "VTX_Mesh.hlsl");
	Load_Shader(G_GlobalLevelKey, "Shader_Define.hlsl");*/
	m_Resources[0].m_Shaders.emplace("VTX_TexPos.hlsl", CShader::Create(m_pDevice, "../../EngineSDK/Inc/Engine_Shader/VTX_TexPos.hlsl", "VTX_TexPos.hlsl"));
	m_Resources[0].m_Shaders.emplace("VTX_Mesh.hlsl", CShader::Create(m_pDevice, "../../EngineSDK/Inc/Engine_Shader/VTX_Mesh.hlsl", "VTX_Mesh.hlsl"));
	m_Resources[0].m_Shaders.emplace("VTX_NorTex.hlsl", CShader::Create(m_pDevice, "../../EngineSDK/Inc/Engine_Shader/VTX_NorTex.hlsl", "VTX_NorTex.hlsl"));
	m_Resources[0].m_Shaders.emplace("VTX_SkinMesh.hlsl", CShader::Create(m_pDevice, "../../EngineSDK/Inc/Engine_Shader/VTX_SkinMesh.hlsl", "VTX_SkinMesh.hlsl"));
	//m_Resources[0].m_Shaders.emplace("Shader_Define.hlsl", CShader::Create(m_pDevice, "../../EngineSDK/Inc/Engine_Shader/Shader_Define.hlsl", "Shader_Define.hlsl"));
	
}

string CResourceMgr::MakePath(const string& pathKey)
{
	auto iter = m_KeyPath.find(pathKey);
	if (iter != m_KeyPath.end()) {
		return iter->second;
	}

	return string();
}


CResourceMgr* CResourceMgr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CResourceMgr* instance = new CResourceMgr(pDevice, pContext);
	if (FAILED(instance->Initiallize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CResourceMgr::Free()
{
	__super::Free();

	for (auto& pair : m_LevelIndex)
		Clear_Resource(pair.first);

	m_LevelIndex.clear();
	m_Resources.clear();

	Safe_Release(m_pInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
