#pragma once
class CObject;
class CTile;

class CCollision
{
public:
	static bool OnCollision(RECT target1, RECT target2);

public:
	CCollision();
	~CCollision();
public:
	void Initialize();
	void Update();
	void Render();
	void Release();

private:
	void Collision_Object(OBJ_LAYER _Main, OBJ_LAYER _Sub);
	void Collision_Ground(OBJ_LAYER _Main, GRD_LAYER _Sub);

private:
	vector<vector<CObject*>>* m_pCollisionObj;
	vector<vector<CTile*>>* m_pCollisionGrd;
};

