#include "ResourceMgr.h"
#include "ILevelService.h"
#include "GameInstance.h"

#include "VIBuffer.h"
#include "VI_Rect.h"
#include "Shader.h"

CResourceMgr::CResourceMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{pDevice}, m_pContext{pContext}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CResourceMgr::~CResourceMgr()
{
}

HRESULT CResourceMgr::Initiallize()
{
	return S_OK;
}

FMOD::Sound* CResourceMgr::Load_Sound(const string& levelTag, const string& key)
{
	auto iter = m_Sounds.find(levelTag);
	if (iter == m_Sounds.end())
		return nullptr;

	FMOD::Sound* pSound = iter->second.Find(key);

	if (pSound == nullptr)
	{
		/* 파일 로드 로직 */
	}
	return pSound;
}

CVIBuffer* CResourceMgr::Load_VIBuffer(const string& levelTag, const string& key, INIT_DESC* pArg)
{
	auto iter = m_Buffers.find(levelTag);

	if (iter == m_Buffers.end())
		return nullptr;

	CVIBuffer* pBuffer = iter->second.Find(key);

	if (pBuffer != nullptr)
		return pBuffer;

	CVIBuffer::VI_LOAD_DESC*  bufferDesc = static_cast<CVIBuffer::VI_LOAD_DESC*>(pArg);

	switch (bufferDesc->m_eType)
	{
	case Engine::BUFFER_TYPE::BASIC_RECT:
		 pBuffer = CVI_Rect::Create(m_pDevice);
		break;
	case Engine::BUFFER_TYPE::BASIC_CUBE:
		break;
	case Engine::BUFFER_TYPE::BASIC_SPHERE:
		break;
	case Engine::BUFFER_TYPE::MESH:
		break;
	default:
		break;
	}

	iter->second.Add_Resource(key, pBuffer);
	pBuffer->Set_Key(key);
	return pBuffer;
}


CShader* CResourceMgr::Load_Shader(const string& levelTag, const string& key)
{

	auto iter = m_Shaders.find(levelTag);

	if (iter == m_Shaders.end())
		return nullptr;

	CShader* pShader = iter->second.Find(key);

	if (pShader == nullptr)
		pShader  = CShader::Create(m_pDevice, key);
	if (pShader != nullptr)
		pShader->Set_Key(key);

	iter->second.Add_Resource(key, pShader);
	return pShader;
}

void CResourceMgr::Clear_Resource(const string& levelTag)
{
	m_Buffers[levelTag].Clear();
	m_Sounds[levelTag].Clear();
	m_Shaders[levelTag].Clear();
}

HRESULT CResourceMgr::Sync_To_Level()
{
	ILevelService* pLevelMgr = CGameInstance::GetInstance()->Get_LevelMgr();

	if (!pLevelMgr) {
		MSG_BOX("There is No Level in Level Manager : CResourceMgr");
		return E_FAIL;
	}

	vector<string> LevelList = pLevelMgr->Get_LevelList();

	for (string& name : LevelList) {
		m_Buffers.emplace(name,ResourcePool<CVIBuffer>());
		m_Sounds.emplace(name,ResourcePool<FMOD::Sound>());
		m_Shaders.emplace(name,ResourcePool<CShader>());
	}
}

void CResourceMgr::Load_InitialResource()
{
	auto buffer_result = m_Buffers.emplace(G_GlobalLevelKey, ResourcePool<CVIBuffer>());//{이터, 불리안} 반환
	auto& bufferPool = buffer_result.first->second;
	bufferPool.Add_Resource("Engine_Default_Rect", CVI_Rect::Create(m_pDevice));

	auto shader_result = m_Shaders.emplace(G_GlobalLevelKey, ResourcePool<CShader>());
	auto& shaderPool = shader_result.first->second;
	shaderPool.Add_Resource("VTX_TexPos.hlsl", CShader::Create(m_pDevice, "../../EngineSDK/Inc/Engine_Shader/VTX_TexPos.hlsl"));
}

CResourceMgr* CResourceMgr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CResourceMgr* instance = new CResourceMgr(pDevice,  pContext);
	if (FAILED(instance->Initiallize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CResourceMgr::Free()
{

	for (auto& pair : m_Buffers)
		pair.second.Clear();

	for (auto& pair : m_Sounds)
		pair.second.Clear();


	for (auto& pair : m_Shaders)
		pair.second.Clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
