#ifndef _GAMEMANAGER_
#define _GAMEMANAGER_

#include "CsceneManager.h"
#include "CPlayer.h"

class CGameManager
{
SINGLETON(CGameManager)

public:
	void Initialize();//필요 매니저 객체들의 생성
	void Update(); // 로직의 구동
	void Render(); // 출력의 구동
	void Release();// 매니저 객체들의 메모리 해제

	void SetGameOn(bool _bool) { m_isGameOn = _bool; }
	bool GetGameOn() { return m_isGameOn; }

private :
	bool m_isGameOn;
};

#endif // !_GAMEMANAGER_


