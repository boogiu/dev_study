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
	m_pDevice = pDevice;

	if(!m_pDevice)
		return E_FAIL;
	m_pDevice->AddRef();

	m_ePolicy = LIGHT_POLICY::CAMERA_POS;

	m_pDevice->LightEnable(0, true);

	return S_OK;
}

void CLightMgr::Update_Light(_float& dt)
{
	const size_t maxLights = 8;
	size_t count = 0;

	for (; count < m_LightContainer.size();)
	{
		if (count > maxLights) //상위 8개 까지만
			break;

		if (!m_LightContainer[count]->Get_ComponentActive())
			continue;//활성화 아니라면 넘어감

		const D3DLIGHT9& desc = m_LightContainer[count]->Get_Light();

		m_pDevice->SetLight(count, &desc);
		m_pDevice->LightEnable(count, TRUE);

		++count;
	}

	// 남은 라이트는 꺼주기
	for (; count < maxLights; ++count) {
		m_pDevice->LightEnable(count, FALSE);
	}
	m_LightContainer.clear();

}

void CLightMgr::Set_Priority()
{
	//여기서 빛의 우선순위를 관리할 것임.
}


void CLightMgr::Add_Light(CLight* light)
{
	auto iter = find_if(m_LightContainer.begin(), m_LightContainer.end(),
		[&light](CLight* data)->bool {
			return data == light;
		});

	if (iter != m_LightContainer.end()) 
		return;

	m_LightContainer.push_back(light);
}

void CLightMgr::Free()
{
	m_LightContainer.clear();
	Safe_Release(m_pDevice);
}
