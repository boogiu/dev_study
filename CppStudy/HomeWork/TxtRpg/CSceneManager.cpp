#include "CSceneManager.h"
#include "pch.h"

CSceneManager::CSceneManager()
	:sceneArr(nullptr),nowScene(SCENE::START)
{
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::Initialize()
{
	if (sceneArr != nullptr) return; // 한번만 실행되도록

	sceneArr = new CScene*[SCENE::ENDSCENE]; // END 개수만큼 생성
	
	//각 배열 안에 객체 포인터 생성
	sceneArr[0] = new CStartScene;
	sceneArr[1] = new CSelectScene;
	sceneArr[2] = new CVillageScene;
	sceneArr[3] = new CShopScene;
	sceneArr[4] = new CShopScene;//수정 필요
	sceneArr[5] = new CBattleScene;

	for (int i = 0; i < SCENE::ENDSCENE; ++i) {
		sceneArr[i]->Initialize();
	}

}

void CSceneManager::Render()
{
	sceneArr[nowScene]->Render();
	system("cls");
}

void CSceneManager::Update()
{
	sceneArr[nowScene]->Update();
}

void CSceneManager::Release()
{
	if (sceneArr == nullptr) return; // 생성되었다면
	for (int i = 0; i < SCENE::ENDSCENE; ++i) {
		sceneArr[i]->Release();
		SAFE_DELETE(sceneArr[i]);
	}
	delete[] sceneArr;
}

void CSceneManager::ChangerScene(SCENE _scene)
{
	nowScene = _scene;
}
