#pragma once
class CObject;
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

private:
	void CheckObjectOnGrounded(OBJ_TAG rightTag);
	void CheckCollisionRectToRect(OBJ_TAG rightTag, OBJ_TAG leftTag);
	void CheckCollisionLineToRect(OBJ_TAG rightTag, OBJ_TAG leftTag);
	void CheckCollisionCircleToRect(OBJ_TAG rightTag, OBJ_TAG leftTag);

	void RenderCollider(OBJ_TAG tag, HDC _hDC);
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

private:
	bool m_bRender;

	static CCollisionMgr* m_pInstance;
};

