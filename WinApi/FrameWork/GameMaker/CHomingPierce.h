#pragma once
#include "CObject.h"
#include <gdiplus.h>
using namespace Gdiplus;
class CAttackLine;
class CHomingPierce : public CObject
{
public:
    CHomingPierce();
    virtual ~CHomingPierce();
    enum class STATE { APPEAR, TARGETING, FIRE };

public:
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC _hDC) override;
    virtual void Release() override;

    void Appear();

public: 
    void Set_Angle(float _angle) { m_fAngle = _angle; }
    void Set_Angle(int _angle) { m_fAngle = (float)_angle; }
    void Set_State(STATE _State) { m_eState = _State; }

private:
    STATE m_eState;
    Image* PierceAnim;      // 아틀라스 이미지
    Image* PierceImg;       // 본 이미지
    float m_fAngle;         // 회전 각도
    bool m_bFired;          // 발사 여부
    bool m_bAppear;          // 발사 여부

    int m_iFrame;           // 현재 프레임
    float m_fFrameTime;     // 누적 시간
    CAttackLine* m_AtkLine;
    const int m_iMaxFrame = 21;
    const float m_fFrameDelay = 0.2f;
};
