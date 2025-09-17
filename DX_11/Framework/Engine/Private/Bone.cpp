#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::InitializeFromFile(ifstream& ifs)
{
	return S_OK;
}

void CBone::Update_CombinedTransformMatrix(const vector<CBone*> Bones)
{
    if (m_iParentBoneIndex == -1) {
        XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix));
    }
    else {
        XMStoreFloat4x4(&m_CombinedTransformationMatrix, 
            XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
    }
}

_matrix CBone::Get_CombinedTransformationMatrix()
{
    return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
}

CBone* CBone::Create(ifstream& ifs)
{
    CBone* instance = new CBone;
    if (FAILED(instance->InitializeFromFile(ifs))) {
        Safe_Release(instance);
    }
    return instance;
}

void CBone::Free()
{
}
