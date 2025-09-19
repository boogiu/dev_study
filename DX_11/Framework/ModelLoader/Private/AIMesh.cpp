#include "AIMesh.h"
#include "Engine_Defines.h"
#include "Skeleton.h"
#include "AISkeleton.h"
#include "Bone.h"

CAIMesh::CAIMesh(const string& meshID)
	:CMesh(meshID)
{
}

CAIMesh::~CAIMesh()
{
}

HRESULT CAIMesh::Initialize(ID3D11Device* pDevice, const aiMesh* pAIMesh, CSkeleton* pSkeleton, MESH_TYPE eType)
{
	m_iVertexBufferCount = 1;
	m_iVerticesCount = pAIMesh->mNumVertices;
	m_iVertexStride = eType == MESH_TYPE::ANIM ? sizeof(VTXSKINMESH) : sizeof(VTXMESH);

	m_iIndicesCount = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pAIMesh = pAIMesh;
	m_MaterialIndex = pAIMesh->mMaterialIndex;
	m_pSkeleton = pSkeleton;
	m_ElementCount = eType == MESH_TYPE::ANIM ? VTXSKINMESH::iElementCount : VTXMESH::iElementCount;
	m_ElementKey = eType == MESH_TYPE::ANIM ? VTXSKINMESH::Key : VTXMESH::Key;
	m_ElementDesc = eType == MESH_TYPE::ANIM ? VTXSKINMESH::Elements : VTXMESH::Elements;

	Safe_AddRef(m_pSkeleton);
	eType == MESH_TYPE::ANIM ? Create_AnimVertex(pDevice) : Create_Vertex(pDevice);

	Create_Index(pDevice);
	return S_OK;
}

HRESULT CAIMesh::Create_Vertex(ID3D11Device* pDevice)
{

	m_VBMeshContainer.reserve(m_iVerticesCount);

	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = m_iVertexStride * m_iVerticesCount;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	for (size_t i = 0; i < m_iVerticesCount; i++)
	{
		VTXMESH mesh = {};
		memcpy(&mesh.vPosition, &m_pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&mesh.vNormal, &m_pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&mesh.vTexcoord, &m_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&mesh.vTangent, &m_pAIMesh->mTangents[i], sizeof(_float3));
		m_VBMeshContainer.push_back(mesh);
	}

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = m_VBMeshContainer.data();

	HRESULT hr = pDevice->CreateBuffer(&VBDesc, &subData, &m_pVB);
	return hr;
}

HRESULT CAIMesh::Create_AnimVertex(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = m_iVertexStride * m_iVerticesCount;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	m_VBSkinContainer.reserve(m_iVerticesCount);

	for (size_t i = 0; i < m_iVerticesCount; i++)
	{
		VTXSKINMESH mesh = {};
		memcpy(&mesh.vPosition, &m_pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&mesh.vNormal, &m_pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&mesh.vTexcoord, &m_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&mesh.vTangent, &m_pAIMesh->mTangents[i], sizeof(_float3));
		m_VBSkinContainer.push_back(mesh);
	}
	/*만약 본의 개수가 없더라면.*/
	_uint NumBones = m_pAIMesh->mNumBones;

	for (size_t i = 0; i < NumBones; i++)
	{
		aiBone* pAIBone = m_pAIMesh->mBones[i];
		string BoneName = pAIBone->mName.data;
		_int BoneIndex = m_pSkeleton->Find_BoneIndexByName(BoneName);
		m_BoneIndices.push_back(BoneIndex);

		_float4x4 m_OffsetMatrix = {};
		memcpy(&m_OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_OffsetMatrix)));
		static_cast<CAISkeleton*>(m_pSkeleton)->Set_Offset(BoneIndex, m_OffsetMatrix);

		for (size_t j = 0; j < pAIBone->mNumWeights; j++)
		{
			aiVertexWeight     AIWeight = pAIBone->mWeights[j];

			if (0.0f == m_VBSkinContainer[AIWeight.mVertexId].vBlendWeight.x)
			{
				m_VBSkinContainer[AIWeight.mVertexId].vBlendIndex.x = BoneIndex;
				m_VBSkinContainer[AIWeight.mVertexId].vBlendWeight.x = AIWeight.mWeight;
			}

			else if (0.0f == m_VBSkinContainer[AIWeight.mVertexId].vBlendWeight.y)
			{
				m_VBSkinContainer[AIWeight.mVertexId].vBlendIndex.y = BoneIndex;
				m_VBSkinContainer[AIWeight.mVertexId].vBlendWeight.y = AIWeight.mWeight;
			}

			else if (0.0f == m_VBSkinContainer[AIWeight.mVertexId].vBlendWeight.z)
			{
				m_VBSkinContainer[AIWeight.mVertexId].vBlendIndex.z = BoneIndex;
				m_VBSkinContainer[AIWeight.mVertexId].vBlendWeight.z = AIWeight.mWeight;
			}

			else if (0.0f == m_VBSkinContainer[AIWeight.mVertexId].vBlendWeight.w)
			{
				m_VBSkinContainer[AIWeight.mVertexId].vBlendIndex.w = BoneIndex;
				m_VBSkinContainer[AIWeight.mVertexId].vBlendWeight.w = AIWeight.mWeight;
			}
		}
	}

	if (0 == NumBones)
	{
		_int BoneIndex = m_pSkeleton->Find_BoneIndexByName(m_VIKey);
		_float4x4       OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
		/*m_OffsetMatrices.push_back(OffsetMatrix);*/
		m_BoneIndices.push_back(BoneIndex);
	}


	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = m_VBSkinContainer.data();
	HRESULT hr = pDevice->CreateBuffer(&VBDesc, &subData, &m_pVB);

	return hr;
}

HRESULT CAIMesh::Create_Index(ID3D11Device* pDevice)
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

void CAIMesh::Save_File(ofstream& ofs)
{
	MESH_INFO_HEADER infoHeader = {};
	infoHeader.BoneCount = m_BoneIndices.size();
	infoHeader.IndicesCount = m_iIndicesCount;
	infoHeader.VerticesCount = m_iVerticesCount;
	strcpy_s(infoHeader.MeshName, m_VIKey.data());
	infoHeader.MaterialIndex = m_MaterialIndex;
	ofs.write(reinterpret_cast<const char*>(&infoHeader), sizeof(MESH_INFO_HEADER));

	if (m_iVertexStride == sizeof(VTXSKINMESH)) {
		for (VTXSKINMESH& vertex : m_VBSkinContainer) {
			ofs.write(reinterpret_cast<const char*>(&vertex), sizeof(VTXSKINMESH));
		}
	}

	else if(m_iVertexStride == sizeof(VTXMESH)){
		for (VTXMESH& vertex : m_VBMeshContainer) {
			ofs.write(reinterpret_cast<const char*>(&vertex), sizeof(VTXSKINMESH));
		}
	}

	for(_uint indices : m_IBContainer)
		ofs.write(reinterpret_cast<const char*>(&indices), sizeof(_uint));

	//for()
}

CAIMesh* CAIMesh::Create(ID3D11Device* pDevice, const aiMesh* pAIMesh, CSkeleton* pSkeleton, MESH_TYPE eType)
{
	CAIMesh* instance = new CAIMesh(pAIMesh->mName.data);
	if (FAILED(instance->Initialize(pDevice, pAIMesh, pSkeleton, eType))) {
		Safe_Release(instance);
	}

	instance->m_pAIMesh = { nullptr };
	return instance;
}

void CAIMesh::Free()
{
	m_VBSkinContainer.clear();
	m_IBContainer.clear();
	__super::Free();
	Safe_Release(m_pSkeleton);
}

