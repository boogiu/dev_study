#pragma once
#include "FieldObject.h"
NS_BEGIN(Client)
class CField_Bridge :
    public CFieldObject
{

private:
    CField_Bridge();
    CField_Bridge(const CField_Bridge& rhs);
    virtual ~CField_Bridge() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    virtual HRESULT Sync_MapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable) override;

public:
    static CField_Bridge* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END