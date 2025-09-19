#pragma once
#include "ModelData.h"
#include "Loader_Defines.h"

NS_BEGIN(Loader)
class CAIModelData :
    public CModelData
{
protected:
    CAIModelData();
    virtual ~CAIModelData() override;

public:
    HRESULT Initialize(const aiScene* pAIScene, ID3D11Device* pDevice,MESH_TYPE eType);
public:
    virtual void Render_GUI();
    void Save_File(ofstream& ofs);
private:
    _bool m_bShowSkeletonBones = { false };

public:
    static CAIModelData* Create(const aiScene* pAIScene, ID3D11Device* pDevice, MESH_TYPE eType);
    virtual void Free() override;
};
NS_END
