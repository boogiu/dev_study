#pragma once
class CObject;

class CGravity 
{
public:
	CGravity();
	~CGravity();
public:
	void Initialize();
	void Update();
	void Render();
	void Release();
public:
	float GetGravity() {return m_fGravity;};

private:
	void SetGravObj(vector<CObject*>* _objVec) { m_pGravObj = _objVec; };
	void GravityUpdate();

private:
	float m_fGravity;
	float m_fMaxGravity;

	vector<CObject*>* m_pGravObj;
};

