#include "StaticModel.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "IRenderService.h"
#include "ModelData.h"



CStaticModel::CStaticModel()
{
}

CStaticModel::CStaticModel(const CStaticModel& rhs)
    :CModel(rhs)
{
}

HRESULT CStaticModel::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStaticModel::Initialize(COMPONENT_DESC* pArg)
{
    return S_OK;
}

HRESULT CStaticModel::Link_Model(const string& levelKey, const string& MeshKey)
{
    m_pData = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_ModelData(levelKey, MeshKey);
    m_DrawableMeshes.resize(m_pData->Get_MeshCount(), true);
    return S_OK;
}

HRESULT CStaticModel::Render_Model(ID3D11DeviceContext* pContext, _uint Index)
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
}
