#include "Engine_Define.h"
#include "CTerrainCollider.h"
#include "CResourceMgr.h"
#include "CTerrain.h"

CTerrainCollider::CTerrainCollider()
{
}

CTerrainCollider::~CTerrainCollider()
{
}

CTerrainCollider* CTerrainCollider::Create()
{
	CTerrainCollider* instance = new CTerrainCollider;

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CTerrainCollider::Ready_Component()
{
	return S_OK;
}

void CTerrainCollider::Update_Component(float& dt)
{
}

void CTerrainCollider::LateUpdate_Component(float& dt)
{
	if (m_bActive)
		Register_System();
}

CComponent* CTerrainCollider::Clone() const
{
	return nullptr;
}
void CTerrainCollider::Set_Mesh(const wstring& key)
{
	m_pMesh = dynamic_cast<CTerrain*>(CResourceMgr::GetInstance()->Find_Mesh(key));

	if (m_pMesh) {
		m_pMesh->AddRef();
	}
}
void CTerrainCollider::Free()
{
	Safe_Release(m_pMesh);
}