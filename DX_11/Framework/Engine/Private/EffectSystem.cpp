#include "EffectSystem.h"
#include "EffectData.h"
#include "EffectInstance.h"
#include "GameInstance.h"
#include "SpriteEffectRenderer.h"

/*Å¬·¡½º Æó±â*/
CEffectSystem::CEffectSystem()
{
}

CEffectSystem::~CEffectSystem()
{
}

HRESULT CEffectSystem::Initialize()
{
	ID3D11Device* pDevice= 	CGameInstance::GetInstance()->Get_Device();
	m_pSpriteRenderer = CSpriteEffectRenderer::Create(pDevice);

	if (m_pSpriteRenderer == nullptr) {
		return E_FAIL;
	}
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


void CEffectSystem::Render(ID3D11DeviceContext* pContext)
{
	for (auto Instance : m_ActiveEffects)
	{
		Instance->Render();
	}

	m_pSpriteRenderer->RenderAll(pContext);
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

void CEffectSystem::Spawn_Preset(const EffectDataPreset& preset, const EffectRequestPacket& desc)
{
	CEffectInstance* pInstance = { nullptr };

	if (!m_EffectPool.empty()) {
		pInstance = m_EffectPool.back();
		m_EffectPool.pop_back();
	}
	else {
		pInstance = CEffectInstance::Create();
	}

	pInstance->MakePreset(preset, desc);
	m_ActiveEffects.push_back(pInstance);
}

void CEffectSystem::QueingSpriteEffect(const EffectSpriteDrawDesc& desc)
{
	m_pSpriteRenderer->Queue(desc);
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

	Safe_Release(m_pSpriteRenderer);
}
