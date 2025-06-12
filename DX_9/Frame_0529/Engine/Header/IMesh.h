#pragma once
#include "CBase.h"

BEGIN(Engine)

class ENGINE_DLL IMesh :
    public CBase
{
protected:
    virtual ~IMesh() = default;
public:
    // 메시 접근
    virtual const LPD3DXMESH& GetMesh() const = 0;

    // FVF 및 서브셋 정보
    virtual DWORD GetFVF() const = 0;
    virtual DWORD GetSubsetCount() const = 0;

    // 정점/인덱스 버퍼 직접 접근 (필요 시)
    virtual const LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer() const = 0;
    virtual const LPDIRECT3DINDEXBUFFER9& GetIndexBuffer() const = 0;

    // 경로
    virtual const std::string& GetKey() const = 0;
};

END