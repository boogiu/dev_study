#include "pch.h"
#include "CRenderManager.h"
#include "CRenderer.h"

CRenderManager::CRenderManager()
{
}

CRenderManager::~CRenderManager()
{
	Release();
}

void CRenderManager::Initialize()
{
}

void CRenderManager::Update()
{
}

void CRenderManager::Late_Update()
{
}

void CRenderManager::Render(HDC _hDC)
{
	for (CRenderer* render : m_RenderContainer[RenderLayer::BASE]) {
		render->Render(_hDC);
	}
	for (CRenderer* render : m_RenderContainer[RenderLayer::OBJECT]) {
		render->Render(_hDC);
	}
}

void CRenderManager::Release()
{
}

void CRenderManager::RegisterRenderer(CRenderer* comp)
{
	RenderLayer layer =comp->Get_Layer();
	auto& container = m_RenderContainer[layer];

	auto iter = std::find_if(container.begin(), container.end(),
		[&comp](CRenderer* renderer)->bool {
			return comp == renderer;
		});

	if (iter == container.end()) {
		container.push_back(comp);
	}
	else {
		return;
	}
}

void CRenderManager::ReleaseRenderer(CRenderer* comp)
{
	RenderLayer layer = comp->Get_Layer();
	auto& container = m_RenderContainer[layer];
	auto iter = std::remove_if(container.begin(), container.end(),
		[&comp](CRenderer* renderer)->bool {
			return comp == renderer;
		});

	if (iter != container.end()) {
		container.erase(iter, container.end());
	}
}
