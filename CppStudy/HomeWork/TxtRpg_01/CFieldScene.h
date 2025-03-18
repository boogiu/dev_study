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
	void MonsterPool();

private:
	CMonster** m_MonsterArr;
};

#endif // !__FIELDSCENE_H__

