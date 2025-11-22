#pragma once
#include "Emitter.h"

class CSpriteEmitter :
    public CEmitter
{
private:
    CSpriteEmitter();
    ~CSpriteEmitter() DEFAULT;

public:
    HRESULT Initialize(EmitterTemplate* pData);
    void Update(float dt, const EffectTransform& transform) override;
    void Render() override;

private:
    SpriteEmitterData m_Data;    
    EffectTransform   m_Transform;

    _float m_Time = 0.f;
    _int m_CurrentFrame = 0;
    _bool m_Alive = true;
public:
    static CSpriteEmitter* Create(EmitterTemplate* pData);
    void Free() override;
};

