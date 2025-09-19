#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class ENGINE_DLL CModelData :
    public CBase
{
protected:
    CModelData();
    virtual ~CModelData() override;

public:
    HRESULT Initialize(const string& filePath, ID3D11Device* pDevice);
    HRESULT Render_Mesh(ID3D11DeviceContext* pContext, _uint Index);
    _matrix Get_OffsetMatrix(_uint BoneIndex);
    _int Get_BoneParentIndex(_uint i);
    _float4x4 Get_TransformMatrix(_uint BoneIndex);

public:
    _uint Get_MeshCount() { return m_Meshes.size(); }
    _uint Get_MaterialIndex(_uint meshIndex);
    _uint Get_BoneCount();
    _bool hasMesh() { return !m_Meshes.empty(); }
    _int Find_BoneIndexByName(const string& BoneName);
    const string& Find_BoneNameByIndex(_uint BoneIndex);
    virtual const D3D11_INPUT_ELEMENT_DESC* Get_ElementDesc(_uint DrawIndex);
    virtual const _uint Get_ElementCount(_uint DrawIndex);
    virtual const string_view Get_ElementKey(_uint DrawIndex);
public:
    virtual void Render_GUI();
protected:
    class CSkeleton* m_pSkeleton = { nullptr };
    vector<class CMesh*> m_Meshes;

public:
    static CModelData* Create(const string& filePath, ID3D11Device* pDevice);
    virtual void Free() override;
};

NS_END