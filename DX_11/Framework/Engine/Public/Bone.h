#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class ENGINE_DLL CBone :
    public CBase
{
protected:
    CBone();
    virtual ~CBone() DEFAULT;
public:
    HRESULT InitializeFromFile(ifstream& ifs);
    void Update_CombinedTransformMatrix(const vector<CBone*> Bones);
    _matrix Get_CombinedTransformationMatrix();
protected:
    string				m_BoneName = {};
    _float4x4			m_TransformationMatrix = {};
    _float4x4			m_CombinedTransformationMatrix = {};
    _int				m_iParentBoneIndex = { -1 };

public:
    static CBone* Create(ifstream& ifs);
    virtual void Free() override;
};

NS_END