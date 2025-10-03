#include "Sprite2D.h"

CSprite2D::CSprite2D()
{
}

CSprite2D::CSprite2D(const CSprite2D& rhs)
{
}

CSprite2D::~CSprite2D()
{
}

HRESULT CSprite2D::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CSprite2D::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}

void CSprite2D::Render_GUI()
{
}

CSprite2D* CSprite2D::Create()
{
	CSprite2D* instance = new CSprite2D();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
		MSG_BOX("Sprite2D Comp Failed To Create : CSprite2D");
	}
	return instance;
}

CComponent* CSprite2D::Clone()
{
	return new CSprite2D(*this);
}

void CSprite2D::Free()
{
	__super::Free();
}
