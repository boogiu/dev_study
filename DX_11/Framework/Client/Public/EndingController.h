#pragma once
#include "LevelObject.h"

NS_BEGIN(Client)
class CEndingController :
    public CLevelObject
{
private:
    CEndingController();
    CEndingController(const CEndingController& rhs);
    ~CEndingController() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt)override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void Set_EventSystem(class CEventSystem* pSystem);
    void Recieve_Ending(const BaseEvent& evt);

public:
    void Batch_Character();
    void Execute_Scene();
    void Set_EnvObject(class CGameObject* pSky, class CGameObject* pSun, class CGameObject* pLeaf);

private:
    _bool m_bEndingStart = { false };
    _bool m_bEndingProceed= { false };
    _float m_fElapsedTime = {};

    class CEventSystem* m_pEventSystem = { nullptr };
    class CPlayer* m_Player = { nullptr };
    vector<class CNonPlayer*> m_NonPlayers ;

    class CSunCam* m_pSun = { nullptr };
    class CSkyBox* m_pSkyBox = { nullptr };
    class CLeafParticle* m_pLeaf = { nullptr };
    class CMoon* m_pMoon= { nullptr };
    class CGameObject* m_pStars= { nullptr };
    class CSceneCamera* m_pEndingCam = { nullptr };
public:
    static CEndingController* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
