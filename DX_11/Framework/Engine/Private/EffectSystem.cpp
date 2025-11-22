#include "EffectSystem.h"
#include "EffectData.h"
#include "EffectInstance.h"

CEffectSystem::CEffectSystem()
{
}

CEffectSystem::~CEffectSystem()
{
}

HRESULT CEffectSystem::Initialize()
{
	return S_OK;
}

void CEffectSystem::Update(_float dt)
{
	for (int i = 0; i < m_ActiveEffects.size(); ++i)
	{
		CEffectInstance* pEffect = m_ActiveEffects[i];
		pEffect->Update(dt);

		if (!pEffect->IsAlive())
		{
			m_EffectPool.push_back(pEffect);
			m_ActiveEffects.erase(m_ActiveEffects.begin() + i);
			--i;
		}
	}
}

void CEffectSystem::Render()
{
	for (auto Instance : m_ActiveEffects)
	{
		Instance->Render();
	}
}

void CEffectSystem::Spawn(CEffectData* data, const EffectRequestPacket& desc)
{
	CEffectInstance* pInstance = { nullptr };

	if (!m_EffectPool.empty()) {
		pInstance = m_EffectPool.back();
		m_EffectPool.pop_back();
	}
	else {
		pInstance = CEffectInstance::Create();
	}

	pInstance->Play(data, desc);
	m_ActiveEffects.push_back(pInstance);
}


CEffectSystem* CEffectSystem::Create()
{
	CEffectSystem* Instance = new CEffectSystem();
	if (FAILED(Instance->Initialize())) {
		Safe_Release(Instance);
	}
	return Instance;
}

void CEffectSystem::Free()
{
	__super::Free();

	for (auto Instance : m_ActiveEffects)
		Safe_Release(Instance);
	for (auto Instance : m_EffectPool)
		Safe_Release(Instance);

	m_ActiveEffects.clear();
	m_EffectPool.clear();
}
