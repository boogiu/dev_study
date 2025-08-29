#pragma once
#include "IService.h"
NS_BEGIN(Engine)
class ENGINE_DLL IGraphicService abstract :
public IService
{
protected:
	virtual ~IGraphicService() DEFAULT;

public:
	virtual HRESULT Clear_BackBuffer_View(const _float4* pClearColor) PURE;
	virtual HRESULT Clear_DepthStencil_View() PURE;
	virtual HRESULT Present() PURE;
	virtual HRESULT Get_InputLayout(
		class CVIBuffer* pBuffer,
		class CShader* pShader, _uint PassIndex,
		ID3D11InputLayout** ppInputLayout) PURE;
};

NS_END