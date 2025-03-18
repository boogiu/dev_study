#include "CBaseScene.h"
#include "CGameMgr.h"
CBaseScene::CBaseScene()
{
}

CBaseScene::~CBaseScene()
{
}

void CBaseScene::Init()
{
}

void CBaseScene::Render()
{
}

void CBaseScene::Release()
{
}

void CBaseScene::ChangeScene(SCENE_ENUM _scene)
{
	CGameMgr::GetInstance()->SetScene(_scene);
}
