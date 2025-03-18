#ifndef __SCENE_H__
#define __SCENE_H__
#include "pch.h"

class CBaseScene
{
public:
	CBaseScene();
	virtual ~CBaseScene();

public:
	virtual void Init();
	virtual void Render();
	virtual void Release();
	void ChangeScene(SCENE_ENUM _scene);
};

#endif // !__SCENE_H__
