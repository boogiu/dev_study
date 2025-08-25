#include "Client_Defines.h"
#include "BackGround.h"
#include "GameInstance.h"
#include "GameObject.h"

CBackGround::CBackGround()
{
}

CBackGround::CBackGround(const CBackGround& rhs)
    : CGameObject(rhs)
{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Initialize_Prototype()
{
    //여기서 컴포넌트 추가 ...//
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CBackGround::Initialize(INIT_DESC* pArg)
{
    //여기서 컴포넌트등 여러 초기화 설정 진행 ...//

    __super::Initialize(pArg);
    return S_OK;
}


void CBackGround::Priority_Update(_float dt)
{
    int i = 0;
}

void CBackGround::Update(_float dt)
{
    int i = 0;
}

void CBackGround::Late_Update(_float dt)
{
    int i = 0;
}

CBackGround* CBackGround::Create()
{
    CBackGround* instance = new CBackGround();
    if (FAILED(instance->Initialize_Prototype()))
    {
        MSG_BOX("Object Create Failed : CBackGround");
        Safe_Release(instance);
    }

    return instance;
}

CGameObject* CBackGround::Clone(INIT_DESC* pArg)
{
    CBackGround* instance = new CBackGround(*this);

    if (FAILED(instance->Initialize(pArg)))
    {
        MSG_BOX("Object Clone Failed : CBackGround");
        Safe_Release(instance);
    }

    return instance;
}

void CBackGround::Free()
{
    __super::Free();
}
