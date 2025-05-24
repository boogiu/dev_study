#pragma once
#include "UI_Objects.h"
class CCamera;
class CObject;

class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();
	CUIMgr(CUIMgr& rhs) = delete;
	CUIMgr operator=(CUIMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();
public:
	void UI_Set(bool Show);
public:
	static CUIMgr* Get_Instance() {
		if (!m_pInstance) {
			m_pInstance = new CUIMgr;
		}
		return m_pInstance;
	}
	static void Destroy_Instance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	template<typename T>
	CBase_UI* Request_UI() {
		CBase_UI* instance = new T;
		instance->Set_Active(true);
		m_UI_List.push_back(instance);
		return instance;
	}

private:
	static CUIMgr* m_pInstance;
	bool m_bShow;
	list <CBase_UI*> m_UI_List;
	CObject* m_pTarget;
};

