#pragma once
class CItem;
class CObject
{
public: 
	CObject();
	virtual ~CObject();

public : 
	virtual void Initialize();
	virtual void Render();
	virtual void Update();
	virtual void Release();
	virtual int GetAtkType();

public:
	tagStatus& GetInfo() { return m_Stat;};
	void GetDamage(CObject* obj);
	void GetDebuff(int _AtkType, int turn =3, int damage=2);
	bool CheckDead() { return 0 >= nowHp; }
	
	void Heal(int point);
	void StateRestore(STATE_KEY key);

	const wstring& GetName();
	wstring RenderState();

private:
	void CheckState();

protected:
	void StateRestore();

	int nowHp;
	int m_AtkType; // 공격 타입
	int m_State; //현재 상태
	wstring m_Sname;
	tagStatus m_Stat;
	unordered_map<STATE_KEY, vector<int>> m_stateContainer;
};

