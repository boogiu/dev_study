#pragma once
#include "Animator3D.h"
NS_BEGIN(Loader)
class CLoadAnimator3D :
    public CAnimator3D
{
private:
    CLoadAnimator3D();
    CLoadAnimator3D(const CLoadAnimator3D& rhs);
    virtual ~CLoadAnimator3D() DEFAULT;
public:
    HRESULT Initialize();
    void Set_Data(class CModelData* pData);
    void Update_Load();
private:
    HRESULT Add_AIAnimation(const string& filePath);
    void Release_Data();
    void Save_Animations();

public :
    void Render_GUI() override;

private:
    Importer		m_Importer = {};
    const aiScene* m_pAIScene = { nullptr };
    _bool m_bShowAnimatorSkeletons = { false };
    _int SelectIndex = {};
    vector<string> LoadPathes;
    string ParentPath = {};
    _int nowIndex = {-1};

public:
    static CLoadAnimator3D* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};
NS_END
