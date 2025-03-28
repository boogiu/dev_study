#pragma once
class CPlayer;

class CScene
{
public :
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Release();
protected:
	CPlayer* m_pPlayer;
};

