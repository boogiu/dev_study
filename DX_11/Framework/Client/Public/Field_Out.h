#pragma once
#include "FieldObject.h"

NS_BEGIN(Client)
class CField_Out :
    public CFieldObject
{
private:
    CField_Out();
    CField_Out(const CField_Out& rhs);
    virtual ~CField_Out() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    virtual HRESULT Sync_MapData(NEW_MAP_OBJECT_HEADER objHeader,vector<string> modelMapTable) override;
private:
    void Override_Pass();

private:
    _float m_fElpaseTime = {0.f};
    _float m_fWaveTime = {0.f};
    _float m_fCircularTime = {0.f};
    _float m_fFade = {0.f};

    _float m_Distance = {0.f};
    static _float Player_Distance;
    static CField_Out* s_pSoundOwner;
    static _uint      s_LastFrameUpdated;
public:
    static CField_Out* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END