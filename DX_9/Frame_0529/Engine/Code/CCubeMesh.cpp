#include "Engine_Define.h"
#include "CCubeMesh.h"
#include "CGraphicDev.h"
#include "CResourceMgr.h"

CCubeMesh::CCubeMesh()
	:m_FVF(FVF_LIGHTTEX),m_pVB(nullptr),m_pIB(nullptr),m_pDevice(nullptr)
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
    HRESULT hr = m_pDevice->CreateVertexBuffer(sizeof(VTXLIGHTTEX) * numVertices,
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

    VTXLIGHTTEX cube[] = {
        // 뒷면 (-Z)
        { {-s,+s,-s}, { 0,  0, -1 }, {0.33f, 0.0f} },  // 0
        { {+s,+s,-s}, { 0,  0, -1 }, {0.66f, 0.0f} },  // 1
        { {+s,-s,-s}, { 0,  0, -1 }, {0.66f, 0.66f} }, // 2
        { {-s,-s,-s}, { 0,  0, -1 }, {0.33f, 0.66f} }, // 3

        // 앞면 (+Z)
        { {-s,+s,+s}, { 0,  0,  1 }, {0.33f, 0.33f} }, // 4
        { {+s,+s,+s}, { 0,  0,  1 }, {0.66f, 0.33f} }, // 5
        { {+s,-s,+s}, { 0,  0,  1 }, {0.66f, 1.0f} },  // 6
        { {-s,-s,+s}, { 0,  0,  1 }, {0.33f, 1.0f} },  // 7
    };

    DWORD indices[] =
    {
        3,0,1, 3,1,2,
        6,5,4, 6,4,7,
        2,1,5,2,5,6,
        7,4,0,7,0,3,
        0,4,5,0,5,1,
        7,3,2,7,2,6
    };

    Compute_Normals(cube, indices, numVertices, numIndices);

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
