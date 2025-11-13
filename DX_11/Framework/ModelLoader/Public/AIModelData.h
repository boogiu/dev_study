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
    void CollectMeshNodes(aiNode* pNode, const aiMatrix4x4& parentTransform);
public:
    virtual void Render_GUI();
    void Save_File(ofstream& ofs);
private:
    _bool m_bShowSkeletonBones = { false };
    unordered_map<UINT, aiMatrix4x4> m_MeshNodeTransform;
    unordered_map<UINT, string> m_MeshNodeName;

public:
    static CAIModelData* Create(const aiScene* pAIScene, ID3D11Device* pDevice, MESH_TYPE eType);
    virtual void Free() override;
};
NS_END
