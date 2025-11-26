#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class ENGINE_DLL CEffectData :
    public CBase
{
private:
    CEffectData();
    ~CEffectData() DEFAULT;

public:
    _float Get_Durate() { return m_fDuration; };
    _bool isLoop() { return m_bLoop; };
    string Get_Name() { return name; };
    vector<SpriteEmitterData>& Get_SpriteEmitterData() { return m_SpriteEmitterDatas; }

private:
    string name;
    _float m_fDuration = 1.0f;
    _bool m_bLoop = false;
    vector<SpriteEmitterData> m_SpriteEmitterDatas;

public:
    static CEffectData* Create(const EffectDataPreset& preset);
    static CEffectData* Create();
    virtual void Free();
};

NS_END