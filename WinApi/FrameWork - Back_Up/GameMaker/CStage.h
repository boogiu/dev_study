#pragma once
class CObject;
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

protected:
	void Load_Map(const TCHAR* pFilePath);


protected:
	vector<CObject*> m_TileList;
	//vector<vector<CColider*>> m_TileCollider;

};

