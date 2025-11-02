#include "UI_Object.h"
#include "GameInstance.h"
#include "Model.h"
#include "Material.h"
#include "IRenderService.h"
#include "Sprite2D.h"
#include "Child.h"

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
    m_WinSizeX = rhs.m_WinSizeX;
    m_WinSizeY = rhs.m_WinSizeY;
}

HRESULT CUI_Object::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    Add_Component<CSprite2D>();

    m_WinSizeX = CGameInstance::GetInstance()->Get_ClientSize().x;
    m_WinSizeY = CGameInstance::GetInstance()->Get_ClientSize().y;

    return S_OK;
}

HRESULT CUI_Object::Initialize(INIT_DESC* pArg)
{
    __super::Initialize(pArg);
     UI_DESC* uiDesc = static_cast<UI_DESC*>(pArg);

     if (pArg != nullptr) {
        m_fX = uiDesc->fX;
        m_fY = uiDesc->fY;

        m_fSizeX = uiDesc->fSizeX;
        m_fSizeY = uiDesc->fSizeY;
        m_fRadian = uiDesc->fRadian;
        Update_UITransform();
     }
    return S_OK;
}


void CUI_Object::Pre_EngineUpdate(_float dt)
{
    __super::Pre_EngineUpdate(dt);
    Update_UITransform();
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

void CUI_Object::Post_EngineUpdate(_float dt)
{
    UI_PACKET packet;
    packet.pSprite2D = Get_Component<CSprite2D>();
    packet.pWorldMatrix = m_pTransform->Get_WorldMatrix_Ptr();

    if (packet.pSprite2D == nullptr)
        return;
    if (!packet.pSprite2D->IsValid())
        return;
    if (!packet.pSprite2D->Get_CompActive())
        return;

    CGameInstance::GetInstance()->Get_RenderSystem()->Submit_UI(packet);


    for (auto& child : Get_Children()) {
        if (child)
            child->Post_EngineUpdate(dt);
    }
}

void CUI_Object::Render_GUI()
{
    __super::Render_GUI();
    ImGui::SeparatorText("Position");
    if (ImGui::Button("Left"))  m_fX -= 10.f;
    ImGui::SameLine();
    if (ImGui::Button("Right")) m_fX += 10.f;
    if (ImGui::Button("Up"))			m_fY -= 10.f;
    ImGui::SameLine();
    if (ImGui::Button("Down"))    m_fY += 10.f;

    ImGui::SeparatorText("Scale");
    if (ImGui::Button("Sub X"))       m_fSizeX -= 10.f;
    ImGui::SameLine();
    if (ImGui::Button("Add X"))         m_fSizeX += 10.f;
    if (ImGui::Button("Sub Y"))			m_fSizeY -= 10.f;
    ImGui::SameLine();
    if (ImGui::Button("Add Y"))    m_fSizeY += 10.f;
}

void CUI_Object::Update_UITransform()
{
    m_WinSizeX = CGameInstance::GetInstance()->Get_ClientSize().x;
    m_WinSizeY = CGameInstance::GetInstance()->Get_ClientSize().y;

    m_pTransform->Scale({ m_fSizeX, m_fSizeY, 1.f });
    m_pTransform->Set_Pos({ m_fX - m_WinSizeX * 0.5f, -m_fY + m_WinSizeY * 0.5f, 0.f });
    m_pTransform->Rotate({ 0, 0, m_fRadian });

    if (auto pChildComp = Get_Component<CChild>())
    {
        if (auto pParent = dynamic_cast<CUI_Object*>(pChildComp->Get_Parent()))
        {
            m_pTransform->Set_Pos({ (m_fX + pParent->m_fX) - m_WinSizeX * 0.5f, -(m_fY + pParent->m_fY) + m_WinSizeY * 0.5f, 0.f });
        }
    }

}

void CUI_Object::Rotate_Left(_float _radian)
{
    m_fRadian += _radian;
}

//Align_To(Anchor::Bottom | Anchor::Right, { 0,10 });
_float2 CUI_Object::Align_To(ANCHOR anchor, _float2 _pivot)
{
    _uint anchorFlags = static_cast<_uint>(anchor);

    if (anchorFlags & static_cast<_uint>(ANCHOR::Left))
        m_fX = _pivot.x + HalfX();
    else if (anchorFlags & static_cast<_uint>(ANCHOR::Right))
        m_fX = _pivot.x - HalfX();
    else 
        m_fX = _pivot.x;

    if (anchorFlags & static_cast<_uint>(ANCHOR::Top))
        m_fY = _pivot.y + HalfY();
    else if (anchorFlags & static_cast<_uint>(ANCHOR::Bottom))
        m_fY = _pivot.y - HalfY();
    else 
        m_fY = _pivot.y;

    return _float2(m_fX, m_fY);
}

void CUI_Object::Free()
{
    __super::Free();
}
