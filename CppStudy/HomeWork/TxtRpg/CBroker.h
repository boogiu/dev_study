#ifndef _BROKER_
#define _BROKER_
#include "CPlayer.h"

class CBroker
{
public :
	CBroker();
	~CBroker();

public :
	void SetPlayer(CPlayer* _player);
	void DealPC( int _class); //DealPlayerClass
	char*  DealPN(); //DealPlayerName

private:
	CPlayer* player;
};

#endif // !_BROKER_


