#pragma once
#include "LevelObject.h"

NS_BEGIN(Client)
class CEffectSpawner :
    public CLevelObject
{
private:
    CEffectSpawner();
    CEffectSpawner(const CEffectSpawner& rhs);
    ~CEffectSpawner() DEFAULT;
public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt)override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void Request_Effect(const string& tag,const EffectData& data);
    class CBaseEffect* Build_Effect(const string& tag);

private:
    vector<class CBaseEffect*> m_ActivePool;
    vector<class CBaseEffect*> m_ReturnPool;
    unordered_map<string, vector<class CBaseEffect*>> m_EffectPool;

public:
    static CEffectSpawner* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
