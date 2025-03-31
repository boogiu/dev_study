#pragma once
#include "CScene.h"

class  CPlayer;
class CObject;

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

private:
	bool Fight(CPlayer* player, CObject* compete);
};

