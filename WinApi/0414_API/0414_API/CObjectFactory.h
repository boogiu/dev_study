#pragma once
#include "CGameObject.h"

template<typename T>
class CObjectFactory
{
public:
	CObjectFactory(){}
	~CObjectFactory(){}
public:
	static CGameObject* CreateObject() {
		CGameObject* pObj = new T;
		//r°ü¸®ÀÚ
		return pObj;
	}
};

