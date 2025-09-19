#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "GameObject.h"
#include "MaterialData.h"

CMaterial::CMaterial()
{
}

CMaterial::CMaterial(const CMaterial& rhs)
	:CComponent(rhs),
	m_MaterialDatas(rhs.m_MaterialDatas)
{
	for (auto& data : m_MaterialDatas)
		Safe_AddRef(data);
}

HRESULT CMaterial::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMaterial::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}


HRESULT CMaterial::Link_Material(const string& levelKey, const string& materialKey)
{
	for (CMaterialData* material : CGameInstance::GetInstance()->Get_ResourceMgr()->Load_MaterialData(levelKey, materialKey)) {
		m_MaterialDatas.push_back(material);
		Safe_AddRef(material);
	}

	return S_OK;
}

CShader* CMaterial::Get_Shader(_uint Index)
{
	if (Index >= m_MaterialDatas.size()) return nullptr;

	return m_MaterialDatas[Index]->Get_Shader();
}

_uint CMaterial::Get_ShaderID(_uint Index)
{
	if (Index >= m_MaterialDatas.size()) return 0;

	return m_MaterialDatas[Index]->Get_ShaderID();
}

_uint CMaterial::Get_MaterialDataID(_uint Index)
{
	if (Index >= m_MaterialDatas.size()) return 0;

	return m_MaterialDatas[Index]->Get_MaterialDataID();
}

void CMaterial::Apply_Material(ID3D11DeviceContext* pContext, _uint Index)
{
	if (Index >= m_MaterialDatas.size()) return;
	m_MaterialDatas[Index]->ApplyData(pContext , m_TextureIndex);
}

HRESULT CMaterial::GetPassSignature(_uint Index, D3DX11_PASS_DESC* pOutPassDesc)
{
	if (Index >= m_MaterialDatas.size()) E_FAIL;

	return m_MaterialDatas[Index]->GetPassSignature(pOutPassDesc);
}

const string& CMaterial::GetPassConstant(_uint Index)
{
	return m_MaterialDatas[Index]->Get_PassConstant();
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
	for (auto& data : m_MaterialDatas)
		Safe_Release(data);
	m_MaterialDatas.clear();
}

void CMaterial::Render_GUI()
{

	if (m_MaterialDatas.empty())
		return;
	ImGui::SeparatorText("Material");
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (m_MaterialDatas.size() * 2) + (ImGui::GetStyle().WindowPadding.y * 4);

	if(ImGui::Button("Material Tabs")) {
		m_bMaterialTabOpen = true;
	}

	if(m_bMaterialTabOpen){
		ImGui::SetNextWindowSize(ImVec2(500, 400));
		if (ImGui::Begin("Materials", &m_bMaterialTabOpen, ImGuiWindowFlags_NoCollapse))
		{
		if (ImGui::BeginTabBar("##MaterialTabs"))
		{
			for (int i = 0; i < m_MaterialDatas.size(); ++i)
			{
				CMaterialData* pData = m_MaterialDatas[i];
				string tab_name = "Material " + to_string(i);
				if (ImGui::BeginTabItem(tab_name.c_str()))
				{
					pData->Render_GUI();
					ImGui::EndTabItem();
				}
			}
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
	}
}