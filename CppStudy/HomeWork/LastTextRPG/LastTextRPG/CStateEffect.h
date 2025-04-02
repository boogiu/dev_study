#pragma once
class  CObject;
class CStateEffect
{
public:
	CStateEffect();
	~CStateEffect();

public:
	virtual void operator()(CObject* target);
};

