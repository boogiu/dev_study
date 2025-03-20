#pragma once
class CPlayer;
class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void Init(CPlayer* _pPlayer);
	virtual void Render();
	virtual void Release();
protected:
	CPlayer* m_pPlayer;
};

