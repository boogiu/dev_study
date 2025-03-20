#pragma once

#include "CScene.h"

class CPlayer;
class CVillage : public CScene
{
public:
	CVillage();
	~CVillage() override;

public:
	void Init(CPlayer* _pPlayer)override;
	void Render()override;
	void Release()override;
};

