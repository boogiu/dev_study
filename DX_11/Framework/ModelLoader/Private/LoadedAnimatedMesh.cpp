#include "LoadedAnimatedMesh.h"
#include "Engine_Defines.h"
#include "Skeleton.h"

CLoadedAnimatedMesh::CLoadedAnimatedMesh(const string& meshID)
	:CMesh(meshID)
{
}

CLoadedAnimatedMesh::~CLoadedAnimatedMesh()
{
}

HRESULT CLoadedAnimatedMesh::Initialize(ID3D11Device* pDevice, const aiMesh* pAIMesh, CSkeleton* pSkeleton)
{
	m_iVertexBufferCount = 1;
	m_iVerticesCount = pAIMesh->mNumVertices;
	m_iVertexStride = sizeof(VTXSKINMESH);

	m_iIndicesCount = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pAIMesh = pAIMesh;
	m_MaterialIndex = pAIMesh->mMaterialIndex;
	m_pSkeleton = pSkeleton;
	Safe_AddRef(m_pSkeleton);

	Create_Vertex(pDevice);
	Create_Index(pDevice);
	return S_OK;
}

HRESULT CLoadedAnimatedMesh::Create_Vertex(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = m_iVertexStride * m_iVerticesCount;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	m_VBContainer.reserve(m_iVerticesCount);

    for (size_t i = 0; i < m_iVerticesCount; i++)
    {
		VTXSKINMESH mesh = {};
        memcpy(&mesh.vPosition,		&m_pAIMesh->mVertices[i], sizeof(_float3));
        memcpy(&mesh.vNormal,			&m_pAIMesh->mNormals[i], sizeof(_float3));
        memcpy(&mesh.vTexcoord,	&m_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
        memcpy(&mesh.vTangent,		&m_pAIMesh->mTangents[i], sizeof(_float3));
		m_VBContainer.push_back(mesh);
    }
	/*만약 본의 개수가 없더라면.*/
	_uint NumBones = m_pAIMesh->mNumBones;

	for (size_t i = 0; i < NumBones; i++)
	{
		aiBone* pAIBone = m_pAIMesh->mBones[i];
		string BoneName = pAIBone->mName.data;
		_int BoneIndex = m_pSkeleton->FindBoneIndex_ByName(BoneName);
		
		_float4x4 m_OffsetMatrix = {};
		memcpy(&m_OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		m_OffsetMatrices.push_back(m_OffsetMatrix);
		m_BoneIndices.push_back(BoneIndex);

		for (size_t j = 0; j < pAIBone->mNumWeights; j++)
		{
			aiVertexWeight     AIWeight = pAIBone->mWeights[j];

			if (0.0f == m_VBContainer[AIWeight.mVertexId].vBlendWeight.x)
			{
				m_VBContainer[AIWeight.mVertexId].vBlendIndex.x = i;
				m_VBContainer[AIWeight.mVertexId].vBlendWeight.x = AIWeight.mWeight;
			}

			else if (0.0f == m_VBContainer[AIWeight.mVertexId].vBlendWeight.y)
			{
				m_VBContainer[AIWeight.mVertexId].vBlendIndex.y = i;
				m_VBContainer[AIWeight.mVertexId].vBlendWeight.y = AIWeight.mWeight;
			}

			else if (0.0f == m_VBContainer[AIWeight.mVertexId].vBlendWeight.z)
			{
				m_VBContainer[AIWeight.mVertexId].vBlendIndex.z = i;
				m_VBContainer[AIWeight.mVertexId].vBlendWeight.z = AIWeight.mWeight;
			}

			else if (0.0f == m_VBContainer[AIWeight.mVertexId].vBlendWeight.w)
			{
				m_VBContainer[AIWeight.mVertexId].vBlendIndex.w = i;
				m_VBContainer[AIWeight.mVertexId].vBlendWeight.w = AIWeight.mWeight;
			}
		}
	}

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = m_VBContainer.data();
	HRESULT hr = pDevice->CreateBuffer(&VBDesc, &subData, &m_pVB);

	return hr;
}

HRESULT CLoadedAnimatedMesh::Create_Index(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC IDDesc;
	IDDesc.ByteWidth = m_iIndexStride * m_iIndicesCount;
	IDDesc.Usage = D3D11_USAGE_DEFAULT;
	IDDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IDDesc.CPUAccessFlags = 0;
	IDDesc.MiscFlags = 0;
	IDDesc.StructureByteStride = m_iIndexStride;

	m_IBContainer.reserve(m_iIndicesCount);


	for (size_t i = 0; i < m_pAIMesh->mNumFaces; i++)
	{
		m_IBContainer.push_back(m_pAIMesh->mFaces[i].mIndices[0]);
		m_IBContainer.push_back(m_pAIMesh->mFaces[i].mIndices[1]);
		m_IBContainer.push_back(m_pAIMesh->mFaces[i].mIndices[2]);
	}

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = m_IBContainer.data();

	HRESULT hr = pDevice->CreateBuffer(&IDDesc, &subData, &m_pIB);

	return hr;
}

CLoadedAnimatedMesh* CLoadedAnimatedMesh::Create(ID3D11Device* pDevice, const aiMesh* pAIMesh, CSkeleton* pSkeleton)
{
	CLoadedAnimatedMesh* instance = new CLoadedAnimatedMesh(pAIMesh->mName.data);
	if (FAILED(instance->Initialize(pDevice, pAIMesh, pSkeleton))) {
		Safe_Release(instance);
	}

	instance->m_pAIMesh = { nullptr };
	return instance;
}

void CLoadedAnimatedMesh::Free()
{
	m_VBContainer.clear();
	m_IBContainer.clear();
	Safe_Release(m_pSkeleton);

	__super::Free();
}

