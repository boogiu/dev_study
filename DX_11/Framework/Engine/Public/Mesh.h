#pragma once
#include "VIBuffer.h"
NS_BEGIN(Engine)
class ENGINE_DLL CMesh :
	public CVIBuffer
{
protected:
	CMesh();
	CMesh(const string& ModelKey);
	virtual ~CMesh();

public:
	virtual HRESULT Initialize_From_File(ID3D11Device* pDevice, ifstream& ifs, MESH_TYPE eType);
private:
	virtual HRESULT Create_AnimateVertex(ID3D11Device* pDevice, ifstream& ifs) ;
	virtual HRESULT Create_StaticVertex(ID3D11Device* pDevice, ifstream& ifs) ;
	virtual HRESULT Create_Index(ID3D11Device* pDevice) override;

public:
	_uint Get_MaterialIndex() { return m_MaterialIndex; }

public:
	virtual void Render_GUI();

protected:
	vector<_uint> m_indices;						//임시 인덱스 (함수로 따로 빼려고 하다보니 생김)
	_uint m_MaterialIndex = {};					//어떤 머티리얼을 사용하는가??

	
public:
	static CMesh* Create(ID3D11Device* pDevice,ifstream& ifs, MESH_TYPE eType);
	virtual void Free() override;
};
NS_END
