#include "pch.h"
#include "CComponent.h"

#include "CRenderer.h"
#include "CRenderManager.h"

CComponent::CComponent()
	: m_pOwner (nullptr)
{
}

CComponent::~CComponent()
{
}

template<>
void RegisterOnSystem<CRenderer>(CRenderer* renderer) {
    CRenderManager::Get_Instance().RegisterRenderer(renderer);
}

template<>
void ReleaseOnSystem<CRenderer>(CRenderer* renderer) {
    CRenderManager::Get_Instance().ReleaseRenderer(renderer);
}
