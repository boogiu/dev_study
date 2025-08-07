#include "Engine_Define.h"
#include "CScene.h"
#include "CRenderMgr.h"
#include "CLayer.h"

CScene::CScene()
	: m_pCache(nullptr)
{
}

CScene::~CScene()
{
}

void CScene::Create_Layer(const wstring& key)
{
	if (m_mapLayer.count(key)) return;

	CLayer* tmp = CLayer::Create();
	m_mapLayer.insert({key,tmp });

}

void CScene::Free_Layer(const wstring& key)
{
	auto iter = m_mapLayer.find(key);

	if (iter == m_mapLayer.end()) return;

	Safe_Release(iter->second);
	m_mapLayer.erase(iter);
}

void CScene::Free_AllLayer()
{
	for (auto& pair : m_mapLayer) {
		Safe_Release(pair.second);
		pair.second = nullptr;
	}

	m_mapLayer.clear();
}
