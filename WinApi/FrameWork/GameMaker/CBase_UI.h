#pragma once
class CBase_UI
{
public:
	CBase_UI();
	virtual ~CBase_UI();
public:
	enum class AL_DIR	{		LEFT,		RIGHT,		UP,		DOWN};

	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hDC)PURE;
	virtual void Release()PURE;
public:
	void Set_Info(UI_INFO _info) { m_tInfo = _info; };
	void Set_Scale(float xSize, float ySize);
	void AlignToPointX(float x, AL_DIR _dir);
	void AlignToPointY(float y, AL_DIR _dir);
	void Make_GapX(float fromX, float distance, AL_DIR move_dir);
	void Make_GapY(float fromY, float distance, AL_DIR move_dir);

public:
	bool Get_Actvie() { return m_bActive; }
	void Set_Active(bool active) {m_bActive = active;}

protected:
	bool m_bActive;
	HDC memDC;
	RECT m_tRect;
	UI_INFO m_tInfo;
};

