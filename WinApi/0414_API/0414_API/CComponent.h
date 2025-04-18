#pragma once
class CGameObject;

class CComponent
{
public:
	CComponent();
	~CComponent();

public:

	void SetOwner(CGameObject* owner) {
		m_pOwner = owner;
	}

public:
	virtual void Initialize() PURE;
	virtual void Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hDC)PURE;
	virtual void Release()PURE;

protected:
	CGameObject* m_pOwner;
};

