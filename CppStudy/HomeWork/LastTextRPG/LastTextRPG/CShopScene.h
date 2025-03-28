#pragma once
#include "CScene.h"
class CShopScene :public CScene {
public:
	CShopScene();
	~CShopScene() override;

public:
	void Initialize()override;
	void Update()override;
	void Render()override;
	void Release()override;
};

