#include "pch.h"
#include "CSceneManager.h"

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::Initialize()
{
}

void CSceneManager::Render(SCENE _scene)
{
	switch (_scene)
	{
	case CSceneManager::MAIN:
		break;
	case CSceneManager::START:
		break;
	case CSceneManager::VILLAGE:
		break;
	case CSceneManager::SHOP:
		break;
	case CSceneManager::MOTEL:
		break;
	case CSceneManager::BATTLE:
		break;
	case CSceneManager::END:
		break;
	default:
		break;
	}
}


void CSceneManager::Update()
{
}

void CSceneManager::Release()
{
}
