#include "StaticModel.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "IRenderService.h"
#include "ModelData.h"
#include"GameObject.h"
#include "Transform.h"

CStaticModel::CStaticModel()
{
}

CStaticModel::CStaticModel(const CStaticModel& rhs)
    :CModel(rhs),	m_pData(rhs.m_pData),
    m_DrawableMeshes(rhs.m_DrawableMeshes)
{
    Safe_AddRef(m_pData);
}

HRESULT CStaticModel::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStaticModel::Initialize(COMPONENT_DESC* pArg)
{
    return S_OK;
}

HRESULT CStaticModel::Link_Model(const string& levelKey, const string& modelDataKey)
{
    if(m_pData)
        Safe_Release(m_pData);

    m_pData = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_ModelData(levelKey, modelDataKey);

    if (!m_pData)
        return E_FAIL;

    Safe_AddRef(m_pData);
    m_DrawableMeshes.resize(m_pData->Get_MeshCount(), true);
    return S_OK;
}

const D3D11_INPUT_ELEMENT_DESC* CStaticModel::Get_ElementDesc(_uint DrawIndex)
{
    return m_pData->Get_ElementDesc(DrawIndex);
}

const _uint CStaticModel::Get_ElementCount(_uint DrawIndex)
{
    return m_pData->Get_ElementCount(DrawIndex);
}

const string_view CStaticModel::Get_ElementKey(_uint DrawIndex)
{
    return m_pData->Get_ElementKey(DrawIndex);
}

_uint CStaticModel::Get_MeshCount()
{
    return m_pData->Get_MeshCount();
}

_uint CStaticModel::Get_MaterialIndex(_uint Index)
{
    return m_pData->Get_MaterialIndex(Index);
}

_bool CStaticModel::isDrawable(_uint Index)
{
    return m_DrawableMeshes[Index];
}

void CStaticModel::SetDrawable(_uint Index, _bool isDraw)
{
    if (Index >= m_DrawableMeshes.size())
        return;

    m_DrawableMeshes[Index] = isDraw;
}

BOUNDING_BOX CStaticModel::Get_LocalBoundingBox()
{
    return m_pData->Get_LocalBoundingBox();
}

BOUNDING_BOX CStaticModel::Get_WorldBoundingBox()
{
    BOUNDING_BOX wordlBox = m_pData->Get_LocalBoundingBox();
    _float4x4* pWorldMat = m_pOwner->Get_Component<CTransform>()->Get_WorldMatrix();
    XMStoreFloat3(&wordlBox.vMin, XMVector3TransformCoord(XMLoadFloat3(&wordlBox.vMin), XMLoadFloat4x4(pWorldMat)));
    XMStoreFloat3(&wordlBox.vMax, XMVector3TransformCoord(XMLoadFloat3(&wordlBox.vMax), XMLoadFloat4x4(pWorldMat)));
    return wordlBox;
}

vector<BOUNDING_BOX> CStaticModel::Get_MeshBoundingBox()
{
    vector<BOUNDING_BOX> boxes;

    for (size_t i = 0; i < m_pData->Get_MeshCount(); i++)
    {
        boxes.push_back(m_pData->Get_MeshBoundingBox(i));
    }
    return boxes;
}

HRESULT CStaticModel::Draw(ID3D11DeviceContext* pContext, _uint Index)
{
    return m_pData->Render_Mesh(pContext, Index);
}
void CStaticModel::Render_GUI()
{
    ImGui::SeparatorText("Animate Model");
    float childWidth = ImGui::GetContentRegionAvail().x;
    const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
    const float childHeight = (textLineHeight * (m_pData->Get_MeshCount() + 4)) + (ImGui::GetStyle().WindowPadding.y * 2);

    ImGui::BeginChild("##Animate ModelChild", ImVec2{ 0, childHeight }, true);
    m_pData->Render_GUI();
    ImGui::EndChild();
}

CStaticModel* CStaticModel::Create()
{
    CStaticModel* instance = new CStaticModel();

    if (FAILED(instance->Initialize_Prototype())) {
        MSG_BOX("CStaticModel Create Failed : CStaticModel");
        Safe_Release(instance);
    }

    return instance;
}

CComponent* CStaticModel::Clone()
{
    CStaticModel* instance = new CStaticModel(*this);
    return instance;
}

void CStaticModel::Free()
{
    __super::Free();
    Safe_Release(m_pData);
}
