#pragma once
#include "CScene.h"
class CLogoScene  : public CScene
{
public:
	CLogoScene();
	~CLogoScene();

	// CScene을(를) 통해 상속됨
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

private:
	HDC memDC;

	// CScene을(를) 통해 상속됨
	void CloseScene() override;
};

