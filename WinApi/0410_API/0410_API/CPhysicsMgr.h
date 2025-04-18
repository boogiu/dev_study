#pragma once
class CObject;
class CVelocity;
class CGravity;
class CCollision;
class CTile;

class CPhysicsMgr : 
	public Singleton<CPhysicsMgr>
{
	friend class Singleton;

private:
	CPhysicsMgr();
	~CPhysicsMgr();

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
public:
	void RegisterPhysics(CObject* _obj, OBJ_LAYER _layer, PHYSICS _physics);
	void RegisterPhysics(CTile* _tile, GRD_LAYER _layer, PHYSICS _physics);

	vector<CObject*>* GetPhysicsObject(PHYSICS _physics);
	vector<CTile*>* GetPhysicsTile(PHYSICS _physics);

	vector<vector<CObject*>>* GetCollisionObject();
	vector<vector<CTile*>>* GetCollisionGround();
private:
	void TryAdd(vector<CObject*>& container, CObject* obj);
	void TryAdd(vector<CTile*>& container, CTile* _tile);

private:
	CVelocity* m_Velocity;
	CGravity* m_Gravity;
	CCollision* m_Collision;

	vector<vector<CObject*>> m_vecColliderObj;
	vector<CObject*> m_vecGravityObj;
	vector<CObject*> m_vecVelocityObj;

	vector<vector<CTile*>> m_vecColliderGrd;
	vector<CTile*> m_vecGravityGrd;
	vector<CTile*> m_vecVelocityGrd;

};

