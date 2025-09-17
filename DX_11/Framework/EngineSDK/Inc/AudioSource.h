#pragma once
#include "Component.h"
NS_BEGIN(Engine)

class ENGINE_DLL CAudioSource :
    public CComponent
{
public:
    typedef struct tagAudioSlot {
        _bool isLoop = { false };
        _bool isPaused = { false };
        _bool is3DAttribute = {true};

        _float fVolume = { 1.f };
        _int iLoopCount = { 0 };

        class CSoundData* pSound = {};
        FMOD::Channel* pChanel = { nullptr };
    }AUDIO_SLOT;

private:
    CAudioSource();
    CAudioSource(const CAudioSource& rhs);
    virtual ~CAudioSource();
public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
    HRESULT Add_Slot(const string& levelTag, const string& SoundKey, const string& slotKey = {}, bool isLoop = false);
    void Play(const string& SoundKey);

public:
    void Render_GUI();
private:
    class IAudioService* m_pAudioDevice = { nullptr };
    class CTransform* m_pTransform = { nullptr };
    unordered_map<string, AUDIO_SLOT> m_Audios;
    _float4 m_vPos = {};
public:
    static CAudioSource* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};
NS_END
