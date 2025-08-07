#pragma once
#include "IMesh.h"
BEGIN(Engine)

class ENGINE_DLL CTerrain :
	public IMesh
{
public:
	explicit CTerrain();
	explicit CTerrain(CTerrain& rhs);
	virtual ~CTerrain();
public:
	static CTerrain* Create();
public:
	HRESULT Ready_Mesh();
public:
	const LPD3DXMESH& GetMesh() const override { return LPD3DXMESH(); };
	DWORD GetFVF() const override { return m_FVF; };
	DWORD GetSubsetCount() const override { return 0; };
	const LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer() const override { return m_pVB; };
	const LPDIRECT3DINDEXBUFFER9& GetIndexBuffer() const override { return m_pIB; };
	const wstring& GetKey() const override { return m_key; };
	vector<float>& GetHeightMap()  { return m_heightMap; };
	int Get_Row() { return m_iRow; }
	int Get_Col() { return m_iCol; }

public:
	void SetFVF(DWORD fvf) { m_FVF = fvf; }
	void SetVTX_Buffer(const LPDIRECT3DVERTEXBUFFER9& vb);
	void SetIDX_Buffer(const LPDIRECT3DINDEXBUFFER9& ib);
	void SetKey(const wstring& key) { m_key = key; }
	void SetKey(vector<float> map) { m_heightMap = map; }
	void SetVTX_Num(int row, int col) { m_iCol = col; m_iRow = row; }
	void ComputeIDX();
	CTerrain* Clone();

private:
	wstring m_key;
	DWORD m_FVF;
	LPDIRECT3DDEVICE9 m_pDevice;
	vector<float> m_heightMap;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	
	int m_iRow;
	int m_iCol;

private:
	void Free() override;
};

END
