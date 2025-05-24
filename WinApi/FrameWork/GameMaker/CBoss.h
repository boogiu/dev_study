#pragma once
#include "CMonster.h"

class CAnim;
class CBossState;

class CBoss :
    public CMonster
{
public:
	CBoss();
	~CBoss();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

public:
	void ChangeState(BOSS_STATE state);
	BOSS_STATE Get_State() { return m_nowState; }

	void SetAnim(wstring animKey);
	void Set_BossType(int i);

	wstring Get_Name() { return m_BossName; }

	void Set_Name(const wstring name) { m_BossName = name; }
	CAnim* Get_Anim() { return m_pAnim; }

	void Reset_Scale() { m_tInfo.Set_Size(150.f, 150.f); }
	void Set_WaitPoint(POINT _pt) { m_WaitPoint = _pt; }
	POINT Get_WaitPoint() { return m_WaitPoint; }

	bool Get_IDLE_END() { return m_bEndIdle; };
	void  Set_IDLE_END(bool end) { m_bEndIdle = end; };

	bool Get_Waiting() { return m_bWaiting; };
	void  Set_Waiting(bool wait) { m_bWaiting = wait; };

	CObject* Get_Target() { return m_pTarget; };
	void  Set_Target(CObject* _target) { m_pTarget = _target; };

	void Change_anim_path(wstring newPath);
	void OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType) override;
	void Set_Render(bool render) { m_bRender = render; }
	float ReturnHpRatio() const {
		return (m_MaxHp > 0) ? static_cast<float>(m_NowHp) / m_MaxHp : 0.f;
	}

	void RestoreHealth() { m_NowHp = m_MaxHp; }
	bool Get_Dead() { return m_bDead; }
	bool IsInState(BOSS_STATE state) { return m_nowState == state; }
private:
	void Get_Damage(float Dmg);

private:
	bool m_bEndIdle;
	bool m_bWaiting;
	bool m_bRender;
	bool m_bDead;
	float m_MaxHp;
	float m_NowHp;

	CObject* m_pTarget;

	CAnim* m_pAnim;
	BOSS_STATE m_nowState;
	unordered_map<BOSS_STATE, CBossState*> m_StateContainer;
	wstring m_BossName;
	POINT m_WaitPoint;

	// CMonster을(를) 통해 상속됨
	void Find_Target(CObject* target) override;
	void Not_Find_Target() override;
};

