#include "Client_Defines.h"
#include "UI_EventMsg.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "ObjectContainer.h"
#include "UI_Text.h"
#include "TextSlot.h"
#include "UI_Cursor.h"

CUI_EventMsg::CUI_EventMsg()
{
}

CUI_EventMsg::CUI_EventMsg(const CUI_EventMsg& rhs)
	:CUI_Object(rhs)
{
}

HRESULT CUI_EventMsg::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUI_EventMsg::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Add_Component<CObjectContainer>();
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_DialogueBox.png");


	CUI_Object* pUI = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_BaseText" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 200,50 })
		.Position({ 0,0 })
		.Build("Text");

	Get_Component<CObjectContainer>()->Add_Child(pUI, false);
	
	m_pTexts = dynamic_cast<CUI_Text*>(pUI);
	m_pTexts->Set_Anchor(ANCHOR::Center);
	m_pTexts->Set_Scale(1.f);
	CUI_Object* pCursor = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Cursor" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 35,20 })
		.Position({ 0, 70})
		.Build("Dialcursor");

	m_pCursor = dynamic_cast<CUI_Cursor*>(pCursor);
	Get_Component<CObjectContainer>()->Add_Child(m_pCursor, false);
	m_pCursor->Get_Component<CSprite2D>()->ChangeSprite(3);
	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(false);
	m_pCursor->Set_Speed(15.f);
	m_bActive = false;
	return S_OK;
}

void CUI_EventMsg::Priority_Update(_float dt)
{
	if (!m_bActive) return;

	if (m_bOpenComplete = Size_To(XMLoadFloat2(&m_vOpenSize), m_fOpenSpeed * dt)) {
		m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(true);
		m_pTexts->Set_Active(true);
	}

	if (m_bSequenceComplete) {
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SPACE)) {
			SequenceClear();
			m_iSequence++;
			if (m_iSequence >= m_iSequenceSize)
				UI_DeActive(nullptr);
		}
	}

	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CUI_EventMsg::Update(_float dt)
{
	if (!m_bActive) return;

	if (m_bOpenComplete) {
		m_pCursor->Set_Pivot({ 0, 70 }, { 0.f,0.f }, { 0,.8f });
		m_fTypeTime += dt;

		if (m_fTypeTime > m_fPauseTime) {

		wstring Fulltext = sequence[m_iSequence];
		wstring visibleText;
		if (m_iPauseSubset < static_cast<_int>(Fulltext.size()))
		{
			m_iPauseSubset++;
			if (Fulltext.substr(m_iPauseSubset, 1) == L".")
			{
				m_fPauseTime = .25f;
				m_fTypeTime = 0.f;
			}
			else
				m_fPauseTime = .15f;
		}

		visibleText = Fulltext.substr(0, m_iPauseSubset);

		m_pTexts->Get_Component<CTextSlot>()->Set_Text(visibleText);

		/*해당 시퀀스 글자 출력 완료*/
		if (visibleText.size() == sequence[m_iSequence].size())
			m_bSequenceComplete = true;
		}
	}

	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CUI_EventMsg::Late_Update(_float dt)
{
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CUI_EventMsg::Render_GUI()
{
	_float2 CursorPos = {};
	ImGui::InputFloat2("CursorPos", reinterpret_cast<_float*>(&CursorPos));
	m_pCursor->Set_CenterPos(CursorPos);
}

void CUI_EventMsg::UI_Active(void* pArg)
{
	EventMsgDesc* desc = static_cast<EventMsgDesc*>(pArg);

	m_fOpenSpeed = desc->OpenSpeed;
	m_vOpenSize = desc->OpenSize;

	m_iSequenceSize = desc->textSequence.size();
	sequence = desc->textSequence;

	m_bActive = true;

	if (desc && desc->OnClose)
		m_onClose = desc->OnClose;
}

void CUI_EventMsg::UI_DeActive(void* pArg)
{
	m_onClose();
	SequenceClear();
	m_vOpenSize = { 0,0 };
	m_fSizeX = 0;
	m_fSizeY = 0;
	m_iSequenceSize = 0;
	vector<wstring> dummy;
	sequence.swap(dummy);
	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(false);
	m_bActive = false;
	m_onClose = nullptr;
}

void CUI_EventMsg::SequenceClear()
{
	m_bSequenceComplete = false;
	m_fTypeTime = 0.f;
	m_iPauseSubset = 0;
}

CUI_EventMsg* CUI_EventMsg::Create()
{
	CUI_EventMsg* instance = new CUI_EventMsg();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_EventMsg");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_EventMsg::Clone(INIT_DESC* pArg)
{
	CUI_EventMsg* instance = new CUI_EventMsg(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_EventMsg");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_EventMsg::Free()
{
	__super::Free();
}

