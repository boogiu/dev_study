#pragma once
#include "IMesh.h"

BEGIN(Engine)

class ENGINE_DLL CCubeMesh :
    public IMesh
{
private:
    explicit CCubeMesh();
    virtual ~CCubeMesh();
public:
    static CCubeMesh* Create();
    HRESULT Ready_Mesh();
public:
    void SetKey(const string& _key) { m_Key = _key; }
public:
    const LPD3DXMESH& GetMesh() const override {return LPD3DXMESH();};
    DWORD GetFVF() const override { return m_FVF; };
    DWORD GetSubsetCount() const override { return 0; };
    const LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer() const override { return  m_pVB; };
    const LPDIRECT3DINDEXBUFFER9& GetIndexBuffer() const override { return  m_pIB; };;
    const std::string& GetKey() const override { return m_Key; };
    void Create_CubeMesh(LPDIRECT3DDEVICE9 pDevice);
    void Compute_Normals(VTXLIGHTTEX* vertices, DWORD* indices, size_t vertexCount, size_t indexCount);


private:
    DWORD m_FVF;
    string m_Key;

    LPDIRECT3DDEVICE9 m_pDevice;
    LPDIRECT3DVERTEXBUFFER9 m_pVB;
    LPDIRECT3DINDEXBUFFER9 m_pIB;

private:
    void Free() override;
};

END