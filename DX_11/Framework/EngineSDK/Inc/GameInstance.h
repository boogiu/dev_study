#pragma once
#include "Base.h"

NS_BEGIN(Engine)
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
	class IGraphicService* Get_GraphicDev() { return m_pGraphicDevice; }
	class ITimeService* Get_TimeMgr() { return m_pTimeManager; }
	class IInputService* Get_InputDev() { return m_pInputDevice; }
	class ISoundService* Get_SoundDev() { return m_pSoundDevice; }
	class ILevelService* Get_LevelMgr() { return m_pLevelManager; }

public:
	ID3D11Device* Get_Device() { return m_pDevice; };
	ID3D11DeviceContext* Get_Context() { return m_pDeviceContext; };

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };

#pragma region Game_Managers
	class IGraphicService* m_pGraphicDevice = { nullptr };
	class ITimeService* m_pTimeManager = { nullptr };
	class IInputService* m_pInputDevice = { nullptr };
	class ISoundService* m_pSoundDevice = {nullptr};
	class ILevelService* m_pLevelManager = { nullptr };

#pragma endregion
public:
	virtual void Free() override;
};

NS_END
