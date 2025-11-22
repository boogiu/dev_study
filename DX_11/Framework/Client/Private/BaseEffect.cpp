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

void CBaseEffect::Free()
{
    __super::Free();
}
