#pragma once
#include "CScene.h"

class CPlayer;
class CMainScene : public CScene
{
public:
	CMainScene();
	~CMainScene() override;

public:
	void Init(CPlayer* _pPlayer)override;
	void Render()override;
	void Release()override;
};

