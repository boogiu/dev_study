#pragma once
#include "Objects.h"

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();
	CObjMgr(CObjMgr& rhs) = delete;
	CObjMgr operator=(CObjMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	static CObjMgr* Get_Instance() {
		if (!m_pInstance) {
			m_pInstance = new CObjMgr;
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
	template<typename T>
	CObject* ReQuestObject(OBJ_TAG tag);

	vector<CObject*>& Get_ObjectVector(OBJ_TAG tag) { return m_activeObject[tag]; };

private:
	static CObjMgr* m_pInstance;
	CObject* m_pPlayer;

	unordered_map<OBJ_TAG, vector<CObject*>> m_activeObject;
	unordered_map<OBJ_TAG, vector<CObject*>> m_poolObject;
};


template<typename T>
CObject* CObjMgr::ReQuestObject(OBJ_TAG tag) {
	CObject* obj = nullptr;

	// 풀에서 꺼내기
	if (!m_poolObject[tag].empty()) {
		for (auto iter = m_poolObject[tag].begin(); iter != m_poolObject[tag].end(); ++iter) {
			if (dynamic_cast<T*>(*iter)) {
				obj = *iter;
				m_poolObject[tag].erase(iter); // 풀에서 제거
				break;
			}
		}
	}

	// 못 찾았으면 새로 생성
	if (!obj) {
		obj = new T;
	}

	obj->Set_Active(true);
	m_activeObject[tag].push_back(obj);

	return obj;
}

template<>
inline CObject* CObjMgr::ReQuestObject<CPlayer>(OBJ_TAG tag) {
	if (!m_pPlayer) {
		m_pPlayer = new CPlayer;
		m_pPlayer->Set_Active(true);

		m_pPlayer->Initialize();
		m_activeObject[PLAYER].push_back(m_pPlayer); // 최초 1회만 추가
	}
	return m_pPlayer;
}
