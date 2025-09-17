#pragma once
#include "Loader_Defines.h"
#include "Mesh.h"
NS_BEGIN(Engine)
    class CSkeleton;
NS_END
NS_BEGIN(Loader)
class CLoadedAnimatedMesh final:
    public CMesh
{
private:
    CLoadedAnimatedMesh(const string& meshID);
    virtual ~CLoadedAnimatedMesh();

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, const aiMesh* pAIMesh,  CSkeleton* pSkeleton);
private:
	virtual HRESULT Create_Vertex(ID3D11Device* pDevice) override;
	virtual HRESULT Create_Index(ID3D11Device* pDevice) override;
private:
    const aiMesh* m_pAIMesh = { nullptr };
     CSkeleton* m_pSkeleton = { nullptr };

    vector<VTXSKINMESH> m_VBContainer ;
    vector<_uint> m_IBContainer ;

public:
    const vector<VTXSKINMESH>& Get_Vertex() { return m_VBContainer; };
    const vector<_uint>& Get_Index() { return m_IBContainer; };
    const vector<_float4x4>& Get_Offset() { return m_OffsetMatrices; };
    const vector<_uint>& Get_BoneIndices() { return m_BoneIndices; };
public:
    static CLoadedAnimatedMesh* Create(ID3D11Device* pDevice, const aiMesh* pAIMesh, CSkeleton* pSkeleton);
    virtual void Free() override;
};

NS_END