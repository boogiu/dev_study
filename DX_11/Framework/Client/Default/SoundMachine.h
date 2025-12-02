#pragma once
#include "Base.h"

NS_BEGIN(Client)
class CSoundMachine
	:public CBase
{
    struct SoundData {
        string Key;
        _float NowTime = {};
        _float IntervalTime = {};
    };
    struct SoundPacket {
        void Update(_float dt);
        void Play(string, string);
    };

private:
	CSoundMachine();
	~CSoundMachine();

public:
    void Update(_float dt);
    void Play(string, string);

private:
    unordered_map<string, SoundData> Sounds;
    class CAudioSource* m_pSource = { nullptr };

public:
	static CSoundMachine* Create(class CAudioSource* pAudio);
};

NS_END