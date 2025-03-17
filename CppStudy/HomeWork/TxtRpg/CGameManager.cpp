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
	//이미 객체가 만들어졌다면 만들어지지 못하게.
	if (sceneManager != nullptr) return;
	if (player != nullptr) return; 

	//동적할당(인스턴스 생성)
	sceneManager = new CSceneManager;
	player = new CPlayer;
	
	//이니셜라이즈
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
	//객체 내부 릴리즈 진행
	sceneManager->Release();
	player->Release();

	//메모리 반환
	SAFE_DELETE(sceneManager); //만들어진 객체 반환
	SAFE_DELETE(player);
}
