
#pragma once
class CObject;

class CAttackLine
{
public:
	CAttackLine(CObject* owner);
	~CAttackLine();

public:
	void Render(HDC _hDC);
	void Update();
	void Late_Update();

public:
	bool isActive();
	void Set_Active();
	void Set_DeActive();
	bool Can_Hit(CObject* obj);
	OBJ_TAG Get_Target();
	void Set_Target(OBJ_TAG tag);
	void OnCollisionEnter(CObject* target, RECT rc,HIT_TYPE _type);
	CObject* Get_Owner();
	void Clear_Hitted();
	LINE_INFO& Get_LineInfo() { return m_tLineInfo; };
	bool InterSectLine(RECT rc);
	void Set_Line(LINE_INFO line) { m_tLineInfo.Set_Line(line); }
private:
	bool m_bActive;
	OBJ_TAG m_atkTarget;
	LINE_INFO m_tLineInfo;
	vector<CObject*> m_HittedContainer;
	CObject* m_pOwner;
};
