#pragma once
#include "CScene.h"
class CShopScene:public CScene
{
public:
	CShopScene();
	~CShopScene() override;

public:
	// CScene을(를) 통해 상속됨
	void Init() override;
	void Render() override;
	void Release() override;
	int SelectLevel();
	void ShowItems(int _level);
};

