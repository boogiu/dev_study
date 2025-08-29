#include "BasePanel.h"
#include "GameInstance.h"

CBasePanel::CBasePanel(SIZE vp)
	:m_pGameInstance(CGameInstance::GetInstance()), m_VPSize{vp}
{
	Safe_AddRef(m_pGameInstance);
}

void CBasePanel::Render_GUI()
{
}

void CBasePanel::Free()
{
	Safe_Release(m_pGameInstance);
}
