#include "pch.h"
#include "CTestScene.h"
#include "CStage.h"
#include "CTestStage.h"
#include "CResourceMgr.h"

CTestScene::CTestScene()
{
}

CTestScene::~CTestScene()
{
}

void CTestScene::Initialize()
{

	m_NowStage = new CTestStage;
	m_NowStage->Initialize();
}

void CTestScene::Update()
{
	
}

void CTestScene::Late_Update()
{
}

void CTestScene::Render(HDC _hDC)
{
	m_NowStage->Render(_hDC);
}

void CTestScene::Release()
{
}
