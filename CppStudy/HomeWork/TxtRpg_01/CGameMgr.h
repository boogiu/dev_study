#ifndef __CGAMEMGR_H__
#define __CGAMEMGR_H__
#include "pch.h"

//전방 선언
class CBaseScene;
class CPlayer;
class CGameMgr
{
private: //생성자, 소멸자, 복사생성자, 대입 연산자 숨김(삭제)
	CGameMgr();
	~CGameMgr();
	CGameMgr(const CGameMgr&) = delete;
	CGameMgr& operator =(const CGameMgr&) = delete;

public:
	//싱글톤
	// 레퍼런스로 해도 되지만, 우선은 포인터에 익숙해지도록 포인터 형태로 받기
	static CGameMgr* GetInstance() { 		
		static CGameMgr instance;
		return &instance;
	}
public:
	//객체의 초기화 (생성자 아님)
	void Init();
	bool Render();
	void Release();

public : 
	CPlayer* GetPlayer() { return m_player; }
	SCENE_ENUM GetScene() { return m_nowScene; }
	void SetScene(SCENE_ENUM _scene) {  m_nowScene = _scene; }

private:
	bool GameOn;
	//씬의 배열.
	CBaseScene**m_SceneArr;
	//현재 보여지는 씬의 분류
	SCENE_ENUM m_nowScene;

	//플레이어 데이터
	CPlayer* m_player;

};
#endif // !__CGAMEMGR_H__


