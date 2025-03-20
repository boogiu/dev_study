#ifndef __FIELDSCENE_H__
#define __FIELDSCENE_H__
#include "CBaseScene.h"

class CPlayer;
class CMonster;

class CFieldScene : public CBaseScene
{
public:
	// CBaseScene을(를) 통해 상속됨
	CFieldScene();
	~CFieldScene() override;

public:
	void Init() override;
	void Render() override;
	void Release() override;
	int FightMonster(CPlayer* player, CMonster* monster);
private:
	CMonster** m_MonsterArr;
	CPlayer* pPlayer;
	int m_iMonsterCount;
};

#endif // !__FIELDSCENE_H__

