#pragma once
#include "IMesh.h"

BEGIN(Engine)
class ENGINE_DLL CMesh :
	public IMesh
{//해당 메시는 스태틱 메시임
private:
	explicit CMesh();
	virtual ~CMesh() override;
public:
	static CMesh* Create();
	HRESULT Ready_Mesh();

public: //Getter
	const LPD3DXMESH& GetMesh() const override { return m_pMesh; };
	DWORD GetFVF() const override { return m_FVF; };
	DWORD GetSubsetCount() const override { return m_dwSubsetCnt; };
	const LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer() const override { return LPDIRECT3DVERTEXBUFFER9(); };
	const LPDIRECT3DINDEXBUFFER9& GetIndexBuffer() const override { return LPDIRECT3DINDEXBUFFER9(); };
	const string& GetKey() const override { return m_Key; }

public://Setter
	void SetKey(const string& _key) { m_Key = _key; }
	void SetMesh(LPD3DXMESH mesh);
public:
	HRESULT Load(const string pFile);
	void Debug_VertexUV();

private:
	//서브셋 개수 확인
	DWORD m_dwSubsetCnt;
	DWORD m_FVF;
	LPD3DXMESH m_pMesh;
	LPDIRECT3DDEVICE9 m_pDevice;
	vector<D3DXATTRIBUTERANGE> m_Subset;

	string m_Key;

private:
	void Free() override;

};

END