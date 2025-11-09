#include "Client_Defines.h"
#include "UI_Responcer.h"
#include "GameInstance.h"
#include "UI_Object.h"
CUI_Responcer::CUI_Responcer()
{
}

CUI_Responcer::CUI_Responcer(const CUI_Responcer& rhs)
	:CLevelObject(rhs)
{
}

HRESULT CUI_Responcer::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	CUI_Object* pEventMsg= Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_EventMsg" })
		.Add_To_Level("GamePlay_Level")
		.Position({ 600,620.f })
		.Build("EvtMsg");	
	
	CUI_Object* pTalkingMsg= Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_TalkingMsg" })
		.Add_To_Level("GamePlay_Level")
		.Position({ 600,620.f })
		.Build("EvtMsg");

	CGameInstance::GetInstance()->Get_UIMgr()->Add_UIObject(pEventMsg, "GamePlay_Level");
	m_UIContainer.emplace("EvtMsg", pEventMsg);
	
	CGameInstance::GetInstance()->Get_UIMgr()->Add_UIObject(pTalkingMsg, "GamePlay_Level");
	m_UIContainer.emplace("TalkingMsg", pTalkingMsg);
	return S_OK;
}

HRESULT CUI_Responcer::Initialize(INIT_DESC* pArg)
{
	__super::Initialize();
	return S_OK;
}

void CUI_Responcer::Priority_Update(_float dt)
{
}

void CUI_Responcer::Update(_float dt)
{
}

void CUI_Responcer::Late_Update(_float dt)
{
}

void CUI_Responcer::Active_UI(string uiName, void* pArg)
{
	auto iter = m_UIContainer.find(uiName);
	if (iter == m_UIContainer.end())
		return;

	else
		iter->second->UI_Active(pArg);
}

void CUI_Responcer::DeActive_UI(string uiName, void* pArg)
{
	auto iter = m_UIContainer.find(uiName);
	if (iter == m_UIContainer.end())
		return;

	else
		iter->second->UI_DeActive(pArg);
}

void CUI_Responcer::Render_GUI()
{
	for (auto ui : m_UIContainer)
	{
		ui.second->Render_GUI();
	}
}

CUI_Responcer* CUI_Responcer::Create()
{
	CUI_Responcer* instance = new CUI_Responcer();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CUI_Responcer");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CUI_Responcer::Clone(INIT_DESC * pArg)
{
	CUI_Responcer* instance = new CUI_Responcer(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_Responcer");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_Responcer::Free()
{
	__super::Free();
}

