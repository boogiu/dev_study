#pragma once
#include "CObject.h"
class CPlayerState;
class CSkul;
class CAnim;

class CPlayer :
	public CObject
{
public:
	CPlayer();
	virtual ~CPlayer() override;

	enum STATE { IDLE, MOVE, DASH, JUMP, FALL, ATTACKA, ATTACKB, JUMPATTACK, SKILLA, SKILLB, SWITCH, NONE };

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override;

public:
	void OnCollisionEnter(CObject* object, RECT collRECT) override;

public:
	void ChangeState(STATE newState);
	STATE GetState() { return m_nowState; }

	void ChangeSkul(CSkul* m_pCurSkul);

	bool isOnGround();

	void SetMaxJump(int max) { iMax_JumpCount = max; };
	bool CanJump() { return (iNow_JumpCount > 0); };
	void RestoreJump() { iNow_JumpCount = iMax_JumpCount; };
	void Add_JumpCount(int i) { iNow_JumpCount += i; }

	void SetMaxDash(int max) { iMax_DashCount = max; };
	bool CanDash() { return (iNow_DashCount > 0); };
	void RestoreDash() { iNow_DashCount = iMax_DashCount; };
	void Add_DashCount(int i) { iNow_DashCount += i; }

	void SetAnim(const TCHAR* state);
	bool isAnimStart();
	bool isAnimEnd();

	bool CanJumpAtk() { return m_bJumpAtk; }
	void set_JumpAtk(bool atk) { m_bJumpAtk = atk; }
	void DirectionFlip();

	bool CanSwitch() { return true; }
	void SwapSkul();

	CSkul* Get_Skul() { return m_pNowSkul; }
	CAnim* Get_Anim() { return m_pNowAnim; }

private:

	bool m_bJumpAtk;

	int iMax_JumpCount;
	int iNow_JumpCount;

	int iMax_DashCount;
	int iNow_DashCount;

	float DashCoolTime;
	float DashElapsedTime;

	STATE m_nowState;
	unordered_map<STATE, CPlayerState*> m_StateContainer;

	CSkul* m_pNowSkul;
	CSkul* m_pSubSkul;

	CAnim* m_pNowAnim;

	float m_correctX;
};
