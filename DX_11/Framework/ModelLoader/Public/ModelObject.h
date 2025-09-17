#pragma once
#include "GameObject.h"
NS_BEGIN(Loader)

class CModelObject :
    public CGameObject
{
private:
    CModelObject();
    CModelObject(const CModelObject& rhs);
  virtual ~CModelObject() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
public:
   void Render_GUI() override;
private:
    HRESULT Load_AIScene(const string& filePath);
    HRESULT Load_Static(const string& filePath);
    HRESULT Load_Animated(const string& filePath);
    HRESULT Save_AIScene();

    _bool HasBones();
private:
    Importer		m_Importer = {};
    const aiScene* m_pAIScene = { nullptr };
public:
    static CModelObject* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END