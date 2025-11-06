#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CNonPlayer :
    public CGameObject
{
    struct NPC_EventMsg {
       
    };
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
    void Add_EventListen();

protected:
    class CNpcState_Machine* m_pMachine = { nullptr };
    NPC_EventMsg m_EventMsg = {};
    _float m_fDistance = {};
    _float4 m_fMovevector = {};
public:
    virtual void Free()override;
};
NS_END
