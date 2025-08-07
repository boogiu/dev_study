#pragma once
#include "Base.h"
#include "ServiceHub.h"


NS_BEGIN(Engine)
class IGraphicService;
class ENGINE_DLL CGameInstance :
	public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	explicit CGameInstance();
	virtual ~CGameInstance();

public:
	_bool InitSystems(const ENGINE_DESC& engine);
	_bool InitDirectX();

	void Update_Engine(_float dt);

	_bool HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT Draw_Begin(_float4* pColor);
	HRESULT Draw();
	HRESULT Draw_End();

public: 	
	template<typename T>
	T* Get_Service();

public:
	ID3D11Device* Get_Device() { return m_pDevice; };
	ID3D11DeviceContext* Get_Context() { return m_pDeviceContext; };

private:
	CServiceHub m_ServiceHub;
	IGraphicService* m_pGraphicService = { nullptr };
	ID3D11Device* m_pDevice = {nullptr};
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };

public:
	virtual void Free() override;
};

NS_END

template<typename T>
inline T* CGameInstance::Get_Service()
{
	return m_ServiceHub.Get_Service<T>();
}
