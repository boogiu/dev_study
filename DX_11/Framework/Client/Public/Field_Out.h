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
    virtual HRESULT Sync_MapData(MAP_OBJECT_HEADER objHeader,vector<string> modelMapTable) override;
private:
    void Override_Pass();

public:
    static CField_Out* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END