#pragma once
#include "Effects.h" // 이펙트 기반 클래스 포함 (CBaseEffect 또는 CEffect 등)

class CEffectMgr
{
private:
    CEffectMgr();
    ~CEffectMgr();
    CEffectMgr(const CEffectMgr&) = delete;
    CEffectMgr& operator=(const CEffectMgr&) = delete;

public:
    void Initialize();
    void Update();
    void Late_Update();
    void PostRender(HDC _hDC);
    void Render(HDC _hDC);
    void Release();

public:
    static CEffectMgr* Get_Instance() {
        if (!m_pInstance)
            m_pInstance = new CEffectMgr;
        return m_pInstance;
    }

    static void Destroy_Instance() {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

public:
    template<typename T>
    CObject* ReQuestEffect(EFFECT_TAG tag); // EFFECT_TAG는 enum 타입으로 이펙트 종류를 구분

    vector<CObject*>& Get_EffectVector(EFFECT_TAG tag) { return m_activeEffect[tag]; }

private:
    static CEffectMgr* m_pInstance;

    unordered_map<EFFECT_TAG, vector<CObject*>> m_activeEffect;
    unordered_map<EFFECT_TAG, vector<CObject*>> m_poolEffect;
};

template<typename T>
CObject* CEffectMgr::ReQuestEffect(EFFECT_TAG tag) {
    CObject* effect = nullptr;

    // 풀에서 재사용 가능한 이펙트 찾기
    if (!m_poolEffect[tag].empty()) {
        for (auto iter = m_poolEffect[tag].begin(); iter != m_poolEffect[tag].end(); ++iter) {
            if (dynamic_cast<T*>(*iter)) {
                effect = *iter;
                m_poolEffect[tag].erase(iter);
                break;
            }
        }
    }

    // 없으면 새로 생성
    if (!effect) {
        effect = new T;
    }

    effect->Set_Active(true);
    dynamic_cast<CBaseEffect*>(effect)->Set_Tag(tag);
    m_activeEffect[tag].push_back(effect);

    return effect;
}
