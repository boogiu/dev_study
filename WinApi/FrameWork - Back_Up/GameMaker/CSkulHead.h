#pragma once
#include "CProjectile.h"
class CSkulHead : public CObject
{
public:
    CSkulHead();
    ~CSkulHead();

    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

    void OnCollisionEnter(CObject* object, RECT collRECT) override;

private:
    Gdiplus::Image* skullImg;
    CColider* m_Colider;

    bool isHitted;
    float m_fAngle;
};
