#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "GameObject.h"

CMaterial::CMaterial()
{
}

CMaterial::CMaterial(const CMaterial& rhs)
	:CComponent(rhs),
	m_pShader{ rhs.m_pShader },
	m_Textures{ rhs.m_Textures },
	m_MaterialConstant{ rhs.m_MaterialConstant}
{
	Safe_AddRef(m_pShader);

	for (auto& pair : m_Textures)
		Safe_AddRef(pair.second);
}

HRESULT CMaterial::Initialize_Prototype()
{
	
	m_MaterialConstant.vMtrDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	m_MaterialConstant.vMtrlAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	m_MaterialConstant.vMtrlSpecular = _float4(1.0f, 1.0f, 1.0f, 1.f);
	m_MaterialConstant.fSpecularPow = { 0.1f };
	return S_OK;
}

HRESULT CMaterial::Initialize(COMPONENT_DESC* pArg)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(MaterialConstants);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	CGameInstance::GetInstance()->Get_Device()->CreateBuffer(&desc, nullptr, &m_pMaterialCBuffer);

	return S_OK;
}

HRESULT CMaterial::Link_Shader(const string& levelKey, const string& key)
{
	m_pShader = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Shader(levelKey, key);

	if (!m_pShader)
		return E_FAIL;

	Safe_AddRef(m_pShader);
	return S_OK;
}

HRESULT CMaterial::Link_Texture(const string& levelKey, const string& key, const string& constant)
{
	CTexture* pTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture(levelKey, key);
	if (!pTexture) {
		MSG_BOX("There is no Texture Key  : Link_Texture");
		return E_FAIL;
	}

	auto iter = m_Textures.emplace(constant, pTexture);

	if (iter.second)
		Safe_AddRef(pTexture);

	return S_OK;
}

void CMaterial::Apply_Material(const string& passConstant, ID3D11DeviceContext* pContext)
{
	Bind_InternalValues(pContext);
	m_pShader->Apply(passConstant, pContext);
}

void CMaterial::Bind_InternalValues(ID3D11DeviceContext* pContext)
{
	/*텍스처 바인딩*/
	for (auto& pair : m_Textures)
		m_pShader->Bind_Value(pair.first, pair.second->Get_SRV(), 0);

	/*상수 버퍼*/
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pContext->Map(m_pMaterialCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &m_MaterialConstant, sizeof(MaterialConstants));
	pContext->Unmap(m_pMaterialCBuffer, 0);

	m_pShader->SetConstantBuffer("MaterialBuffer", m_pMaterialCBuffer);
}

CMaterial* CMaterial::Create()
{
	CMaterial* instance = new CMaterial;
	if (FAILED(instance->Initialize_Prototype())) {
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CMaterial::Clone()
{
	CMaterial* instance = new CMaterial(*this);
	return instance;
}

void CMaterial::Free()
{
	Safe_Release(m_pShader);
	Safe_Release(m_pMaterialCBuffer);

	for (auto& pair : m_Textures)
		Safe_Release(pair.second);
}

void CMaterial::Render_GUI()
{
	ImGui::SeparatorText("Material");
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight * (m_Textures.size() +2)) + (ImGui::GetStyle().WindowPadding.y * 4);
	ImGui::BeginChild("##Material Texture", ImVec2{ childWidth,  max(min(childHeight,180), 100) }, true);
	ImGui::SeparatorText("Material Texture");
	for (auto& pair : m_Textures)
	{
		ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), pair.first.c_str());
		pair.second->Render_GUI(64);
	}
	ImGui::EndChild();

	const float propertyHeight = (textLineHeight * (15)) + (ImGui::GetStyle().WindowPadding.y * 4);
	ImGui::BeginChild("##Material Property", ImVec2{ childWidth,  max(min(propertyHeight,180), 100) }, true);
	ImGui::PushItemWidth(childWidth-40);
	ImGui::SeparatorText("Material Property");

	ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Diffuse Material");
	ImGui::SliderFloat4("##DiffuseMaterial", reinterpret_cast<float*>(&m_MaterialConstant.vMtrDiffuse), 0.f, 1.f, "%.2f");

	ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Ambient Material");
	ImGui::SliderFloat4("##AmbientMaterial", reinterpret_cast<float*>(&m_MaterialConstant.vMtrlAmbient), 0.f, 1.f, "%.2f");

	ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Specular Material");
	ImGui::SliderFloat4("##SpecularMaterial", reinterpret_cast<float*>(&m_MaterialConstant.vMtrlSpecular), 0.f, 1.f, "%.2f");

	ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Specular Power");
	ImGui::SliderFloat("##SpecularPow", (&m_MaterialConstant.fSpecularPow), 0.f, 1.f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::EndChild();
}