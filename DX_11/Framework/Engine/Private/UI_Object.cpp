#include "UI_Object.h"
#include "GameInstance.h"
#include "Model.h"
#include "Material.h"
#include "IRenderService.h"
#include "IInputService.h"
CUI_Object::CUI_Object()
{
}

CUI_Object::CUI_Object(const CUI_Object& rhs)
    :CGameObject(rhs)
{
    m_fX = rhs.m_fX;
    m_fY = rhs.m_fY;
    m_fSizeX = rhs.m_fSizeX;
    m_fSizeY = rhs.m_fSizeY;
    m_fRadian = rhs.m_fRadian;
    m_ViewMat = rhs.m_ViewMat;
    m_ProjMat = rhs.m_ProjMat;
}

HRESULT CUI_Object::Initialize_Prototype()
{
    XMStoreFloat4x4(&m_ViewMat, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMat, XMMatrixIdentity());

//    CModel* model = Add_Component<CModel>();
//    CMaterial* material = Add_Component<CMaterial>();
//
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CUI_Object::Initialize(INIT_DESC* pArg)
{
    UI_DESC* uiDesc = static_cast<UI_DESC*>(pArg);
    m_fX = uiDesc->fX;
    m_fY = uiDesc->fY;

    m_fSizeX = uiDesc->fSizeX;
    m_fSizeY = uiDesc->fSizeY;
    m_fRadian = uiDesc->fRadian;

    __super::Initialize(pArg);
    return S_OK;
}

void CUI_Object::Engine_Update(_float dt)
{
  
    Update_UITransform();

    UI_PACKET packet;
   // packet.pModel = Add_Component<CModel>();
   // packet.pMaterial = Add_Component<CMaterial>();
    packet.pWorldMatrix = m_pTransform->Get_WorldMatrix();
    packet.pViewMatrix = &m_ViewMat;

    CGameInstance::GetInstance()->Get_RenderSystem()->Submit_UI(packet);
}

void CUI_Object::Priority_Update(_float dt)
{

}

void CUI_Object::Update(_float dt)
{
}

void CUI_Object::Late_Update(_float dt)
{
}

void CUI_Object::Update_UITransform()
{
    m_WinSizeX = CGameInstance::GetInstance()->Get_ClientSize().x;
    m_WinSizeY = CGameInstance::GetInstance()->Get_ClientSize().y;

    m_pTransform->Scale({ m_fSizeX,m_fSizeY,1.f });
    m_pTransform->Set_Pos({m_fX - m_WinSizeX*0.5f,  -m_fY+m_WinSizeY*0.5f, 0.f});
    m_pTransform->Rotate({ 0,0,m_fRadian});
}

void CUI_Object::Rotate_Left(_float _radian)
{
    m_fRadian += _radian;
}


//Align_To(Anchor::Bottom | Anchor::Right, { 0,10 });
_float2 CUI_Object::Align_To(UI_Anchor anchor, _float2 _pivot)
{
    _uint anchorFlags = static_cast<_uint>(anchor);

    if (anchorFlags & static_cast<_uint>(UI_Anchor::Left))
        m_fX = _pivot.x + HalfX();
    else if (anchorFlags & static_cast<_uint>(UI_Anchor::Right))
        m_fX = _pivot.x - HalfX();
    else 
        m_fX = _pivot.x;

    if (anchorFlags & static_cast<_uint>(UI_Anchor::Top))
        m_fY = _pivot.y + HalfY();
    else if (anchorFlags & static_cast<_uint>(UI_Anchor::Bottom))
        m_fY = _pivot.y - HalfY();
    else 
        m_fY = _pivot.y;

    return _float2(m_fX, m_fY);
}
