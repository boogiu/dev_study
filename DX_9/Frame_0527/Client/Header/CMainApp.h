#pragma once
#include "CBase.h"

class CMainApp:public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Ready_MainApp();
	int			Update_MainApp(const float& fTimeDelta);
	void		LateUpdate_MainApp(const float& fTimeDelta);
	void		Render_MainApp();

public:
	static CMainApp* Create();
private:
	virtual void Free();
};

