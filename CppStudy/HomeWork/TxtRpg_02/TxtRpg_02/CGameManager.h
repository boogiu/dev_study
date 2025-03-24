#pragma once

class CObject;
class CPlayer;
class CScene;
class CDataBase;
class CItem;

class CGameManager
{
private : 
	CGameManager();
	~CGameManager();
	CGameManager(const CGameManager&) = delete;
	CGameManager& operator = (const CGameManager& rhs)=delete;

public :
	static CGameManager& GetInstance() {
		static CGameManager instance;
		return instance;
	}

public: 
	void Init();
	bool Render();
	void Release();
	void SceneChange(E_SCENE _scene) { m_nowScene = _scene; };
	CPlayer* GetPlayer();
	CObject* GetMonster(int _level);
	const std::vector<CItem*>& GetItems()const;

private :
	E_SCENE m_nowScene;
	CPlayer* m_pPlayer;
	CScene** m_SceneArr;
	CDataBase* m_DataBase;
};

