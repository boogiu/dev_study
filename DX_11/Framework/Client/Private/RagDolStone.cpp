#include "Client_Defines.h"
#include "RagDolStone.h"

#include "RagDolPart.h"
#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "ObjectContainer.h"
#include "Helper_Func.h"

CRagDolStone::CRagDolStone()
{
}

CRagDolStone::CRagDolStone(const CRagDolStone& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CRagDolStone::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	return S_OK;
}

HRESULT CRagDolStone::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);


	for (size_t i = 0; i < 9; i++)
	{
		CGameObject* pObj = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_RagDolPart" }).Build("Effect_RagDolStone");
		Get_Component<CObjectContainer>()->Add_Child(pObj, true);
		m_Parts.push_back(dynamic_cast<CRagDolPart*>(pObj));
	}

	m_pSmoke = dynamic_cast<CBaseEffect*>(Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_RagDolSmoke" })
		.Build("Effect_RagDolSmoke"));
	Get_Component<CObjectContainer>()->Add_Child(m_pSmoke, true);
	return S_OK;
}

void CRagDolStone::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CRagDolStone::Update(_float dt)
{
	m_fLifeTime += dt;

	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CRagDolStone::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CRagDolStone::Render_GUI()
{
}

_bool CRagDolStone::isEffectActive()
{
	return m_fLifeTime < 3.f;
}

void CRagDolStone::Reset()
{
	m_fLifeTime = 0.f;
}

void CRagDolStone::Set_DeActive()
{
	m_isAlive = false;
}

void CRagDolStone::Set_ReActive(const EffectData& data)
{
	Reset();
	m_isAlive = true;

	_int count = Helper::Get_Random_Int(5, 9);

	for (size_t i = 0; i < count; i++)
	{
		m_Parts[i]->Set_ReActive(data);
	}
	m_pSmoke->Set_ReActive(data);
}

CRagDolStone* CRagDolStone::Create()
{
	CRagDolStone* instance = new CRagDolStone();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CRagDolStone");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CRagDolStone::Clone(INIT_DESC* pArg)
{
	CRagDolStone* instance = new CRagDolStone(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CRagDolStone");
		Safe_Release(instance);
	}

	return instance;
}

void CRagDolStone::Free()
{
	__super::Free();
}
