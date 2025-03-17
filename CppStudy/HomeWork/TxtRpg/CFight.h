#ifndef _FIGHT_
#define _FIGHT_

#include "CPlayer.h"
#include "CMonster.h"

class CFight
{
public:
	static int FightMonster(CPlayer* _player, CMonster* _monster);
};

#endif // !_FIGHT_

