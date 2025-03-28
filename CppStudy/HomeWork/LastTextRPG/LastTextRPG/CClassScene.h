#pragma once
#include "CScene.h"

class CClassScene :public CScene
{
public:
	CClassScene();
	~CClassScene() override;

public:
	void Initialize()override;
	void Update()override;
	void Render()override;
	void Release()override;
};
