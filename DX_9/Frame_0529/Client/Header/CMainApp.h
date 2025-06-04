#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"

namespace Engine {
	class CGameObject;
}
class CTestCam;

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public: 
	HRESULT Ready_MainApp();
	int Update_MainApp(const float& fTimeDelta);
	void LateUpdate_MainApp(const float& fTimeDelta);
	void Render_MainApp();
private:
	Engine::CGraphicDev* m_pDeviceClass;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	CGameObject* pTest=nullptr;
	CGameObject* pLight=nullptr;
	CTestCam* pCam=nullptr;

public:
	static CMainApp* Create();
private:
	virtual void Free() override;
};

