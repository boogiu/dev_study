#include "Client_Defines.h"
#include "UI_TalkingMsg.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "ObjectContainer.h"

#include "UI_Text.h"
#include "TextSlot.h"
#include "UI_Cursor.h"
#include "UI_NameTag.h"
#include "SelectPanel.h"

#include "Level.h"
#include "NpcSpawner.h"
#include "NonPlayer.h"

CUI_TalkingMsg::CUI_TalkingMsg()
{
}

CUI_TalkingMsg::CUI_TalkingMsg(const CUI_TalkingMsg& rhs)
	:CUI_Object(rhs)
{
}

HRESULT CUI_TalkingMsg::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUI_TalkingMsg::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	Add_Component<CObjectContainer>();

	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_DialogueBox.png");

	Ready_Parts();


	auto Level = CGameInstance::GetInstance()->Get_CurrentLevel();
	m_pNpcSpawner = Level->Get_LevelObject<CNpcSpawner>();

	m_bActive = false;

	return S_OK;
}

void CUI_TalkingMsg::Priority_Update(_float dt)
{
	if (!m_bActive) return;

	m_bOpenComplete = Size_To(XMLoadFloat2(&m_vOpenSize), m_fOpenSpeed * dt);
	if (!m_bOpenComplete)
		return;

	/*시퀀스 하나가 끝났다면.*/
	if (m_bSeqComplete) {
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SPACE)) {
			m_nowSeqIndex++;

			if (m_nowSeqIndex >= m_Sequences.size()) {
				m_nowSeqIndex = m_Sequences.size() - 1; //마지막으로 유지 시키고,
				UI_DeActive(nullptr);

				//
				//if (m_Sequences[m_nowSeqIndex].postAction.Type.empty()) {
				//	UI_DeActive(nullptr); //직접 종료 해줌.
				//}
				//else {
				//}
			}
			else {
				m_iTypeSubStr = 0;
			}

			/*모션 호출 초기화*/
			m_bMotionCalled = false;
			m_bSeqComplete = false;
		}
	}

	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CUI_TalkingMsg::Update(_float dt)
{
	if (!m_bActive) return;
	if (!m_bOpenComplete)
		return;

	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(true);
	m_pTexts->Set_Active(true);
	m_pNameTag->Set_Active(true);
	m_pNameTag->Set_Name(m_SpeakerName);
	m_pCursor->Set_Pivot({ 0, 70 }, { 0.f,0.f }, { 0,.8f });
	m_fTypingTime += dt;
	m_fElapseTime += dt;

	DoSeqMotions();

	if (m_fTypingTime > m_fTypePuaseTime) { //타이핑 시간 충분히 지나면
		wstring Fulltext = m_Sequences[m_nowSeqIndex].Text; //이게 텍스트
		wstring visibleText; //타이핑 애니메이션 할 텍스트

		if (m_iTypeSubStr < static_cast<_int>(Fulltext.size())) {
			//아직 잘라낸 숫자가(타이핑이 된 숫자가) 적은 동안에는 계속 타이핑 진행
			m_iTypeSubStr++; //타이핑 시간 지났으니 다음 것도 출력

			if (Fulltext.substr(m_iTypeSubStr, 1) == L".")
				m_fTypePuaseTime = .25f;
			else
				m_fTypePuaseTime = .05f;

		}
		m_fTypingTime = 0.f;


		visibleText = Fulltext.substr(0, m_iTypeSubStr);
		m_pTexts->Get_Component<CTextSlot>()->Set_Text(visibleText);

		//모든 글자가 출력되었고, 내가 정한 퍼즈타임이 지났고, 모션들이 진행되었고
		if (visibleText.size() == Fulltext.size() && m_fElapseTime > m_Sequences[m_nowSeqIndex].pauseTime && m_bMotionCalled)
		{
			m_bHasSelection = !(m_Sequences[m_nowSeqIndex].choiceSelection.empty());

			// 선택지가 없다면
			if (!m_bHasSelection) {
				m_bSeqComplete = true;
			}

			// 선택지가 있다면
			else {
				Active_Select();
				_int selectedAction = m_pSelectPanel->Check_Select();
				if (selectedAction != -1) {
					ReArrange_Sequence(selectedAction);
				}
			}
		}
	}

	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CUI_TalkingMsg::Late_Update(_float dt)
{
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CUI_TalkingMsg::Render_GUI()
{
}

void CUI_TalkingMsg::UI_Active(void* pArg)
{
	TalkingMsgDesc* desc = static_cast<TalkingMsgDesc*>(pArg);

	m_fOpenSpeed = desc->OpenSpeed;
	m_vOpenSize = desc->OpenSize;

	m_pSpeaker = desc->Speaker;

	m_SpeakerID = desc->SpeakerID;
	m_Sequences = m_pNpcSpawner->Get_SequenceData(desc->SpeakerID, desc->startSequence);
	m_nowSeqIndex = 0;
	m_bActive = true;
	m_SpeakerName = desc->Speaker->Get_NpcData().NpcName;

	if (desc && desc->OnClose)
		m_onClose = desc->OnClose;

	m_bSeqComplete = false;
}

void CUI_TalkingMsg::UI_DeActive(void* pArg)
{
	m_bActive = false;
	POST_ACTION_DATA_DESC NextSeq = m_Sequences[m_nowSeqIndex].postAction;

	m_vOpenSize = { 0,0 };
	m_fSizeX = 0;
	m_fSizeY = 0;

	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(false);

	m_pTexts->Set_Active(false);
	m_pTexts->Clear_Text();

	m_pNameTag->Set_Active(false);
	m_pSelectPanel->DeActive();
	m_fElapseTime = 0.f;
	m_iTypeSubStr = 0;
	m_fTypingTime = 0;

	if (m_onClose)
		m_onClose(NextSeq.Type.empty());

	if (NextSeq.Type.empty() == false)
		m_pSpeaker->Do_PostAction(NextSeq);

	m_onClose = nullptr;
	SequenceClear();
}

void CUI_TalkingMsg::SequenceClear()
{
	m_nowSeqIndex = -1;
	m_bSeqComplete = { false };
	m_bMotionCalled = { false };
	m_bHasSelection = { false };
	m_bSelectionActive = { false };
	m_SpeakerName = {};

	m_SpeakerID = { -1 };
	m_pSpeaker = { nullptr };
	m_pListner = { nullptr };
}

void CUI_TalkingMsg::Ready_Parts()
{
	CUI_Object* pNameTag = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_NameTag" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 100,40 })
		.Position({ -320, -70 })
		.Rotate(XMConvertToRadians(8))
		.Build("NameTag");

	m_pNameTag = dynamic_cast<CUI_NameTag*>(pNameTag);
	m_pNameTag->Set_Active(false);
	Get_Component<CObjectContainer>()->Add_Child(pNameTag, false);

	/*텍스트*/
	CUI_Object* pText = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_BaseText" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 200,50 })
		.Position({ 0,0 })
		.Build("Text");

	Get_Component<CObjectContainer>()->Add_Child(pText, false);
	m_pTexts = dynamic_cast<CUI_Text*>(pText);
	m_pTexts->Set_Anchor(ANCHOR::Center);
	m_pTexts->Set_Scale(1.f);

	/*커서*/
	CUI_Object* pCursor = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Cursor" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 35,20 })
		.Position({ 0, 70 })
		.Build("Dialcursor");

	m_pCursor = dynamic_cast<CUI_Cursor*>(pCursor);
	Get_Component<CObjectContainer>()->Add_Child(m_pCursor, false);
	m_pCursor->Get_Component<CSprite2D>()->ChangeSprite(3);
	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(false);
	m_pCursor->Set_Speed(15.f);

	CUI_Object* pSelectUI = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_SelectPanel" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 0,0 })
		.Position({ 300,-100 })
		.Build("Select");

	Get_Component<CObjectContainer>()->Add_Child(pSelectUI, false);
	m_pSelectPanel = dynamic_cast<CSelectPanel*>(pSelectUI);
}

void CUI_TalkingMsg::DoSeqMotions()
{
	if (m_bMotionCalled) return;

	/*화자의 모션을 제어 => string으로 혹은 스테이트  변경으로?*/
	/*ㄴㄴ 단순 애니메이션 제어 형식이 깔끔*/
	m_pSpeaker->Set_Animation(m_Sequences[m_nowSeqIndex].Motion);
	m_pSpeaker->Set_Voice(m_Sequences[m_nowSeqIndex].Voice);
	m_pSpeaker->Set_Emotion(m_Sequences[m_nowSeqIndex].Emotion);

	m_bMotionCalled = true;
}

void CUI_TalkingMsg::DoPostAction()
{
}

void CUI_TalkingMsg::Active_Select()
{
	if (m_bSelectionActive) return;

	vector<wstring> choiceData;
	for (auto& choice : m_Sequences[m_nowSeqIndex].choiceSelection)
	{
		choiceData.push_back(choice.Text);
	}
	m_pSelectPanel->Get_Component<CSprite2D>()->Set_CompActive(true);
	m_pSelectPanel->Set_Selecte(choiceData, 0.f);
	m_pSelectPanel->Active();
	m_bSelectionActive = true;
}

void CUI_TalkingMsg::ReArrange_Sequence(_int selectChoice)
{
	_int NextSeq = m_Sequences[m_nowSeqIndex].choiceSelection[selectChoice].NextSequenceID;
	/*시퀀스 우선 종료 시키고*/

	m_Sequences = m_pNpcSpawner->Get_SequenceData(m_SpeakerID, NextSeq);
	m_nowSeqIndex = 0;
	m_bActive = true;
	m_bSelectionActive = false;
	m_pSelectPanel->DeActive();
	m_pSelectPanel->Get_Component<CSprite2D>()->Set_CompActive(false);
	m_iTypeSubStr =  0;
}

CUI_TalkingMsg* CUI_TalkingMsg::Create()
{
	CUI_TalkingMsg* instance = new CUI_TalkingMsg();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_TalkingMsg");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_TalkingMsg::Clone(INIT_DESC* pArg)
{
	CUI_TalkingMsg* instance = new CUI_TalkingMsg(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_TalkingMsg");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_TalkingMsg::Free()
{
	__super::Free();
}

