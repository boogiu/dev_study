#pragma once
class CTile
{
public: 
	CTile();
	CTile(TILE _info);
	~CTile();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	const TILE& GetTile() { return m_tTile; }
	void SetRect(RECT _rc);
	void SetType(TILE_ID _ID);
	void SetDir(DIRECTION _dir);
private:
	TILE m_tTile;
};

