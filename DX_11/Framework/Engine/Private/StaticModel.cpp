#include "StaticModel.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "IRenderService.h"
#include "ModelData.h"



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
    m_pData = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_ModelData(levelKey, modelDataKey);
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

BOUNDING_BOX CStaticModel::Get_LocalBoundingBox()
{
    return m_pData->Get_LocalBoundingBox();
}

HRESULT CStaticModel::Draw(ID3D11DeviceContext* pContext, _uint Index)
{
    return m_pData->Render_Mesh(pContext, Index);
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
