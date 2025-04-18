#pragma once
#include "CComponent.h"
class CCollider :
    public CComponent
{
public:
    CCollider();
    ~CCollider();

public:
    // CComponent을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    VECTOR2& GetPos() { return m_vPos; }
    void SetPos(VECTOR2 _pos) { m_vPos = _pos; }
    VECTOR2& GetScale() { return m_vScale; }
    void SetScale(VECTOR2 _scale) { m_vScale = _scale; }

private:
    VECTOR2 m_vPos;
    VECTOR2 m_vScale;
};

