#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CNonPlayer :
    public CGameObject
{
protected:
    CNonPlayer();
    CNonPlayer(const CNonPlayer& rhs);
    virtual ~CNonPlayer() DEFAULT;
public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override; 
    virtual void Awake() override;

public:
    void Render_GUI() override;

protected:
    void Add_BaseAnimClip();
    void Add_Parts();

protected:
    class CNpcState_Machine* m_pMachine = { nullptr };
public:
    virtual void Free()override;
};
NS_END
