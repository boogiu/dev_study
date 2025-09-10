#include "Model.h"
#include "GameInstance.h"
#include "VIBuffer.h"
#include "IResourceService.h"
#include "IRenderService.h"

CModel::CModel()
{
}

CModel::CModel(const CModel& rhs)
	: m_pBuffer(rhs.m_pBuffer)
	,m_iElementCount(rhs.m_iElementCount)
	,m_pElementDesc(rhs.m_pElementDesc)
{
	Safe_AddRef(m_pBuffer);
}

CModel::~CModel()
{
}

HRESULT CModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CModel::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}

HRESULT CModel::Link_Buffer(const string& levelKey, const string& bufferKey, BUFFER_TYPE eType)
{
	m_pBuffer = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_VIBuffer(levelKey, bufferKey,eType);
	
	if (!m_pBuffer)
		return E_FAIL;

	Safe_AddRef(m_pBuffer);
	return S_OK;
}

void CModel::Link_Element(const D3D11_INPUT_ELEMENT_DESC* pDesc, _uint elementCount)
{
	m_pElementDesc = pDesc;
	m_iElementCount = elementCount;
}

HRESULT CModel::Bind_Model(ID3D11DeviceContext* pContext)
{
	return m_pBuffer->Bind_Buffer(pContext);
}

HRESULT CModel::Render_Model(ID3D11DeviceContext* pContext)
{
	return m_pBuffer->Render(pContext);
}


void CModel::Render_GUI()
{
	ImGui::SeparatorText("Model");
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight *2) + (ImGui::GetStyle().WindowPadding.y * 4);
	ImGui::BeginChild("##ModelChild", ImVec2{ childWidth, childHeight }, true);

	ImGui::EndChild();
}

CModel* CModel::Create()
{
	CModel* instance = new CModel();
	if (FAILED(instance->Initialize_Prototype())) {
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CModel::Clone()
{
	CModel* instance = new CModel(*this);
	return instance;
}

void CModel::Free()
{
	__super::Free();
	Safe_Release(m_pBuffer);
}
