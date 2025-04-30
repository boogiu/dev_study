#pragma once
class CTile
{
public:
	CTile();
	CTile(INFO _INFO, int  Row, int  Col);
	CTile(TILE_INFO _info) : m_tTile(_info),m_bMove(true) {};
	~CTile();

	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();
	
public:
	bool Get_IsMove() { return m_bMove; }
	void Set_IsMove(bool move) { m_bMove = move; }
	void Set_Pos(POINT _pos) { m_tTile.m_tInfo.fX = _pos.x; m_tTile.m_tInfo.fY = _pos.y;}
	void Set_SCALE(POINT _pos) { m_tTile.m_tInfo.fCX = _pos.x; m_tTile.m_tInfo.fCY = _pos.y; }
	void Set_Col(int col) { m_tTile.m_Col = col; }
	void Set_Row(int row) { m_tTile.m_Row = row; }


	RECT GetRect() { return m_tTile.m_tSpriteRect; }
	TILE_INFO Get_TileINFO() const{ return m_tTile; }

private:
	void Update_Rect();

private:
	bool m_bMove;
	TILE_INFO m_tTile;
};

