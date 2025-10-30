#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CFieldHole :
    public CGameObject
{
    enum HoleState{Digged, Barried};
private:
    CFieldHole();
    CFieldHole(const CFieldHole& rhs);
    ~CFieldHole() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI()override;

protected:
    HoleState m_eState = { Digged };
public:
    static CFieldHole* Create();
    void Free() override;

    // CGameObject을(를) 통해 상속됨
    CGameObject* Clone(INIT_DESC* pArg) override;
};

NS_END

