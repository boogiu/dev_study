#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class CTarget_Manager :
    public CBase
{
private:
	CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTarget_Manager() DEFAULT;

public:

public:
	HRESULT Add_RenderTarget(const string& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const string& strMRTTag, const string& strTargetTag);
	HRESULT Begin_MRT(const string& strMRTTag);
	HRESULT End_MRT();
	HRESULT Get_TargetParam(const string& strTargetTag, SHADER_PARAM& param);
#ifdef _USING_GUI
	void Render_GUI();
#endif // _USING_GUI
private:
	class CRenderTarget* Find_RenderTarget(const string& strTargetTag);
	vector<CRenderTarget*>& Find_MRT(const string& strMRTTag);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	unordered_map<string, class CRenderTarget*>				m_RenderTargets;
	unordered_map<string, vector<class CRenderTarget*>>			m_MRTs;

	ID3D11RenderTargetView* m_pBackBufferRTV = { nullptr };
	ID3D11DepthStencilView* m_pDSV = { nullptr };
#ifdef _USING_GUI
	_float width, height;
#endif // _USING_GUI
public:
	static CTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END

