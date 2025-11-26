#pragma once
#include "Emitter.h"

class CSpriteEmitter :
    public CEmitter
{
private:
    CSpriteEmitter();
    ~CSpriteEmitter() DEFAULT;

public:
    HRESULT Initialize(SpriteEmitterData Data);
    void Update(float dt, const EffectTransform& transform) override;
    void Render() override;

private:
    SpriteEmitterData m_Data;    
    EffectTransform   m_Transform;

    _float m_Time = 0.f;
    _uint m_CurrentFrame = 0;
    _bool m_Alive = true;

public:
    static CSpriteEmitter* Create(SpriteEmitterData Data);
    void Free() override;
};

