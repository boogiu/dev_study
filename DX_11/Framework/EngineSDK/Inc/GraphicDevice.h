#pragma once
#include "IGraphicService.h"
NS_BEGIN(Engine)
class CGraphicDevice :
	public IGraphicService
{
private:
	CGraphicDevice();
	virtual ~CGraphicDevice() override;

public: 
	HRESULT Initialize(HWND hWnd, WINMODE isWindowed, _uint iWinSizeX, _uint iWinSizeY,
		 ID3D11Device** ppDevice,  ID3D11DeviceContext** ppContext);

	virtual HRESULT Clear_BackBuffer_View(const _float4* pClearColor) override;
	virtual HRESULT Clear_DepthStencil_View() override;
	virtual HRESULT Present() override;
	virtual HRESULT Get_InputLayout(
		class CVIBuffer* pBuffer, class CShader* pShader, _uint PassIndex, 
		ID3D11InputLayout** ppInputLayout) override;

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };
	IDXGISwapChain* m_pSwapChain = { nullptr };
	ID3D11RenderTargetView* m_pBackBufferRTV = { nullptr };
	ID3D11DepthStencilView* m_pDepthStencilView = { nullptr };

private:
	HRESULT Ready_SwapChain(HWND hWnd, WINMODE isWindowed, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilView(_uint iWinCX, _uint iWinCY);

private:
	unordered_map<string, ID3D11InputLayout*> m_InputLayouts;

public:
	static  CGraphicDevice* Create(ENGINE_DESC engine, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContextOut);
	virtual void Free() override;
};
NS_END
