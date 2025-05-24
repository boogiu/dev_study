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
	CTile* Get_nowTile() { return m_nowTile; }

private:
	void Key_Check();
	void Push_Tile();
	void Change_Tile(CTile* Dst, CTile* Src, bool Resize, bool Repos);
	void Erase_Tile();
	void Save_Tile();
	void Load_Tile();

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

	bool m_bGrid;
	bool m_bUI;

	POINT	ptMouse{};
	CTile* m_nowTile;
	vector<CTile*> m_tileUIContainer;
	vector<CTile*> m_tileDataContainer;

};
