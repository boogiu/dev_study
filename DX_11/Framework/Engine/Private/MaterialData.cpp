#include "MaterialData.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "Texture.h"

_uint CMaterialData::s_NextID = 1;


CMaterialData::CMaterialData(const string& MaterialKey)
	:m_MaterialKey(MaterialKey)
{
	m_MaterialConstant.vMtrlAmbient = { 0.5f,0.5f,0.5f,1.f };
}

CMaterialData::~CMaterialData()
{
}

HRESULT CMaterialData::Initialize(ID3D11Device* pDevice, const string& levelKey, ifstream& ifs, const string& directory)
{
	MATERIAL_INFO_HEADER infoHeader = {};

	ifs.read(reinterpret_cast<char*>(&infoHeader),sizeof(MATERIAL_INFO_HEADER));

	m_MaterialConstant=infoHeader.materialConstant;
	m_passConstant=infoHeader.passConstant;
	Link_Shader(levelKey, infoHeader.ShaderKey);

	for (size_t i = 0; i < infoHeader.TextureTypeCount; i++)
	{
		TEXTURE_FILE_HEADER textureHeader = {};
		ifs.read(reinterpret_cast<char*>(&textureHeader), sizeof(TEXTURE_FILE_HEADER));
		for (size_t i = 0; i < textureHeader.TextureCount; i++)
		{
			TEXTURE_INFO_HEADER textureInfoHeader = {};
			ifs.read(reinterpret_cast<char*>(&textureInfoHeader), sizeof(TEXTURE_INFO_HEADER));
			CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(textureInfoHeader.TextureKey, directory+ textureInfoHeader.TextureKey);
			Link_Texture(levelKey, textureInfoHeader.TextureKey, static_cast<TEXTURE_TYPE>(textureHeader.typeID));
		}
	}

	CreateCBuffer(pDevice);
	return S_OK;
}

HRESULT CMaterialData::CreateCBuffer(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(MaterialConstants);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	pDevice->CreateBuffer(&desc, nullptr, &m_pMaterialCBuffer);
	return E_NOTIMPL;
}

void CMaterialData::ApplyData(ID3D11DeviceContext* pContext, _uint Index)
{
	for (auto& pair : m_Textures) {
		if (Index < pair.second.size())
			m_pShader->Bind_Value(ConvertToConstant(pair.first), pair.second[Index]->Get_SRV(), 0);
		else
			m_pShader->Bind_Value(ConvertToConstant(pair.first), pair.second[0]->Get_SRV(), 0);
	}

	/*상수 버퍼*/
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pContext->Map(m_pMaterialCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &m_MaterialConstant, sizeof(MaterialConstants));
	pContext->Unmap(m_pMaterialCBuffer, 0);

	m_pShader->SetConstantBuffer("MaterialBuffer", m_pMaterialCBuffer);
	m_pShader->Apply(m_passConstant, pContext);
}

HRESULT CMaterialData::GetPassSignature(D3DX11_PASS_DESC* pOutPassDesc)
{
	return m_pShader->GetPassSignature(m_passConstant, pOutPassDesc);
}

void CMaterialData::Render_GUI()
{
	if (m_Textures.empty())
		return;

	for (auto& pair : m_Textures) {
		ImGui::Text(ConvertToConstant(pair.first).c_str());
		const auto& vector = pair.second;

		for (size_t i = 0; i < vector.size(); i++)
		{
			vector[i]->Render_GUI(64);
		}
	}
}

HRESULT CMaterialData::Link_Texture(const string& levelKey, const string& textureKey, TEXTURE_TYPE eType)
{
	CTexture* pTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture(levelKey, textureKey);
	if (!pTexture) {
		MSG_BOX("There is no Texture Key  : Link_Texture");
		return E_FAIL;
	}

	m_Textures[eType].push_back(pTexture);
	Safe_AddRef(pTexture);

	return S_OK;
}

HRESULT CMaterialData::Link_Shader(const string& levelKey, const string& shaderKey)
{
	m_pShader = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Shader(levelKey, shaderKey);

	if (!m_pShader)
		return E_FAIL;

	Safe_AddRef(m_pShader);
	return S_OK;
}

string CMaterialData::ConvertToConstant(TEXTURE_TYPE eType)
{
	switch (eType)
	{
	case Engine::TEXTURE_TYPE::NONE:
		break;
	case Engine::TEXTURE_TYPE::DIFFUSE:
		return "g_DiffuseTexture";
	case Engine::TEXTURE_TYPE::SPECULAR:
		break;
	case Engine::TEXTURE_TYPE::AMBIENT:
		break;
	case Engine::TEXTURE_TYPE::EMISSIVE:
		break;
	case Engine::TEXTURE_TYPE::HEIGHT:
		break;
	case Engine::TEXTURE_TYPE::NORMALS:
		break;
	case Engine::TEXTURE_TYPE::SHININESS:
		break;
	case Engine::TEXTURE_TYPE::OPACITY:
		break;
	case Engine::TEXTURE_TYPE::DISPLACEMENT:
		break;
	case Engine::TEXTURE_TYPE::LIGHTMAP:
		break;
	case Engine::TEXTURE_TYPE::REFLECTION:
		break;
	case Engine::TEXTURE_TYPE::BASE_COLOR:
		break;
	case Engine::TEXTURE_TYPE::NORMAL_CAMERA:
		break;
	case Engine::TEXTURE_TYPE::EMISSION_COLOR:
		break;
	case Engine::TEXTURE_TYPE::METALNESS:
		break;
	case Engine::TEXTURE_TYPE::DIFFUSE_ROUGHNESS:
		break;
	case Engine::TEXTURE_TYPE::AMBIENT_OCCLUSION:
		break;
	case Engine::TEXTURE_TYPE::UNKNOWN:
		break;
	case Engine::TEXTURE_TYPE::SHEEN:
		break;
	case Engine::TEXTURE_TYPE::CLEARCOAT:
		break;
	case Engine::TEXTURE_TYPE::TRANSMISSION:
		break;
	case Engine::TEXTURE_TYPE::MAYA_BASE:
		break;
	case Engine::TEXTURE_TYPE::MAYA_SPECULAR:
		break;
	case Engine::TEXTURE_TYPE::MAYA_SPECULAR_COLOR:
		break;
	case Engine::TEXTURE_TYPE::MAYA_SPECULAR_ROUGHNESS:
		break;
	case Engine::TEXTURE_TYPE::ANISOTROPY:
		break;
	case Engine::TEXTURE_TYPE::GLTF_METALLIC_ROUGHNESS:
		break;
	default:
		break;
	}
	return string();
}


CMaterialData* CMaterialData::Create(ID3D11Device* pDevice, const string& levelKey, const string& MaterialKey, ifstream& ifs, const string& directory)
{
	CMaterialData* instance = new CMaterialData(MaterialKey);
	if (FAILED(instance->Initialize(pDevice, levelKey,ifs, directory))) {
		Safe_Release(instance);
	}
	return instance;
}

void CMaterialData::Free()
{
	Safe_Release(m_pShader);
	Safe_Release(m_pMaterialCBuffer);

	for (auto& pair : m_Textures)
		for(auto& tex : pair.second)
			Safe_Release(tex);

	m_Textures.clear();
}
