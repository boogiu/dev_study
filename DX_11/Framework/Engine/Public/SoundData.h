#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class ENGINE_DLL CSoundData final :
    public CBase
{

private:
    CSoundData(const string& key);
    virtual ~CSoundData() DEFAULT;
public:
    HRESULT Initialize( FMOD::System* pSystem, const string& filePath);
    FMOD::Sound* Get_SoundData() { return m_pSound; };
private:
    string m_SoundKey;
    FMOD::Sound* m_pSound = { nullptr };
public:
    static CSoundData* Create( FMOD::System* pSystem, const string& filePath, const string& key);
    virtual void Free() override;

};

NS_END