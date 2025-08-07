#include "Engine_Define.h"
#include "CMeshRenderer.h"
#include "CResourceMgr.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CTransform.h"
#include "CStateCache.h"

CMeshRenderer::CMeshRenderer()
	:m_pMesh(nullptr), m_pMaterial(nullptr)
{
}

CMeshRenderer::~CMeshRenderer()
{
}

CMeshRenderer* CMeshRenderer::Create()
{
	CMeshRenderer* instance = new CMeshRenderer;

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CMeshRenderer::Ready_Component()
{
	return S_OK;
}

void CMeshRenderer::Update_Component(float& dt)
{
	
}

void CMeshRenderer::LateUpdate_Component(float& dt)
{
	if (m_bActive)
		CRenderMgr::GetInstance()->Add_Renderer(this);
}

void CMeshRenderer::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_pTransform || !m_pMesh)
		return;

	// Transform 적용
	pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());

	LPD3DXMESH pMesh = m_pMesh->GetMesh();
	if (!pMesh) return;

	const auto& materials = m_pMaterial ? m_pMaterial->Get_Material() : vector<MATTEX>();

	DWORD subsetCount = m_pMesh->GetSubsetCount();
	m_pCache->SetFVF(m_pMesh->GetFVF());

	for (DWORD i = 0; i < subsetCount; ++i)
	{
		// 머티리얼 설정
		if (i < materials.size()) {
			pDevice->SetMaterial(&materials[i].material);
			pDevice->SetTexture(0, materials[i].texture);
		}

		pMesh->DrawSubset(i);
	}
}



CComponent* CMeshRenderer::Clone() const
{
	return nullptr;
}

void CMeshRenderer::Set_Mesh(const wstring& key)
{
	m_pMesh = static_cast<CMesh*>(CResourceMgr::GetInstance()->Find_Mesh(key));

	if (m_pMesh)
		m_pMesh->AddRef();

	m_pMaterial = CResourceMgr::GetInstance()->Find_Material(key);

	if (m_pMaterial)
		m_pMaterial->AddRef();
}


void CMeshRenderer::Free()
{

	if (m_pMesh)
		Safe_Release(m_pMesh);

	if (m_pMaterial)
		Safe_Release(m_pMaterial);

}
