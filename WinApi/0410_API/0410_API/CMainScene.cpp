#include "pch.h"
#include "CMainScene.h"
#include "Managers.h"
#include "CButton.h"
#include "Physics.h"

CMainScene::CMainScene()
	:m_StartButton(nullptr), m_clicked(false)
{
}

CMainScene::~CMainScene()
{
	Release();
}

void CMainScene::Initialize()
{
	m_StartButton = new CButton;
	m_StartButton->Initialize(L"시작 버튼");

}

void CMainScene::Update()
{
	m_StartButton->Update();

	if (CInputMgr::GetInstance().KeyAway(VK_LBUTTON)) {
		if (CCollision::OnCollision(m_StartButton->GetRect(), mouseRange)) {
			m_clicked = !m_clicked;
		}

		if (m_clicked) {
			CSceneMgr::GetInstance().ChangeScene(SCENE::STAGE);
		}
	}
}

void CMainScene::Render(HDC _hDC)
{
	m_StartButton->Render(_hDC);
}

void CMainScene::Release()
{
	Safe_Delete<CButton*>(m_StartButton);
}
