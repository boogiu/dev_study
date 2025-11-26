#include "Client_Defines.h"
#include "BaseEffect.h"
CBaseEffect::CBaseEffect()
{
}

CBaseEffect::CBaseEffect(const CBaseEffect& rhs)
    :CGameObject(rhs)
{
}

HRESULT CBaseEffect::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CBaseEffect::Initialize(INIT_DESC* pArg)
{
    __super::Initialize(pArg);
    return S_OK;
}

void CBaseEffect::Priority_Update(_float dt)
{
}

void CBaseEffect::Update(_float dt)
{
}

void CBaseEffect::Late_Update(_float dt)
{
}

void CBaseEffect::Render_GUI()
{
}

void CBaseEffect::Set_DeActive()
{
}

void CBaseEffect::Set_ReActive(const EffectData& data)
{
}

CBaseEffect* CBaseEffect::Create()
{
	CBaseEffect* instance = new CBaseEffect();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CBaseEffect");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CBaseEffect::Clone(INIT_DESC* pArg)
{
	CBaseEffect* instance = new CBaseEffect(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CBaseEffect");
		Safe_Release(instance);
	}

	return instance;
}

void CBaseEffect::Free()
{
	__super::Free();
}