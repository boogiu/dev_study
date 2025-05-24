#pragma once
class CTile
{
public:
	CTile();
	virtual ~CTile();
public:
	virtual void Initialize() PURE;
	virtual void Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hDC)PURE;
	virtual void Release()PURE;

public: 
	TILE_INFO& Get_TILEINFO() { return  m_tInfo; };
	INFO_EX& Get_Info() { return m_tInfo.tInfo; };
	TILE_SPRITE& Get_Sprite() { return  m_tInfo.tSprite;};
	TILE_TYPE Get_TileType() { return m_tInfo.eType; }
	
	RECT Get_RECT() { RECT tmp; m_tInfo.tInfo.Update_INFO(tmp);return tmp; }
	void UI_Set();
	void Set_Stick(bool stick) { m_bStick = stick; }

protected:
	bool m_bStick;
	HDC memDC;
	TILE_INFO m_tInfo;
};

