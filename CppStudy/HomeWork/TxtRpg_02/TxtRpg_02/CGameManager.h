#pragma once

class CObject;
class CPlayer;
class CScene;
class CDataBase;
class CGameManager
{
public : 
	CGameManager();
	~CGameManager();

public: 
	void Init();
	bool Render();
	void Release();

	static void SceneChange(E_SCENE _scene) { m_nowScene = _scene; };


private :
	static E_SCENE m_nowScene;
	CPlayer* m_pPlayer;
	CScene** m_SceneArr;
	CDataBase* m_DataBase;
};

