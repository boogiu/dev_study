#pragma once
//싱글톤 클래스
/*
* 오브젝트의 탄생과 삭제를 관리하는 클래스
* 다른 객체나 클래스에서 오브젝트를 원할 때엔
* 해당 클래스에서 꺼내서 쓰는 방식으로 진행.
*/

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();
	CObjMgr(CObjMgr& rhs) = delete;
	CObjMgr operator=(CObjMgr& rhs)=delete;

public:
	void Intialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();
	
public:
	template<typename T>
	CGameObject* RequestObject(wstring _monsterID) {
		if (CGameObject* obj= checkPool(_monsterID)) {
			return obj;
		}
		else {
			obj = new T;
			obj->setActive(true);
			m_umapObjects[_monsterID].push_back(obj);
			return obj;
		}
	}

	CGameObject* checkPool(wstring _monsterID);


public:
	static CObjMgr* Get_Instance() {
		if (!m_ObjMgr) {
			m_ObjMgr = new CObjMgr;
		}
		return m_ObjMgr;
	}

	static void Destroy_Instance() {
		if (m_ObjMgr) {
			delete m_ObjMgr;
		}
		m_ObjMgr = nullptr;
	}

private:
	static CObjMgr* m_ObjMgr;
	unordered_map<wstring, vector<CGameObject*>> m_umapObjects;
};

