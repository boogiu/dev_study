#include "StaticModel.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "IRenderService.h"
#include "Mesh.h"



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

HRESULT CStaticModel::Link_Buffer(const string& levelKey, const string& MeshKey)
{
    m_Buffers = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Mesh(levelKey, MeshKey);
    for (CMesh* mesh : m_Buffers) {
        m_DrawableMeshes.push_back(true);
        Safe_AddRef(mesh);
    }
    return S_OK;
}

HRESULT CStaticModel::Render_Mesh(ID3D11DeviceContext* pContext, _uint Index)
{
    if (Index >= m_Buffers.size()) return E_FAIL;
    m_Buffers[Index]->Bind_Buffer(pContext);
    m_Buffers[Index]->Render(pContext);
    return S_OK;
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
