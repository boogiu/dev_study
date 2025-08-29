#pragma once
#include "Engine_Defines.h"

typedef struct  tagGUIContextInterface{
	class IObjectService* pObjectManager = { nullptr };
	class ILevelService* pLevelManager = { nullptr };
	class CLevel* pSelectedLevel = { nullptr };
	class CLayer* pSelectedLayer = { nullptr };
	class CGameObject* pSelectedObject = { nullptr };

	SIZE VPSize = {};
}GUI_CONTEXT;