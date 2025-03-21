#pragma once
class CPlayer;
class CScene
{
public:
	CScene() :m_pPlayer(nullptr) {};
	virtual ~CScene()=0;

public:
	virtual void Init() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
protected:
	CPlayer* m_pPlayer;
};

