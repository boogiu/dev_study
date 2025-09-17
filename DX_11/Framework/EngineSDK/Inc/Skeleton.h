#pragma once
#include "Base.h"

NS_BEGIN(Engine)
using MESHBONE = vector<class CBone*>;
class ENGINE_DLL CSkeleton :
    public CBase
{
protected:
    CSkeleton();
    virtual ~CSkeleton() DEFAULT;
public:
    HRESULT InitializeFromFile(const string& filePath);
    void Update_CombinedMatrix(_float dt);
    _matrix Get_CombinedMatrix(_uint BoneIndex);
    _uint Get_BoneCount() { return m_Bones.size(); };
    const vector<class CBone*>& Get_Bones() { return m_Bones; };
    _int FindBoneIndex_ByName(const string& boneName);
protected:
    vector<class CBone*> m_Bones;
    unordered_map<string, _uint> m_BoneMap;
public:
    static CSkeleton* Create(const string& filePath);
    virtual void Free() override;
};

NS_END