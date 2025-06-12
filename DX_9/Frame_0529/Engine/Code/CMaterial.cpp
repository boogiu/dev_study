#include "Engine_Define.h"
#include "CMaterial.h"

CMaterial::CMaterial()
{
}

CMaterial::~CMaterial()
{
}

CMaterial* CMaterial::Create()
{
	CMaterial* instance = new CMaterial;

	if (FAILED(instance->Ready_Material())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CMaterial::Ready_Material()
{
	return S_OK;
}

void CMaterial::SetSize(int i)
{
	m_vecMaterial.reserve(i);
}

void CMaterial::Free()
{
	for (MATTEX& mat : m_vecMaterial) {
		if (mat.texture) {
			Safe_Release(mat.texture);
		}
	}

}
