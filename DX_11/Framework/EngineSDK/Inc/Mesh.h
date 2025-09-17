#pragma once
#include "VIBuffer.h"
NS_BEGIN(Engine)
class ENGINE_DLL CMesh :
	public CVIBuffer
{
protected:
	CMesh(const string& meshKey);
	virtual ~CMesh();

public:
	virtual HRESULT Initialize_From_File(ID3D11Device* pDevice, ifstream& ifs);
	_uint Get_MaterialIndex() { return m_MaterialIndex; }
private:
	virtual HRESULT Create_AnimateVertex(ID3D11Device* pDevice, ifstream& ifs) ;
	virtual HRESULT Create_StaticVertex(ID3D11Device* pDevice, ifstream& ifs) ;
	virtual HRESULT Create_Index(ID3D11Device* pDevice) override;

public:
	virtual _matrix Get_BoneOffsetMatrix(_uint i);
	virtual _uint Get_BoneIndex(_uint i);
	virtual _uint Get_BoneCount() { return m_BoneIndices.size();};
	const vector<_float4x4>& Bind_BoneMatrices(const vector<class CBone*>& Bones);

protected:
	vector<_uint> m_indices;
	_uint m_MaterialIndex = {}; //어떤 머티리얼을 사용하는가??

	vector<_uint>	m_BoneIndices;				//무슨 본들과 연결되어 있는가?(최종본의 인덱스)
	vector<_float4x4> m_OffsetMatrices;	// 그 본들과의 오프셋(거리)는 어떻게 되어있는가 (걍 인덱스)
	vector<_float4x4> m_BoneMatrices;	
	
public:
	static CMesh* Create(ID3D11Device* pDevice, const string& meshKey,ifstream& ifs);
	virtual void Free() override;
};
NS_END
