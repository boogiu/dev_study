#include "pch.h"
#include "CKeyMgr.h"

CKeyMgr::CKeyMgr()
{
}

CKeyMgr::~CKeyMgr()
{
	Release();
}

void CKeyMgr::Initialize()
{
}

void CKeyMgr::Update()
{
}

void CKeyMgr::Render()
{
}

void CKeyMgr::Release()
{
}

KEYINFO CKeyMgr::KeyUpdate()
{
	KEYINFO tmp{ static_cast<int>(KEY::NONE), KEYSTATE::NONE };

	if (GetAsyncKeyState(VK_UP)) {
		tmp.KeyFlag |= static_cast<int>(KEY::UP);
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		tmp.KeyFlag |= static_cast<int>(KEY::DOWN);
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		tmp.KeyFlag |= static_cast<int>(KEY::LEFT);
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		tmp.KeyFlag |= static_cast<int>(KEY::RIGHT);
	}

	if (GetAsyncKeyState('W')) {
		tmp.KeyFlag |= static_cast<int>(KEY::W);
	}
	if (GetAsyncKeyState('A')) {
		tmp.KeyFlag |= static_cast<int>(KEY::A);
	}
	if (GetAsyncKeyState('S')) {
		tmp.KeyFlag |= static_cast<int>(KEY::S);
	}
	if (GetAsyncKeyState('D')) {
		tmp.KeyFlag |= static_cast<int>(KEY::D);
	}
	return tmp;
}
