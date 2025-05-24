#pragma once

#include "CRenderer.h"

class CRenderManager
{
private:
	CRenderManager();
	~CRenderManager();

public:
	static CRenderManager& Get_Instance() {
		static CRenderManager instance;
		return instance;
	}

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();
private:
};

