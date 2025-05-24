#pragma once
#include "CObject.h"
class CMonster;
class CRader : public CObject
{
public:
	CRader(CMonster* owner);
	~CRader();
public:
	// CObject을(를) 통해 상속됨
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;
public:
	virtual void OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType);
	virtual void Set_Active(bool _active) override;
private:
	CMonster* m_pOwner;
	CObject* m_pTarget;
};

