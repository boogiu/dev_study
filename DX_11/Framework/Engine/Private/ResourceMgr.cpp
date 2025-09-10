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

CResourceMgr::CResourceMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }, m_pContext{ pContext }, m_pInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pInstance);
}

CResourceMgr::~CResourceMgr()
{
}

HRESULT CResourceMgr::Initiallize()
{
	Load_InitialResource();
	return S_OK;
}

CSoundData* CResourceMgr::Load_Sound(const string& levelTag, const string& key)
{
	auto iter = m_Sounds.find(levelTag);
	if (iter == m_Sounds.end())
		return nullptr;

	CSoundData* pSound = iter->second.Find(key);

	if (pSound == nullptr)
	{
		FMOD::System* pSystem = m_pInstance->Get_AudioDev()->Get_System();
		pSound = CSoundData::Create(pSystem, MakePath(levelTag, key), key);
	}

	if (pSound)
		(iter->second).Add_Resource(key, pSound);

	return pSound;
}

CVIBuffer* CResourceMgr::Load_VIBuffer(const string& levelTag, const string& key, BUFFER_TYPE m_eType)
{
	auto iter = m_Buffers.find(levelTag);

	if (iter == m_Buffers.end())
		return nullptr;

	CVIBuffer* pBuffer = iter->second.Find(key);

	if (pBuffer != nullptr)
		return pBuffer;

	switch (m_eType)
	{
	case Engine::BUFFER_TYPE::BASIC_RECT:
		pBuffer = CVI_Rect::Create(m_pDevice, key);
		break;
	case Engine::BUFFER_TYPE::BASIC_CUBE:
		pBuffer = CVI_Cube::Create(m_pDevice, key);
		break;
	case Engine::BUFFER_TYPE::BASIC_SPHERE:
		break;
	case Engine::BUFFER_TYPE::MESH:
		break;
	case Engine::BUFFER_TYPE::TERRAIN:
		pBuffer = CVI_Terrain::Create(m_pDevice, key, MakePath(levelTag, key));
		break;
	default:
		break;
	}

	if (pBuffer)
		iter->second.Add_Resource(key, pBuffer);

	return pBuffer;
}

CShader* CResourceMgr::Load_Shader(const string& levelTag, const string& key)
{

	auto iter = m_Shaders.find(levelTag);

	if (iter == m_Shaders.end())
		return nullptr;

	CShader* pShader = iter->second.Find(key);

	if (pShader != nullptr) {
		return pShader;
	}

	pShader = CShader::Create(m_pDevice, MakePath(levelTag, key), key);
	if (pShader)
		iter->second.Add_Resource(key, pShader);

	return pShader;
}


CTexture* CResourceMgr::Load_Texture(const string& levelTag, const string& key)
{

	auto iter = m_Textures.find(levelTag);

	if (iter == m_Textures.end())
		return nullptr;

	CTexture* pTexture = iter->second.Find(key);

	if (pTexture != nullptr) {
		return pTexture;
	}
	wstring widePath = Helper::ConvertToWideString(MakePath(levelTag, key));

	pTexture = CTexture::Create(m_pDevice, widePath, key);

	if (pTexture)
		iter->second.Add_Resource(key, pTexture);

	return pTexture;
}

void CResourceMgr::Clear_Resource(const string& levelTag)
{
	m_Buffers[levelTag].Clear();
	m_Sounds[levelTag].Clear();
	m_Shaders[levelTag].Clear();
	m_Textures[levelTag].Clear();
}

HRESULT CResourceMgr::Sync_To_Level()
{
	ILevelService* pLevelMgr = m_pInstance->Get_LevelMgr();

	if (!pLevelMgr) {
		MSG_BOX("There is No Level in Level Manager : CResourceMgr");
		return E_FAIL;
	}

	vector<string> LevelList = pLevelMgr->Get_LevelList();

	for (string& name : LevelList) {
		m_Buffers.emplace(name, ResourcePool<CVIBuffer>());
		m_Sounds.emplace(name, ResourcePool<CSoundData>());
		m_Shaders.emplace(name, ResourcePool<CShader>());
		m_Textures.emplace(name, ResourcePool<CTexture>());
		m_filePath.emplace(name, ResourcePool<string>());
	}
}

void CResourceMgr::Load_InitialResource()
{
	auto buffer_result = m_Buffers.emplace(G_GlobalLevelKey, ResourcePool<CVIBuffer>());//{이터, 불리안} 반환
	auto& bufferPool = buffer_result.first->second;
	bufferPool.Add_Resource("Engine_Default_Rect", CVI_Rect::Create(m_pDevice, "Engine_Default_Rect"));

	auto shader_result = m_Shaders.emplace(G_GlobalLevelKey, ResourcePool<CShader>());
	auto& shaderPool = shader_result.first->second;
	shaderPool.Add_Resource("VTX_TexPos.hlsl", CShader::Create(m_pDevice, "../../EngineSDK/Inc/Engine_Shader/VTX_TexPos.hlsl", "VTX_TexPos.hlsl"));
	shaderPool.Add_Resource("Shader_Define.hlsl", CShader::Create(m_pDevice, "../../EngineSDK/Inc/Engine_Shader/Shader_Define.hlsl", "Shader_Define.hlsl"));
}

string CResourceMgr::MakePath(const string& levelTag, const string& key)
{
	auto iter = m_filePath.find(levelTag);
	if (iter == m_filePath.end()) {
		MSG_BOX("Wrong Level Tag. :CResourceMgr ");
		return string();
	}
	else {
		return (iter->second).Find(key);
	}
}

HRESULT CResourceMgr::Add_ResourcePath(const string& levelTag, const string& key, const string& path)
{
	auto iter = m_filePath.find(levelTag);
	if (iter == m_filePath.end())
	{
		MSG_BOX("That Key was Already Taken :CResourceMgr_Add_Resource_Path");
		return E_FAIL;
	}
	else
	{
		if (iter->second.Find(key).empty())
			iter->second.Add_Resource(key, path);
		return S_OK;
	}
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

	for (auto& pair : m_Buffers)
		pair.second.Clear();

	for (auto& pair : m_Sounds)
		pair.second.Clear();

	for (auto& pair : m_Shaders)
		pair.second.Clear();

	for (auto& pair : m_Textures)
		pair.second.Clear();

	Safe_Release(m_pInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
