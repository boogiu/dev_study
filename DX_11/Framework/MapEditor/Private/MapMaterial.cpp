#include "Editor_Defines.h"
#include "MapMaterial.h"

CMapMaterial::CMapMaterial()
{
}

CMapMaterial::CMapMaterial(const CMapMaterial& rhs)
    : CMaterial(rhs)
{
}

HRESULT CMapMaterial::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMapMaterial::Initialize(COMPONENT_DESC* pArg)
{
    return S_OK;
}

void CMapMaterial::Render_GUI()
{
}

CMapMaterial* CMapMaterial::Create()
{
	CMapMaterial* instance = new CMapMaterial;

	if (FAILED(instance->Initialize_Prototype())) {
		Safe_Release(instance);
	}

	return instance;
}

CComponent* CMapMaterial::Clone()
{
	CMapMaterial* instance = new CMapMaterial(*this);
	return instance;
}


void CMapMaterial::Free()
{
	__super::Free();
}
