#include "pch.h"
#include "CDialogueMgr.h"

CDialogueMgr::CDialogueMgr()
{
}

CDialogueMgr::~CDialogueMgr()
{
}

void CDialogueMgr::Initialize()
{
}

void CDialogueMgr::Update()
{
}

void CDialogueMgr::Render()
{
	wcout << L"=================================== " << endl;
	wcout << m_Message << endl;
	wcout << L"=================================== " << endl;
}

void CDialogueMgr::Release()
{
}

void CDialogueMgr::SetMssg(const wstring& _msg)
{
	m_Message = _msg;
}
