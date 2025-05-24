#pragma once
class CObject;

class CAttackBox
{
public:
	CAttackBox(CObject* owner);
	~CAttackBox();
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
	COLLIDER_INFO& Get_Info();
	void OnCollisionEnter(CObject* target, RECT rc,HIT_TYPE _type);
	CObject* Get_Owner();
	void Clear_Hitted();
	COLL_TYPE Get_ColType() { return m_eCollTyp; }
	void Set_ColType(COLL_TYPE collType) { m_eCollTyp = collType; }
	void Set_Counter(int i) { maxCount = i; }
private:
	void Update_Rect();

private:
	bool m_bActive;
	int maxCount;
	CObject* m_pOwner;
	OBJ_TAG m_atkTarget;
	COLLIDER_INFO m_tInfo;
	RECT m_tRect;
	vector<CObject*> m_HittedContainer; 
	COLL_TYPE m_eCollTyp;
	
};

