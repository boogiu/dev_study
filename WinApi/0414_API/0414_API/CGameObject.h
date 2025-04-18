#pragma once
class CComponent;

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

public:
	// CObject을(를) 통해 상속됨
	virtual void Initialize()  PURE;
	virtual void Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hDC)PURE;
	virtual void Release()PURE;

public:
	void AddComponent(CComponent* component);
protected:
	void Update_Component();
	void Render_Component(HDC _hDC);

public:
public:
	template<typename T>
	T* GetComponent() {
		for (CComponent* component : m_vecComponent) {
			if (T* casted = dynamic_cast<T*>(component)) {
				return casted;
			}
		}
		return nullptr;
	}


protected:
	vector<CComponent*> m_vecComponent;
};

