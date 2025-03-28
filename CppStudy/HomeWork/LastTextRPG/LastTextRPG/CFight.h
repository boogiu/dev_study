#pragma once
class CObject;
class CPlayer;
class CFight
{
public:
	static bool Fight(CPlayer* player, CObject* competitor);
private :
	static int iTurn;
};

