#include "CSceneManager.h"
#include "pch.h"

CSceneManager::CSceneManager()
	:sceneArr(nullptr)
{
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::Initialize()
{
	nowScene = SCENE::START;
	if (sceneArr != nullptr) return; // 한번만 실행되도록

	sceneArr = new CScene*[END]; // END 개수만큼 생성
	sceneArr[0] = new CStartScene;
	sceneArr[1] = new CSelectScene;

}

void CSceneManager::Render()
{
	system("cls");
	sceneArr[nowScene]->Render();
}

void CSceneManager::Update()
{

}

void CSceneManager::Release()
{
	if (sceneArr == nullptr) return; // 생성되었다면
	for (int i = 0; i < END; ++i) {
		SAFE_DELETE(sceneArr[i]);
	}
	delete[] sceneArr;
}
