#include "Engine_Define.h"
#include "CLightMgr.h"
#include "CLight.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{
}

CLightMgr::~CLightMgr()
{
	Free();
}

HRESULT CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pDevice)
{

	m_baseLight.Type = D3DLIGHT_DIRECTIONAL;
	m_baseLight.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	m_baseLight.Ambient = { 0.3f, 0.3f, 0.3f, 1.f };
	m_baseLight.Specular = { 0.2f, 0.2f, 0.2f, 1.f };
	m_baseLight.Direction = { -1.f, -1.f, -1.f }; // 일반적으로 위쪽에서 아래로 비추는 방향

	pDevice->SetLight(0, &m_baseLight);
	pDevice->LightEnable(0, true);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);		// 정규화된 노멀 사용
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);			// 스페큘러 하이라이트

	return S_OK;
}

void CLightMgr::Set_Light(LPDIRECT3DDEVICE9 pDevice)
{
	const size_t maxLights = 8;
	size_t count = 1; // 0번은 baseLight 고정임

	for (size_t i = 0; i < m_LightContainer.size() && count < maxLights; ++i)
	{
		if (!m_ActivedLight[i]) continue;//활성화 아니라면 넘어감

		const D3DLIGHT9& desc = m_LightContainer[i]->Get_Light();

		pDevice->SetLight(count, &desc);
		pDevice->LightEnable(count, TRUE);

		++count;
	}

	// 남은 라이트는 꺼주기
	for (; count < maxLights; ++count) {
		pDevice->LightEnable(count, FALSE);
	}
}


void CLightMgr::Add_Light(CLight* light)
{
	auto iter = find_if(m_LightContainer.begin(), m_LightContainer.end(),
		[&light](CLight* data)->bool {
			return data == light;
		});

	if (iter != m_LightContainer.end()) 
		return;

	light->AddRef();

	m_LightContainer.push_back(light);
	m_ActivedLight.push_back(true);

	light->Set_ID(m_LightContainer.size() - 1);
}

void CLightMgr::Remove_Light(CLight* light)
{
	if (!light) return;

	auto iter = std::find(m_LightContainer.begin(), m_LightContainer.end(), light);

	if (iter != m_LightContainer.end()) {
		size_t index = std::distance(m_LightContainer.begin(), iter);

		// 컨테이너에서 해당 라이트 제거
		m_LightContainer.erase(iter);

		// Actived 플래그도 같은 인덱스에서 제거
		if (index < m_ActivedLight.size()) {
			m_ActivedLight.erase(m_ActivedLight.begin() + index);
		}

		for (size_t i = 0; i < m_LightContainer.size(); ++i) {
			m_LightContainer[i]->Set_ID(static_cast<int>(i));
		}
	}
}

void CLightMgr::Free()
{
	for_each(m_LightContainer.begin(), m_LightContainer.end(),
		[](auto& light) {
			Safe_Release(light);
		});

	m_LightContainer.clear();
}
