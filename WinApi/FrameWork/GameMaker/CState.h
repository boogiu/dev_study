#pragma once
class CObject;

class CState
{
public:
	CState();
	virtual ~CState();
public:
	virtual void Enter(CObject& obj) PURE;
	virtual void Update(CObject& obj) PURE;
	virtual bool Exit(CObject& obj) PURE;
};

