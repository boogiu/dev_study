#pragma once

class CObject;

class CObjMgr :
    public Singleton<CObjMgr>
{
	friend class Singleton;
private:
	CObjMgr();
	~CObjMgr();

public:
	void Initialize();
	void Update();
	void Render(HDC _hDC);
	void Release();

public:
	CObject* GetPlayer() { return m_pPlayer; }
	CObject* GetMonster(SCENE _scene);
	CObject* GetBullet(SCENE _scene);

	void ReleaseScene(SCENE _scene);
	vector<CObject*>& GetMonsterList(SCENE _scene);
	vector<CObject*>& GetBulletList(SCENE _scene);

private:
	CObject* m_pPlayer;

	unordered_map<SCENE, vector<CObject*>> m_mapMonster;
	unordered_map<SCENE, vector<CObject*>> m_mapBullet;
};

