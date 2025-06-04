#pragma once
#include "CBase.h"
BEGIN(Engine)

class CCamera;

class ENGINE_DLL CCameraMgr :
	public CBase
{
	DECLARE_SINGLETON(CCameraMgr)
private:
	explicit CCameraMgr();
	virtual ~CCameraMgr();

public:
	//카메라는 카메라 액터에 붙어서 카메라 액터가 움직이는 형태
	void Set_ViewTarget(CCamera* pCam);
	void Apply_Camera(LPDIRECT3DDEVICE9 pDevice);
	CCamera* Get_Camera() { return m_pCurCam; }

private:
	virtual void Free();

private:
	CCamera* m_pCurCam;
};
END
