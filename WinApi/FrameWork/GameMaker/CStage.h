#pragma once
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
	void Render_Tile(HDC _hDC);

protected:
	list<TILE_INFO> m_TileList;
};

