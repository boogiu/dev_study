#include "PipeLine.h"
#include "GameInstance.h"
#include "ICameraService.h"
#include "IResourceService.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"

CPipeLine::CPipeLine()
{
}

HRESULT CPipeLine::Initialize(ID3D11Device* pDevice)
{

	/*상수 버퍼*/
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(FrameBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	pDevice->CreateBuffer(&desc, nullptr, &m_pDeviceFrameBuffer);

	desc.ByteWidth = sizeof(LightBuffer);
	pDevice->CreateBuffer(&desc, nullptr, &m_pDeviceLightBuffer);

	/*프레임 시작 시 한번에 모든 트랜스폼 바인딩*/
	desc.ByteWidth = sizeof(ObjectBufferArray);  
	pDevice->CreateBuffer(&desc, nullptr, &m_pDeviceObjectBuffer);

	/*구조체 버퍼 - > 이건 셰이더 리소스 뷰도 같이 만들어버림*/
	vector<_float4x4> BoneMatrices;
	BoneMatrices.resize(g_iMaxNumBones);

	D3D11_BUFFER_DESC SkinningBufferDesc = {};
	SkinningBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	SkinningBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	SkinningBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	SkinningBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	SkinningBufferDesc.StructureByteStride = sizeof(_float4x4);
	SkinningBufferDesc.ByteWidth = sizeof(_float4x4) * g_iMaxNumBones;

	pDevice->CreateBuffer(&SkinningBufferDesc, nullptr, &m_pDeviceSkinningBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC SkinningResourceDesc = {};
	SkinningResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX; /*텍스처가 아니다! */
	SkinningResourceDesc.BufferEx.NumElements = g_iMaxNumBones;
	SkinningResourceDesc.Format = DXGI_FORMAT_UNKNOWN;										/*픽셀 아님*/

	pDevice->CreateShaderResourceView(m_pDeviceSkinningBuffer, &SkinningResourceDesc, &m_pSkinningResource);

	return S_OK;
}

/*프레임 단위의 버퍼*/
HRESULT CPipeLine::Update_FrameBuffer(ID3D11DeviceContext* pContext)
{
	FrameBuffer frameBuffer{};

	frameBuffer.matProjection = *CGameInstance::GetInstance()->Get_CameraMgr()->Get_ProjMatrix();
	frameBuffer.matView = *CGameInstance::GetInstance()->Get_CameraMgr()->Get_ViewMatrix();

	_float2 clientSize = CGameInstance::GetInstance()->Get_ClientSize();
	if (clientSize.x == 0 || clientSize.y == 0)
		return E_FAIL;

	_float4x4 OrthoProject;
	XMStoreFloat4x4(&OrthoProject, XMMatrixOrthographicLH(clientSize.x, clientSize.y, 0.f, 1.f));
	frameBuffer.matOrthograph = OrthoProject;
	frameBuffer.vCamPosition = CGameInstance::GetInstance()->Get_CameraMgr()->Get_CameraPos();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = pContext->Map(
		m_pDeviceFrameBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);
	if (FAILED(hr))
		return hr;

	memcpy(mappedResource.pData, &frameBuffer, sizeof(FrameBuffer));
	pContext->Unmap(m_pDeviceFrameBuffer, 0);

	return S_OK;
}

/*빛 연산 단위의 버퍼*/
HRESULT CPipeLine::Update_LightBuffer(ID3D11DeviceContext* pContext)
{
	LightBuffer lightBuffer{};

	lightBuffer.vLightDir = { 0,-1,0,0 };
	lightBuffer.vLightDiffuse = { 1.f, 1.f, 1.f, 1.f };
	lightBuffer.vLightAmbient = { 0.8f,0.8f,0.8f,1 };
	lightBuffer.vLightSpecular = { 1.f, 1.f, 1.f, 1.f };

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = pContext->Map(
		m_pDeviceLightBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);
	if (FAILED(hr))
		return hr;

	memcpy(mappedResource.pData, &lightBuffer, sizeof(LightBuffer));

	pContext->Unmap(m_pDeviceLightBuffer, 0);

	return S_OK;
}

_uint CPipeLine::Write_ObjectData(const _float4x4& worldMatrix)
{
	if (!m_pObjectBufferArray)
		return UINT_MAX; // Begin_ObjectBuffer 안 했을 경우

	_uint index = m_ObjectBufferCount++;
	m_pObjectBufferArray->Objects[index].matWorld = worldMatrix;

	return index; // 인덱스를 반환해서 셰이더에서 사용
}

HRESULT CPipeLine::Begin_ObjectBuffer(ID3D11DeviceContext* pContext)
{
	HRESULT hr = pContext->Map(
		m_pDeviceObjectBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&m_mappedObjectBuffer
	);

	if (FAILED(hr))
		return hr;

	m_pObjectBufferArray = reinterpret_cast<ObjectBufferArray*>(m_mappedObjectBuffer.pData);
	m_ObjectBufferCount = 0;

	return S_OK;
}

HRESULT CPipeLine::End_ObjectBuffer(ID3D11DeviceContext* pContext)
{
	pContext->Unmap(m_pDeviceObjectBuffer, 0);
	m_pObjectBufferArray = nullptr;
	m_ObjectBufferCount = 0;

	return S_OK;
}

_uint CPipeLine::Write_SkinningBuffer(const vector<_float4x4>& bones)
{
	if (!m_pSkinningArray) return UINT_MAX;

	const _uint SkinningCount = static_cast<_uint>(bones.size());

	if (m_SkinningOffset + SkinningCount > g_iMaxNumBones) {
		return UINT_MAX; // 초과
	}

	memcpy(&m_pSkinningArray[m_SkinningOffset], bones.data(), sizeof(_float4x4) * SkinningCount);
	_uint LastOffset = m_SkinningOffset;
	m_SkinningOffset += SkinningCount;
	return LastOffset;
}

HRESULT CPipeLine::Begin_SkinningBuffer(ID3D11DeviceContext* pContext)
{
	HRESULT hr = pContext->Map(
		m_pDeviceSkinningBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&m_mappedSkinningBuffer
	);

	if (FAILED(hr))
		return hr;

	m_pSkinningArray = reinterpret_cast<_float4x4*>(m_mappedSkinningBuffer.pData);
	m_SkinningOffset = 0;   
	return S_OK;
}

HRESULT CPipeLine::End_SkinningBuffer(ID3D11DeviceContext* pContext)
{
	pContext->Unmap(m_pDeviceSkinningBuffer, 0);

	m_pSkinningArray = nullptr;
	m_SkinningOffset = 0;
	return S_OK;
}

HRESULT CPipeLine::Bind_PaletteTexture(CShader* pShader)
{
	SHADER_PARAM palette = {};
	palette.iSize = 0;
	palette.typeName = "Texture2D";

	for (auto& pair : m_Palette) {
		palette.pData = pair.second->Get_SRV();
		pShader->Bind_Value(pair.first, palette);
	}
	return S_OK;
}

HRESULT CPipeLine::Add_Palette(const string& ConstantName, CTexture* pTexture)
{

	auto iter = m_Palette.emplace(ConstantName, pTexture);
	if (iter.second) {
		/*성공*/
		Safe_AddRef(pTexture);
	}
	else {
		/*실패*/
		Safe_Release(m_Palette[ConstantName]);
		m_Palette[ConstantName] = pTexture;
		Safe_AddRef(pTexture);
	}
	return S_OK;
}


CPipeLine* CPipeLine::Create(ID3D11Device* pDevice)
{
	CPipeLine* instance = new CPipeLine();
	if (FAILED(instance->Initialize(pDevice)))
	{
		Safe_Release(instance);
	}
	return instance;
}

void CPipeLine::Free()
{
	__super::Free();
	Safe_Release(m_pDeviceFrameBuffer);
	Safe_Release(m_pDeviceObjectBuffer);
	Safe_Release(m_pDeviceLightBuffer);
	Safe_Release(m_pDeviceSkinningBuffer);
	Safe_Release(m_pSkinningResource);

	for (auto& pair : m_Palette) {
		Safe_Release(pair.second);
	}
}
