#ifndef _GAMEMANAGER_
#define _GAMEMANAGER_

#include "CsceneManager.h"
#include "CPlayer.h"

class CGameManager
{
public:
	CGameManager(); 
	~CGameManager(); 

public:
	void Initialize();//필요 매니저 객체들의 생성
	void Update(); // 로직의 구동
	void Render(); // 출력의 구동
	void Release();// 매니저 객체들의 메모리 해제

private:
	//필요한 객체
	//1. SCENE 매니저
	CSceneManager* sceneManager;
	//2. Object : Player
	CPlayer* player;
	//3. 

};

#endif // !_GAMEMANAGER_


