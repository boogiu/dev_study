#pragma once
class CPlayer;
class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void Init() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
protected:
	CPlayer* m_pPlayer;
};

