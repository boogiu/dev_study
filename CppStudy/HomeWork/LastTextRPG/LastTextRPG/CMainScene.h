#pragma once
#include "CScene.h"

class CMainScene:public CScene
{
public:
	CMainScene();
	~CMainScene() override;

public:
	void Initialize()override;
	void Update()override;
	void Render()override;
	void Release()override;
};

