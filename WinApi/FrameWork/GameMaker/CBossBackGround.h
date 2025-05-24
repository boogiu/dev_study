#pragma once
#include "CObject.h"
class CAnim;
class CBoss;

class CBossBackGround : public CObject
{
public:
	CBossBackGround();
	~CBossBackGround();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();
public:
	void SetAnim(BOSS_STATE _state);

private:
	bool m_bFadeOut;
	bool m_bFadeSound;
	BOSS_STATE prevState;
	CAnim* m_pAnim;
};

