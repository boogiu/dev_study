#include "HierarchyPanel.h"
#include "GUIWidget.h"
#include "GameInstance.h"
#include "ILevelService.h"
#include "IObjectService.h"
#include "Layer.h"
#include "GameObject.h"

CHierarchyPanel::CHierarchyPanel(SIZE vp)
    :CBasePanel(vp)
{
}

CHierarchyPanel::~CHierarchyPanel()
{
}

HRESULT CHierarchyPanel::Initialize()
{
    m_pObjectManager = m_pGameInstance->Get_ObjectMgr();
    if (!m_pObjectManager)
        return E_FAIL;

    Safe_AddRef(m_pObjectManager);
    return S_OK;
}

void CHierarchyPanel::Render_GUI()
{
    const string& nowLevel = m_pGameInstance->Get_LevelMgr()->Get_NowLevelKey();
    const auto& LayerMap = m_pObjectManager->Get_LevelLayer(nowLevel);

    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(200, m_VPSize.cy));
    ImGui::Begin("Hierachy",nullptr,ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoCollapse);
        
    ImGui::End();
}

CHierarchyPanel* CHierarchyPanel::Create(SIZE vp)
{
    CHierarchyPanel* instance = new CHierarchyPanel(vp);
    if (FAILED(instance->Initialize()))
    {
        Safe_Release(instance);
    }
    return instance;
}

void CHierarchyPanel::Free()
{
    __super::Free();
    Safe_Release(m_pObjectManager);
}
