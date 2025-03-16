#include "pch.h"
#include "GamePlay.h"

GamePlay::GamePlay()
	: gameState(START),sceneManager(nullptr)
{
}

GamePlay::~GamePlay()
{
	Release();
}

void GamePlay::Initialize()
{
	sceneManager = new CSceneManager();
}

void GamePlay::Update()
{
}

void GamePlay::Release()
{
	//메모리 반환 등의 사항들 진행. 
	SAFE_DELETE(sceneManager);
}

void GamePlay::Render()
{
	sceneManager->Render(sceneManager->GetScene());
	system("cls");
	//렌더링 관련 사항들 진행
}
