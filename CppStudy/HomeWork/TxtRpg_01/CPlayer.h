#ifndef __CPLAYER_H__
#define __CPLAYER_H__
#include "define.h"

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
	void ShowStat();
	//죽으면 false
	void GetDamage(int dmg);
	void Restore();
	int GetAtk() { return m_pInfo.m_iAtk; }
	bool isAlive() { return !(m_pInfo.m_iNowHp <= 0); }
private:
	char* m_cName;
	CLASS_ENUM m_Class;
	int m_iGold;
	tagInfo m_pInfo;
	friend class CGameMgr;
};

#endif // !__CPLAYER_H__

