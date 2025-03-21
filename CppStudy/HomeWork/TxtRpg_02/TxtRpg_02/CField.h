#pragma once
#include "CScene.h"

class CPlayer;
class CObject;

class CField : public CScene
{
public :
	CField();
	~CField() override;
public :
	void Init(CPlayer* _pPlayer)override;
	void Render() override;
	void Release() override;
	void MonsterPool(int _level);

private:
	int m_iSpawnCnt;
	CObject** m_MonsterArr;
};

