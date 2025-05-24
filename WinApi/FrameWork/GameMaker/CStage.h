#pragma once
class CObject;
class CInfiniteObj;
class CScene;

class CStage
{
public: 
	CStage();
	virtual ~CStage();

public:
	virtual void Initialize()PURE;
	virtual void Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hDC)PURE;
	virtual void Release() PURE;

public:
	void Set_OwnScene(CScene* ownScene) { m_pOwnScene = ownScene; }
protected:
	void Load_Map(const TCHAR* pFilePath);
	void Render_BackObj(HDC _hDC);
	void Update_BackObj();
	void Set_Player_Pos();
	CObject* Set_ObjType(TILE_INFO tile);
protected:
	unordered_map<TILE_TYPE, vector<CObject*>> m_StageTile;
protected:
	HDC memDC;
	CScene* m_pOwnScene;

	vector<CInfiniteObj*> m_BackStage;
	vector<CInfiniteObj*> m_BackCutton;
	vector<CInfiniteObj*> m_BackBridge;

	CObject* m_StartPoint;
	CObject* m_StageDoor;
	CObject* m_RewardBox;
	vector<CObject*> m_SpawnContainer;
};

