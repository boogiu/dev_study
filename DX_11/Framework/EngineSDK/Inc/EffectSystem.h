#pragma once
#include "IEffectService.h"
NS_BEGIN(Engine)
class  CEffectSystem :
    public IEffectService
{
private:
    explicit CEffectSystem();
    virtual ~CEffectSystem() override;

public:
    HRESULT Initialize();
    void Update(_float dt);
    void Render(ID3D11DeviceContext* pContext);

public:
    void Spawn(class CEffectData* data, const EffectRequestPacket& desc);
    void Spawn_Preset(const EffectDataPreset& preset, const EffectRequestPacket& desc);

public:
    void QueingSpriteEffect(const EffectSpriteDrawDesc& desc);

private:
    vector<class CEffectInstance*> m_ActiveEffects;
    vector<class CEffectInstance*>  m_EffectPool;
    unordered_map<string, class CEffectData*> m_EffectDatas;

private: /*subRenderer*/
    class CSpriteEffectRenderer* m_pSpriteRenderer = { nullptr };

public:
    static CEffectSystem* Create();
    virtual void Free() override;
};

NS_END


/*
인스턴스 : 실제 이펙트 객체
데이터 : 이펙트 관련 데이터
에미터 : 분출기 -> 이펙트 객체 안에 들어간 데이터의 개수, 파티클 들을 만족시키며 뿌리는 역할
에미터 템플릿 : 분출 방법이 적혀진 데이터

-> 데이터와 템플릿을 쌍으로 정적 데이터로 묶음.
그리고 인스턴스가 이 데이터를 읽고, 자신이 가진 에미터로 뿌림과 동시에 여러가지 이펙트 작업을 수행함.

*/