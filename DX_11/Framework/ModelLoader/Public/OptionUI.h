#pragma once
#include "GameObject.h"

NS_BEGIN(Loader)
class COptionUI :
    public CGameObject
{
protected:
    COptionUI();
    COptionUI(const COptionUI& rhs);
    virtual ~COptionUI() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Render_GUI() override;

private:
    void Render_ImporterTab();
    void Render_Logs();
    HRESULT LookUp_Folder();
    HRESULT Load_AIScene(const string& filePath);

protected:
    Importer		m_Importer = {};
    const aiScene* m_pAIScene = { nullptr };

    _bool isTabOpen = { false };
    _bool bSaveModel = { true };
    _bool bSaveMaterial = { true };
    _bool isStaticModel = { false };

    _bool isReadyToSave = { false };
    _uint m_iSaving_Index = {};

    string m_ParentPath = {};
    string m_SavePath = {};
    vector<string> m_Pathes;
    vector<string> m_Logs;


    class CLoadStaticModel* m_pStaticModelComp = { nullptr };
    class CLoadSkeletalModel* m_pSkeletalModelComp = { nullptr };
    class CLoadMaterial* m_pMaterial = { nullptr };

    string overrideShaderKey = {};
    string overridePassKey = {};
public:
    static COptionUI* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END