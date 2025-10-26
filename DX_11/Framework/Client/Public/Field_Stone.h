#pragma once
#include "FieldObject.h"
NS_BEGIN(Client)
class CField_Stone :
    public CFieldObject
{
private:
    CField_Stone();
    CField_Stone(const CField_Stone& rhs);
    virtual ~CField_Stone() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();
    virtual HRESULT Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable) override;
public:
    void OnCollisionEnter(CGameObject* pObj);
private:
    void Override_Pass();

public:
    static CField_Stone* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END