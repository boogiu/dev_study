#pragma once
#include "Loader_Defines.h"
#include "Mesh.h"
NS_BEGIN(Engine)
    class CSkeleton;
NS_END
NS_BEGIN(Loader)
class CAIMesh final:
    public CMesh
{
private:
    CAIMesh(const string& meshID);
    virtual ~CAIMesh();

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, const aiMesh* pAIMesh,  CSkeleton* pSkeleton, MESH_TYPE eType);
private:
	virtual HRESULT Create_Vertex(ID3D11Device* pDevice) override;
	virtual HRESULT Create_AnimVertex(ID3D11Device* pDevice);
	virtual HRESULT Create_Index(ID3D11Device* pDevice) override;
public:
    const vector<VTXSKINMESH>& Get_Vertex() { return m_VBSkinContainer; };
    const vector<_uint>& Get_Index() { return m_IBContainer; };
    const vector<_uint>& Get_BoneIndices() { return m_BoneIndices; };
    virtual _uint Get_BoneCount() { return m_BoneIndices.size(); };

    void Save_File(ofstream& ofs);
private:
    const aiMesh* m_pAIMesh = { nullptr };
     CSkeleton* m_pSkeleton = { nullptr };
     _int BoneIndex = {};
    vector<VTXSKINMESH> m_VBSkinContainer ;
    vector<VTXMESH> m_VBMeshContainer;
    vector<_uint> m_IBContainer ;
    vector<_uint>	m_BoneIndices;		//어떤 본을 사용하는가??

public:
    void Render_GUI() override;
    static CAIMesh* Create(ID3D11Device* pDevice, const aiMesh* pAIMesh, CSkeleton* pSkeleton, MESH_TYPE eType);
    virtual void Free() override;
};

NS_END