#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__
#include "CBaseScene.h"

class CMainScene:public CBaseScene
{
public:
	CMainScene();
	~CMainScene() override;

public:
	void Init() override;
	void Render() override;
	void Release() override;

	void AskName();
	void AskDirection();

};


#endif // !__MAINSCENE_H__
