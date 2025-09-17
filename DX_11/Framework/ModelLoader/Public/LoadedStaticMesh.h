#pragma once
#include "Loader_Defines.h"
#include "Mesh.h"
NS_BEGIN(Loader)
class CLoadedStaticMesh final:
    public CMesh
{
private:
    CLoadedStaticMesh(const string& meshID);
    virtual ~CLoadedStaticMesh();

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, const aiMesh* pAIMesh);
private:
	virtual HRESULT Create_Vertex(ID3D11Device* pDevice) override;
	virtual HRESULT Create_Index(ID3D11Device* pDevice) override;
private:
    const aiMesh* m_pAIMesh = { nullptr };
    vector<VTXMESH> m_VBContainer ;
    vector<_uint> m_IBContainer ;

public:
    const vector<VTXMESH>& Get_Vertex() { return m_VBContainer; };
    const vector<_uint>& Get_Index() { return m_IBContainer; };

public:
    static CLoadedStaticMesh* Create(ID3D11Device* pDevice, const aiMesh* pAIMesh, const string& meshID);
    virtual void Free() override;
};

NS_END