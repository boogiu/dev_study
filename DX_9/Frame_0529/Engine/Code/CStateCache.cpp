#include "Engine_Define.h"
#include "CStateCache.h"

CStateCache::CStateCache()
	:m_pDevice(nullptr)
{
}

CStateCache::~CStateCache()
{
}

CStateCache* CStateCache::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStateCache* instance = new CStateCache;

	if (FAILED(instance->Ready_Cache(pDevice))) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

void CStateCache::SetRenderState(_D3DRENDERSTATETYPE type, DWORD value)
{
	auto iter = m_stateContainer.find(type);

	if (iter == m_stateContainer.end()) {
		m_stateContainer.insert({ type , value });
		m_pDevice->SetRenderState(type, value); // 상태 캐시에 없던 경우에도 설정 필요
	}
	else {
		if (iter->second == value)
			return;

		m_pDevice->SetRenderState(type, value);
		iter->second = value;
	}
}


void CStateCache::SetFVF(DWORD value)
{
	if (value == curFVF)
		return;
	
	curFVF = value;
	m_pDevice->SetFVF(curFVF);
}

void CStateCache::Clear()
{
	m_stateContainer.clear();
}

HRESULT CStateCache::Ready_Cache(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	if(!pDevice)
		return E_FAIL;

	m_pDevice->AddRef();
	return S_OK;
}

void CStateCache::Free()
{
	m_stateContainer.clear();
	Safe_Release(m_pDevice);
}
