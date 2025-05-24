#pragma once
class CObject;
class CCollider;
class CAttackBox;
class CAttackLine;
class CRader;
class CCollisionMgr
{
private:
	CCollisionMgr();
	~CCollisionMgr();
	CCollisionMgr(CCollisionMgr& rhs) = delete;
	CCollisionMgr operator=(CCollisionMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	static CCollisionMgr* Get_Instance() {
		if (!m_pInstance) {
			m_pInstance = new CCollisionMgr;
		}
		return m_pInstance;
	}
	static void Destroy_Instance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Register_AtkBox(CAttackBox* _AtkBox);
	void Register_AtkLine(CAttackLine* _AtkBox);
	//void Register_AtkLine(CAttackBox* _AtkBox);
	POINT Check_SafetyPlatform(CRader* Rader, float safeDistanceX, float safeDistanceY);
	bool Trigger_With_Player(CObject* Trriger);

private:
	void CheckObjectOnGrounded(OBJ_TAG rightTag);
	void CheckObjectOnWall(OBJ_TAG rightTag);
	void CheckRectAtkCollision(OBJ_TAG tag);
	void CheckLineAtkCollision(OBJ_TAG tag);
	void RenderCollider(OBJ_TAG tag, HDC _hDC);
	void RenderAtkBox(OBJ_TAG tag, HDC _hDC);
	void RenderAtkLine(OBJ_TAG tag, HDC _hDC);

private:
	unordered_map<OBJ_TAG, vector<CAttackBox*>> m_AtkBoxContainer;
	unordered_map<OBJ_TAG, vector<CAttackLine*>> m_AtkLineContainer;
	//unordered_map<OBJ_TAG, vector<CAttackBox*>> m_AtkContainer;

	bool m_bRender;
	static CCollisionMgr* m_pInstance;
};

