#ifndef _SCENEMANAGER_
#define _SCENEMANAGER_
#include "CScene.h"
#include "CStartScene.h"
#include "CSelectScene.h"
#include "CVillageScene.h"

class CSceneManager
{

public: 
	CSceneManager();
	~CSceneManager();

public:
	void Initialize();
	void Render();
	void Update();
	void Release();

private:
	//필요 객체 : Scene들의 배열.
	CScene** sceneArr;  // 각 scene을 생성하고(포인터) -> 그 포인터들을 관리할 배열을 생성(포인터)

};

#endif // !_SCENEMANAGER_


