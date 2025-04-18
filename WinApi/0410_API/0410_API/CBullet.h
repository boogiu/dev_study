#pragma once
#include "CObject.h"
class CBullet :
    public CObject
{
public:
    CBullet();
    ~CBullet();

public:
    // CObject을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

    // CObject을(를) 통해 상속됨
    void Activate() override;
    void DeActivte() override;

    void SetTarget(INFO target);

private :
    void OnCollisionEnter(CObject* _object) override;
    void OnCollisionEnter(CTile* _tile) override;
    void OnCollisionOut(CTile* _tile) override;

private:
    void MoveBullet();
};

