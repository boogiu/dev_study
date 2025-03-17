#ifndef _BATTLESCENE_
#define _BATTLESCENE_

#include "CScene.h"
#include "CMonster.h"

class CBattleScene : public CScene
{
public:
	CBattleScene();
	~CBattleScene() override;

public:
	// CScene을(를) 통해 상속됨
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

private:
	CMonster** monsters;
};

#endif // !_BATTLESCENE_



