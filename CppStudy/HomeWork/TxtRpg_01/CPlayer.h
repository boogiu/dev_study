#ifndef __CPLAYER_H__
#define __CPLAYER_H__
#include "pch.h"

class CPlayer
{
private :
	//생성자 제한 : 게임 매니저
	CPlayer();
	//소멸자 제한 : 게임 매니저
	~CPlayer();

public: 
	void Init();
	void Render();
	void Release();
public:
	void SetClass(CLASS_ENUM _class);
	void SetName();
	char* GetName();
private:
	char* m_cName;
	CLASS_ENUM m_Class;
	int m_iGold;
	int m_iMaxHp;
	int m_iNowHp;

	int m_iAtk;

	friend class CGameMgr;
};

#endif // !__CPLAYER_H__

