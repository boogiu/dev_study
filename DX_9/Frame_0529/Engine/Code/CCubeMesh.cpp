#include "Engine_Define.h"
#include "CCubeMesh.h"
#include "CGraphicDev.h"
#include "CResourceMgr.h"

CCubeMesh::CCubeMesh()
	:m_FVF(FVF_CUBE),m_pVB(nullptr),m_pIB(nullptr),m_pDevice(nullptr)
{
}

CCubeMesh::~CCubeMesh()
{
}

CCubeMesh* CCubeMesh::Create()
{
	CCubeMesh* instance = new CCubeMesh;

	if (FAILED(instance->Ready_Mesh())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CCubeMesh::Ready_Mesh()
{
	m_pDevice = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (m_pDevice) {
		m_pDevice->AddRef();
    Create_CubeMesh(m_pDevice);
    return S_OK;
    }

	return E_FAIL;
}


void CCubeMesh::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
 
void CCubeMesh::Create_CubeMesh(LPDIRECT3DDEVICE9 pDevice)
{
    const DWORD numIndices = 36;
    const DWORD numVertices = 8;

    // 정점 버퍼 생성
    HRESULT hr = m_pDevice->CreateVertexBuffer(sizeof(VTXCUBE) * numVertices,
        0, m_FVF, D3DPOOL_MANAGED, &m_pVB, 0);
    if (FAILED(hr)) {
        return;
    }

    // 인덱스 버퍼 생성
    hr= m_pDevice->CreateIndexBuffer(sizeof(DWORD) * numIndices,
        0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, 0);
    if (FAILED(hr)) {
        return;
    }

    float s = 1.f;

    VTXCUBE cube[8] = {};

    // 전면
    cube[0].vPosition = { -1.f, 1.f, -1.f };
    cube[0].vTexUV = cube[0].vPosition;

    cube[1].vPosition = { 1.f, 1.f, -1.f };
    cube[1].vTexUV = cube[1].vPosition;

    cube[2].vPosition = { 1.f, -1.f, -1.f };
    cube[2].vTexUV = cube[2].vPosition;

    cube[3].vPosition = { -1.f, -1.f, -1.f };
    cube[3].vTexUV = cube[3].vPosition;

    cube[4].vPosition = { -1.f, 1.f, 1.f };
    cube[4].vTexUV = cube[4].vPosition;

    cube[5].vPosition = { 1.f, 1.f, 1.f };
    cube[5].vTexUV = cube[5].vPosition;

    cube[6].vPosition = { 1.f, -1.f, 1.f };
    cube[6].vTexUV = cube[6].vPosition;

    cube[7].vPosition = { -1.f, -1.f, 1.f };
    cube[7].vTexUV = cube[7].vPosition;

    INDEX32 indices[12] = {};
    // X+
    // 오른쪽 위                    // 왼쪽 아래
    indices[0]._0 = 1;        indices[1]._0 = 1;
    indices[0]._1 = 5;        indices[1]._1 = 6;;
    indices[0]._2 = 6;        indices[1]._2 = 2;
                                      
    // X-
    // 오른쪽 위                    // 왼쪽 아래
    indices[2]._0 = 4;       indices[3]._0 = 4;
    indices[2]._1 = 0;        indices[3]._1 = 3;
    indices[2]._2 = 3;       indices[3]._2 = 7;

    // Y+
    // 오른쪽 위                      // 왼쪽 아래
    indices[4]._0 = 4;       indices[5]._0 = 4;
    indices[4]._1 = 5;         indices[5]._1 = 1;
    indices[4]._2 = 1;         indices[5]._2 = 0;

    // Y-
    // 오른쪽 위                          // 왼쪽 아래
    indices[6]._0 = 3;            indices[7]._0 = 3;
    indices[6]._1 = 2;             indices[7]._1 = 6;
    indices[6]._2 = 6;            indices[7]._2 = 7;

    // Z+
    // 오른쪽 위                        // 왼쪽 아래
    indices[8]._0 = 7;          indices[9]._0 = 7;
    indices[8]._1 = 6;           indices[9]._1 = 5;
    indices[8]._2 = 5;          indices[9]._2 = 4;

    // Z-
    // 오른쪽 위                        // 왼쪽 아래
    indices[10]._0 = 0;            indices[11]._0 = 0;
    indices[10]._1 = 1;              indices[11]._1 = 2;
    indices[10]._2 = 2;            indices[11]._2 = 3;

    // 정점 정의
    void* pVertices = nullptr;
    m_pVB->Lock(0, 0, &pVertices, m_FVF);
    memcpy(pVertices, cube, sizeof(cube));
    m_pVB->Unlock();

    // 인덱스 정의
    void* pIndices = nullptr;
    m_pIB->Lock(0, 0, &pIndices, D3DFMT_INDEX32);
    memcpy(pIndices, indices, sizeof(indices));
    m_pIB->Unlock();
}

void CCubeMesh::Compute_Normals(VTXLIGHTTEX* vertices, DWORD* indices, size_t vertexCount, size_t indexCount)
{
    // 1. 모든 정점의 법선을 초기화
    for (size_t i = 0; i < vertexCount; ++i) {
        vertices[i].vNorm = { 0.f, 0.f, 0.f };
    }

    // 2. 삼각형 단위로 순회하며 면 법선을 각 정점에 누적
    for (size_t i = 0; i < indexCount; i += 3)
    {
        DWORD i0 = indices[i];
        DWORD i1 = indices[i + 1];
        DWORD i2 = indices[i + 2];

        _vec3& v0 = vertices[i0].vPosition;
        _vec3& v1 = vertices[i1].vPosition;
        _vec3& v2 = vertices[i2].vPosition;

        _vec3 edge1 = v1 - v0;
        _vec3 edge2 = v2 - v0;

        _vec3 faceNormal;
        D3DXVec3Cross(&faceNormal, &edge1, &edge2);
        D3DXVec3Normalize(&faceNormal, &faceNormal);

        vertices[i0].vNorm += faceNormal;
        vertices[i1].vNorm += faceNormal;
        vertices[i2].vNorm += faceNormal;
    }
    // 3. 누적된 정점 법선 정규화
    for (size_t i = 0; i < vertexCount; ++i) {
        D3DXVec3Normalize(&vertices[i].vNorm, &vertices[i].vNorm);
    }
}
