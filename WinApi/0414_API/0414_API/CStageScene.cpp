#include "pch.h"
#include "CStageScene.h"
#include "CPlayer.h"
#include "CGameObject.h"
#include "CObjectFactory.h"


CStageScene::CStageScene()
{
}

CStageScene::~CStageScene()
{
}

void CStageScene::Initialize()
{
	m_pPlayer = CObjectFactory<CPlayer>::CreateObject();
	m_pPlayer->Initialize();

}

void CStageScene::Update()
{
	m_pPlayer->Update();
}

void CStageScene::Late_Update()
{
}

void CStageScene::Render(HDC _hDC)
{
	m_pPlayer->Render(_hDC);
}

void CStageScene::Release()
{
}
