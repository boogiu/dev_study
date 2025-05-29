#pragma once
#include "CComponent.h"

class CRenderer :
    public CComponent
{
public:
    CRenderer();
    virtual ~CRenderer() override;
public:
    void Initialize() override;
    void Update(float dt) override;
    void Late_Update(float dt) override;
    void Render(HDC _hDC) ;
    
    static ComponentType StaticType() { return ComponentType::Renderer; }
    ComponentType GetType() const override { return StaticType(); }

    void Set_Layer(RenderLayer layer) { m_eLayer = layer; }
    RenderLayer Get_Layer() { return m_eLayer; }

private:
    RenderLayer m_eLayer = RenderLayer::BASE;
};

