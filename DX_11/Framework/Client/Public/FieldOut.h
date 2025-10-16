#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CFieldOut :
    public CGameObject
{
public:
    typedef struct tagFieldOutDesc : public GAMEOBJECT_DESC
    {
        TILE_INDEX Index = {};
        string LevelTag;
        string ModelName;
        string MaterialName;

        tagFieldOutDesc() DEFAULT;
        virtual ~tagFieldOutDesc() DEFAULT;
    }FIELDOUT_DESC;

private:
    CFieldOut();
    CFieldOut(const CFieldOut& rhs);
    virtual ~CFieldOut() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

private:
    void Override_Pass();
public:
    static CFieldOut* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END