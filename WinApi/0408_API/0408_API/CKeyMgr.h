#pragma once
class CKeyMgr
{
public:
	CKeyMgr();
	~CKeyMgr();

public :
	void Initialize();
	void Update();
	void Render();
	void Release();

public:
	KEYINFO KeyUpdate();

private:

};

