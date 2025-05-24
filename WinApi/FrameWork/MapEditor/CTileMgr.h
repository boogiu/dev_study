#pragma once
class CTile;

class CTileMgr
{

private:
	CTileMgr();
	~CTileMgr();
	CTileMgr(CTileMgr& rhs) = delete;
	CTileMgr operator=(CTileMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();
private:
	void Key_Check();
	void Push_Tile();
	void Erase_Tile();
	void Save_Tile();
	void Load_Tile();
	void Render_TileType(HDC _hDC);
	void Render_UI_Tile(HDC _hDC);
	void Erase_All(TILE_TYPE type);
public:
	POINT Get_ScreenPT() { return ptScreen; }
	POINT Get_WorldPT() { return ptWorld; }

public:
	static CTileMgr* Get_Instance() {
		if (!m_pInstance) {
			m_pInstance = new CTileMgr;
		}
		return m_pInstance;
	}
	static void Destroy_Instance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CTileMgr* m_pInstance;

	wstring m_Filename;
	bool m_bGrid;
	bool m_bUI;

	int m_iPltType;

	POINT	ptWorld{};
	POINT	ptScreen{};

	TCHAR szPosBuffer[64];
	RECT BufferPosRect;

	TCHAR szTypeBuffer[32];
	RECT BufferTypeRect;

	TILE_TYPE m_enowType;
	unordered_map<TILE_TYPE, CTile*> m_pNowTile;

	unordered_map<TILE_TYPE, vector<CTile*>> m_TileDataContainer;
	unordered_map<TILE_TYPE, vector<CTile*>> m_TileContainer;
};
