#pragma once
#include "CComponent.h"

class CRenderer :
    public CComponent
{
public:
    enum class RenderLayer {
        OBJECT,
    };

public:
    CRenderer();
    virtual ~CRenderer() override;
public:
    void Initialize() override;
    void Update(float dt) override;
    static ComponentType StaticType() { return ComponentType::Renderer; }
    ComponentType GetType() const override { return StaticType(); }
};

