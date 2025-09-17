#include "Skeleton.h"
#include "Bone.h"

CSkeleton::CSkeleton()
{
}

HRESULT CSkeleton::InitializeFromFile(const string& filePath)
{
    return S_OK;
}

void CSkeleton::Update_CombinedMatrix(_float dt)
{
    for (auto& bone : m_Bones)
        bone->Update_CombinedTransformMatrix(m_Bones);
}

_matrix CSkeleton::Get_CombinedMatrix(_uint BoneIndex)
{
    return m_Bones[BoneIndex]->Get_CombinedTransformationMatrix();
}

_int CSkeleton::FindBoneIndex_ByName(const string& boneName)
{
    auto iter = m_BoneMap.find(boneName);

    if (iter != m_BoneMap.end())
        return iter->second;
    else
        return -1;
}

CSkeleton* CSkeleton::Create(const string& filePath)
{
    CSkeleton* instance = new CSkeleton;
    if (FAILED(instance->InitializeFromFile(filePath))) {
        Safe_Release(instance);
    }
    return instance;
}

void CSkeleton::Free()
{
    __super::Free();
    for (auto& bones : m_Bones)
        Safe_Release(bones);

    m_Bones.clear();
}
