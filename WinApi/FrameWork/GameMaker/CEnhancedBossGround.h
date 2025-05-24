#pragma once
#include "CBossState.h"
class CBaseEffect;

class CEnhancedBossGround :
    public CBossState
{
public:
    CEnhancedBossGround();
    ~CEnhancedBossGround();

public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;

private:
    void LinearEffect(CBoss* boss);
    void LightenEffect(CBoss* boss);
    void SMokenEffect(CBoss* boss);

private:
    bool m_bReady;
    bool m_bMove;
    bool m_bMoveEnd;
    bool m_bBorder;
    float m_lightElapsedTime;
    int m_LightCount;

    CBaseEffect* m_pEffcet;
    CBaseEffect* m_pLightening[5];
    CAttackBox* m_pLightAtk[5];
};

