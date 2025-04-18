#pragma once
class CObject;

class CVelocity
{
public: 
	CVelocity();
	~CVelocity();
public:
	void Initialize();
	void Update();
	void Render();
	void Relese();
public:
	float GetFriction() { return (1.f - m_fFriction); }
	float GetResistance() { return (1.f - m_fResistance); }

	void SetMovableObj(vector<CObject*>* _vector) {m_pMovableObj = _vector;}
private:
	float m_fFriction;
	float m_fResistance;
	vector<CObject*>* m_pMovableObj;
};

