#pragma once
class CTile;

class CTileMgr
{

private:
	CTileMgr();
	~CTileMgr();
	CTileMgr(CTileMgr& rhs) = delete;
	CTileMgr& operator=(CTileMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	void	Save_Data();
	void	Load_Data();
	TILE Get_Tile() { return m_tTile; }

private:
	void Update_Type();
	bool PushTile(TILE _tile);
	void DrawOne(POINT& mouse);
	void DrawDrag(POINT& mouse);
public:
	static CTileMgr* Get_Instance() {
		if (!m_pTileMgr) {
			m_pTileMgr = new CTileMgr;
		}
		return m_pTileMgr;
	}
	static void Destroy_Instance() {
		if (m_pTileMgr) {
			delete m_pTileMgr;
			m_pTileMgr = nullptr;
		}
	}

private:
	POINT m_ptMouse{};
	
	TCHAR m_szBuffer[32];
	RECT m_bufferZone;

	static CTileMgr* m_pTileMgr;
	list<CTile*>	m_Tilelist;
	
	TILE m_tTile;
	CTile* cursorTile;
	TILE_ID m_eType;
	DIRECTION m_eDir;
	
	POINT prevPoint;
};

