#pragma once
#include "CScene.h"

class CBattleScene : public CScene
{
public:
	CBattleScene();
	~CBattleScene() override;

public:
	void Initialize()override;
	void Update()override;
	void Render()override;
	void Release()override;
};

