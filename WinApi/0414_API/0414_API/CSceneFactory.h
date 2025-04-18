#pragma once
#include "CScene.h"
#include "define.h"

template <typename T>
class CSceneFactory
{
public:
	CSceneFactory() {};
	~CSceneFactory() {};

public:
	static CScene* CreateScene() {
		CScene* tmp = new T;
		return tmp;
	}
};

