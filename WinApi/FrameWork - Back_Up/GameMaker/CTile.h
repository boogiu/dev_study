#pragma once
#include "CObject.h"

class CTile : public CObject
{
public: 
	CTile();
	CTile(TILE_INFO_LOAD _info);
	~CTile();

public: 
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	void TileSet(TILE_INFO_LOAD _info);

	TILE_ROLE Get_Role() { return m_eRole; }
private:
	TILE_INFO m_tTile;
	TILE_ROLE m_eRole;  
};

