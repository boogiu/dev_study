#include "PipeLine.h"
#include "GameInstance.h"
#include "ICameraService.h"
#include "IResourceService.h"
#include "Shader.h"
#include "Model.h"

CPipeLine::CPipeLine()
{
}

HRESULT CPipeLine::Initialize(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(FrameBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	pDevice->CreateBuffer(&desc, nullptr, &m_pDeviceFrameBuffer);
	desc.ByteWidth = sizeof(ObjectBuffer);
	pDevice->CreateBuffer(&desc, nullptr, &m_pDeviceObjectBuffer);
	desc.ByteWidth = sizeof(LightBuffer);
	pDevice->CreateBuffer(&desc, nullptr, &m_pDeviceLightBuffer);
	desc.ByteWidth = sizeof(SkinningBuffer);
	pDevice->CreateBuffer(&desc, nullptr, &m_pDeviceSkinningBuffer);

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

/*오브젝트 연산 단위의 버퍼*/
HRESULT CPipeLine::Update_ObjectBuffer(ID3D11DeviceContext* pContext, _float4x4* pMatrix)
{
	ObjectBuffer objectBuffer{};

	objectBuffer.matWorld = *pMatrix;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = pContext->Map(
		m_pDeviceObjectBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);
	if (FAILED(hr))
		return hr;

	memcpy(mappedResource.pData, &objectBuffer, sizeof(ObjectBuffer));

	pContext->Unmap(m_pDeviceObjectBuffer, 0);

	return S_OK;
}

/*메쉬 혹은 모델 단위의 버퍼*/
HRESULT CPipeLine::Update_SkinningBuffer(ID3D11DeviceContext* pContext, const vector<_float4x4>& BoneMatrices)
{
	if (BoneMatrices.empty())
		return S_OK;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = pContext->Map(
		m_pDeviceSkinningBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);

	if (FAILED(hr))
		return hr;

	size_t dataSize = sizeof(_float4x4) * BoneMatrices.size();
	memcpy(mappedResource.pData, BoneMatrices.data(), dataSize);
	pContext->Unmap(m_pDeviceSkinningBuffer, 0);

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
}
