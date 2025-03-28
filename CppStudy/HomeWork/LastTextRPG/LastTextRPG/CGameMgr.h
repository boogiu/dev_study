#pragma once
class CPlayer;
class CDataMgr;
class CSceneMgr;
class CKeyMgr;

class CGameMgr
{
private:
	CGameMgr();
	~CGameMgr();
public:
	CGameMgr(const CGameMgr& rhs) = delete;
	CGameMgr& operator = (const CGameMgr& rhs) = delete;

public:
	//SINGLETON
	static CGameMgr& GetInstance() {
		static CGameMgr instance;
		return instance;
	}

public:
	void Initialize();
	void Release();

	//update 1Frame
	void Update();
	//Render 1Frame
	void Render();

	//게임 종료 여부 true -> 종료
	bool isGameEnd() { return m_GameEnd; };
	void setGameEnd(bool _trigger) { m_GameEnd = _trigger; }

	//플레이어 포인터
	CPlayer* GetPlayer() { return m_player; }

private:
	bool m_GameEnd = false;
	
	CPlayer* m_player;
	CDataMgr* m_dataMgr;
};
