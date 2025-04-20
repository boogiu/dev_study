#pragma once
#include "CScene.h"
#include <string>

/*아직은 사용하지 않을 것 같음*/
template<typename T>
class CSceneFactory {
	CScene* CreateScene() {
		CScene* Instance = new T;
		return Instance;
	}
};