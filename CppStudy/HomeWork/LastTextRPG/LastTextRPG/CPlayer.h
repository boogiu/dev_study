#pragma once
#include "CObject.h"

class CPlayer:public CObject
{
public:
	CPlayer();
	~CPlayer() override;
public :
	void Initialize() override;
	void Release() override;
	
public:
	void SetName();
	void SetClass(CLASS _class);

private:

};

