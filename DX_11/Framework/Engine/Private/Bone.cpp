#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::InitializeFromFile(ifstream& ifs)
{
	return S_OK;
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
