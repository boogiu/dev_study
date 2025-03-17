#ifndef _SCENEMANAGER_
#define _SCENEMANAGER_
#include "CScene.h"
#include "CStartScene.h"
#include "CSelectScene.h"
#include "CVillageScene.h"
#include "CShopScene.h"
#include "CBattleScene.h"

class CSceneManager
{
	SINGLETON(CSceneManager)
public:
	void Initialize();
	void Render();
	void Update();
	void Release();
	void ChangerScene(SCENE _scene);
private:
	//필요 객체 : Scene들의 배열.
	CScene** sceneArr;	 // 각 scene을 생성하고(포인터) -> 그 포인터들을 관리할 배열을 생성(포인터)
	SCENE nowScene;		 //현재 움직이고 있는 씬(enum)
};

#endif // !_SCENEMANAGER_


