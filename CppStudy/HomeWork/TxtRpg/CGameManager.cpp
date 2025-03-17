#include "CGameManager.h"
#include "pch.h"

CGameManager::CGameManager()
	:sceneManager(nullptr),player(nullptr){
}

CGameManager::~CGameManager()
{
}

void CGameManager::Initialize()
{
	if (sceneManager != nullptr) return; //이미 객체가 만들어졌다면 만들어지지 못하게.
	if (player != nullptr) return; //이미 객체가 만들어졌다면 만들어지지 못하게.
	sceneManager = new CSceneManager;
	player = new CPlayer;
	sceneManager->Initialize();
	player->Initialize();
}

void CGameManager::Update()
{
}

void CGameManager::Render()
{
	sceneManager->Render();
}

void CGameManager::Release()
{
	sceneManager->Release();
	player->Release();
	SAFE_DELETE(sceneManager); //만들어진 객체 반환
	SAFE_DELETE(player);
}
