#ifndef __STARTSCENE_H__
#define __STARTSCENE_H__

#include "CBaseScene.h"

class CStartScene :public CBaseScene
{
public:

	CStartScene();
	~CStartScene() override;
	// CBaseScene을(를) 통해 상속됨
public:
	void Init() override;
	void Render() override;
	void Release() override;
	int RenderMain();
	void SelectClass();


};


#endif // !__STARTSCENE_H__

