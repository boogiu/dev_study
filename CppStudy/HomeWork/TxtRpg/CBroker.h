#ifndef _BROKER_
#define _BROKER_
#include "CPlayer.h"

class CBroker
{
public :
	CBroker(); //다른 함수간의 연결이 있을 경우 사용해주기
	~CBroker();

public :
	void SetPlayer(CPlayer* _player);
	void DealPC( int _class); //DealPlayerClass
	char*  DealPN(); //DealPlayerName
	void DealPG();

private:
	CPlayer* player;
};

#endif // !_BROKER_


