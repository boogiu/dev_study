#include "Engine_Define.h"
#include "CSceneMgr.h"
#include "CScene.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	:m_curScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	Free();
}

HRESULT CSceneMgr::Ready_Scene()
{
	return S_OK;
}

void CSceneMgr::Update_Scene(_float& dt)
{
	if (m_curScene)
		m_curScene->Update_Scene(dt);
}

void CSceneMgr::LateUpdate_Scene(_float& dt)
{
	if (m_curScene)
		m_curScene->LateUpdate_Scene(dt);
}

void CSceneMgr::Render_Scene(LPDIRECT3DDEVICE9 pDevice)
{
	if (m_curScene)
		m_curScene->Render_Scene(pDevice);
}

void CSceneMgr::Change_Scene(const wstring& key)
{
	CScene* next = Find_Scene(key);

	if (!next) return;

	if(m_curScene){

		HRESULT exit = m_curScene->Exit_Scene();

		if (FAILED(exit)) {
			MessageBoxW(0, L"¾À ÀüÈ¯ ½ÇÆÐ, ¾À ÅðÀå ¿À·ù", L"error", MB_OK);
		}
	}

	HRESULT enter = next->Enter_Scene();

	if (FAILED(enter)) {
		MessageBoxW(0, L"¾À ÀüÈ¯ ½ÇÆÐ, ¾À ÀÔÀå ¿À·ù", L"error", MB_OK);
		return;
	}

	m_curScene = next;
}

CScene* CSceneMgr::Find_Scene(const wstring& key)
{
	if (m_SceneContainer.empty())
		return nullptr;


	return m_SceneContainer.find(key)->second;
}


HRESULT CSceneMgr::Add_Scene(const wstring& key, CScene* scene)
{
	if (Find_Scene(key)) {
		MessageBoxW(0, L"¾À Ãß°¡ ½ÇÆÐ. Å° Áßº¹µÊ", L"error", MB_OK);
		return E_FAIL;
	}

	if (!scene) {
		MessageBoxW(0, L"¾À Ãß°¡ ½ÇÆÐ. Àß¸ø µÈ ¾À", L"error", MB_OK);
		return E_FAIL;
	}

	m_SceneContainer.insert({ key,scene });
	return S_OK;
}

void CSceneMgr::Free()
{
	for (auto& pair : m_SceneContainer) {
		Safe_Release(pair.second);
	}
	m_SceneContainer.clear();
}
