#include "PlaneModel.h"
#include "VI_Rect.h"
#include "GameInstance.h"
#include "IResourceService.h"
CPlaneModel::CPlaneModel()
{
}

CPlaneModel::CPlaneModel(const CPlaneModel& rhs)
	:CModel(rhs), m_pPlane{ rhs.m_pPlane }
{
	Safe_AddRef(m_pPlane);
}

HRESULT CPlaneModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlaneModel::Initialize(COMPONENT_DESC* pArg)
{
	m_pPlane = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_VIBuffer(G_GlobalLevelKey, "Engine_Default_Plane", BUFFER_TYPE::BASIC_PLANE);
	return S_OK;
}

CPlaneModel* CPlaneModel::Create()
{
	CPlaneModel* instance = new CPlaneModel();

	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("CPlaneModel Create Failed : CPlaneModel");
		Safe_Release(instance);
	}

	return instance;
}

CComponent* CPlaneModel::Clone()
{
	CPlaneModel* instance = new CPlaneModel(*this);
	return instance;
}


const D3D11_INPUT_ELEMENT_DESC* CPlaneModel::Get_ElementDesc(_uint DrawIndex)
{
	return VTXPOSTEX::Elements;
}

const _uint CPlaneModel::Get_ElementCount(_uint DrawIndex)
{
	return VTXPOSTEX::iElementCount;
}

const string_view CPlaneModel::Get_ElementKey(_uint DrawIndex)
{
	return VTXPOSTEX::Key;
}

HRESULT CPlaneModel::Draw(ID3D11DeviceContext* pContext, _uint Index)
{
	m_pPlane->Bind_Buffer(pContext);
	m_pPlane->Render(pContext);
	return S_OK;
}

HRESULT CPlaneModel::Link_Model(const string& levelKey, const string& modelDataKey)
{
	m_pPlane = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_VIBuffer(levelKey, modelDataKey, BUFFER_TYPE::BASIC_PLANE);

	return S_OK;
}

_uint CPlaneModel::Get_MeshCount()
{
	return 1;
}

_uint CPlaneModel::Get_MaterialIndex(_uint Index)
{
	return 0;
}

_bool CPlaneModel::isDrawable(_uint Index)
{
	return true;
}

BOUNDING_BOX CPlaneModel::Get_LocalBoundingBox()
{
	return BOUNDING_BOX{ { -0.5f, 0.f, -0.5f, }, {0.5f,0.f ,0.5f} };
}

void CPlaneModel::Render_GUI()
{
}

void CPlaneModel::Free()
{
	__super::Free();
	Safe_Release(m_pPlane);
}
