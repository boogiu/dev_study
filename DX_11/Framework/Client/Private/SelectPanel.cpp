#include "Client_Defines.h"
#include "Sprite2D.h"
#include "SelectPanel.h"

CSelectPanel::CSelectPanel()
{
}

CSelectPanel::CSelectPanel(const CSelectPanel& rhs)
{
}

CSelectPanel::~CSelectPanel()
{

}
HRESULT CSelectPanel::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CSelectPanel::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CSelectPanel::Priority_Update(_float dt)
{
}

void CSelectPanel::Update(_float dt)
{
}

void CSelectPanel::Late_Update(_float dt)
{
}

void CSelectPanel::Render_GUI()
{
}


CSelectPanel* CSelectPanel::Create()
{
	CSelectPanel* instance = new CSelectPanel();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CSelectPanel");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CSelectPanel::Clone(INIT_DESC* pArg)
{
	CSelectPanel* instance = new CSelectPanel(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CSelectPanel");
		Safe_Release(instance);
	}

	return instance;
}

void CSelectPanel::Free()
{
	__super::Free();
}

