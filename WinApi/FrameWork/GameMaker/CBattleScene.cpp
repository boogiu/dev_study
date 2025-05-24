#include "pch.h"
#include "CBattleScene.h"
#include "Managers.h"
#include "Stages.h"

CBattleScene::CBattleScene()
:m_iStageNumber(0)
{
}

CBattleScene::~CBattleScene()
{
	Release();
}

void CBattleScene::Initialize()
{
	CUIMgr::Get_Instance()->UI_Set(true);
	CObjMgr::Get_Instance()->Awake_Pool(true);
	CCamera::Get_Instance()->Set_Target(CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER));
	m_iStageNumber = 0;

	CStage* pStage = new CTutorialStage;
	m_StageVector.push_back(pStage);
	pStage->Initialize();

	
	 pStage = new CFirstStage;
	m_StageVector.push_back(pStage);
	
	pStage = new CBossStage;
	m_StageVector.push_back(pStage);
	
	/*CStage* pStage = new CBossStage;
	m_StageVector.push_back(pStage);
	pStage->Initialize();*/

	for (CStage* stage : m_StageVector) {
		stage->Set_OwnScene(this);
	}
}

void CBattleScene::Update()
{
	m_StageVector[m_iStageNumber]->Update();
}

void CBattleScene::Late_Update()
{
	m_StageVector[m_iStageNumber]->Late_Update();

	if (m_iStageNumber >= m_StageVector.size()) {
		CSceneMgr::Get_Instance()->Change_Scene(END);
	}
}

void CBattleScene::Render(HDC _hDC)
{
	m_StageVector[m_iStageNumber]->Render(_hDC);
}

void CBattleScene::Release()
{
	for (CStage* stage : m_StageVector) {
		Safe_Delete<CStage*>(stage);
	}
	m_StageVector.clear();
}

void CBattleScene::Change_Stage()
{
	CObjMgr::Get_Instance()->ClearObject();
	m_iStageNumber += 1;
	m_StageVector[m_iStageNumber]->Initialize();
}


void CBattleScene::CloseScene()
{
}

